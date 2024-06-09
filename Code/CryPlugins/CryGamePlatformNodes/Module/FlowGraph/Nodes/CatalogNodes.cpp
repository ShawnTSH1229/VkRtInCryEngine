// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "CatalogNodes.h"

#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>

namespace Cry::GamePlatform
{

using ECatalogSortOrder = IService::ECatalogSortOrder;
#define FLOWGRAPH_CATSORT_UICONFIG "enum_int:AllPurchaseCountDaily=0,DigitalReleaseDate=1,PaidPurchaseCountAllTime=2,PaidPurchaseCountDaily=3,PhysicalReleaseDate=4,UserRating=5"

static inline ECatalogSortOrder UIConfigToCatalogSortOrder(int uicValue)
{
	switch (uicValue)
	{
	case 0:
		return ECatalogSortOrder::AllPurchaseCountDaily;
	case 1:
		return ECatalogSortOrder::DigitalReleaseDate;
	case 2:
		return ECatalogSortOrder::PaidPurchaseCountAllTime;
	case 3:
		return ECatalogSortOrder::PaidPurchaseCountDaily;
	case 4:
		return ECatalogSortOrder::PhysicalReleaseDate;
	case 5:
		return ECatalogSortOrder::UserRating;
	}

	return ECatalogSortOrder::UserRating;
}

void CFlowNode_BrowseCatalog::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Browse", _HELP("Send a request to retrieve the catalog list.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."), "Platform",  _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("SortOrder", 0, _HELP("Sorting order of the items."), "SortOrder", _UICONFIG(FLOWGRAPH_CATSORT_UICONFIG)),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Requests the catalog in the specific sorting order. Wait for OnCatalogRecieved events after requesting the catalog.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_BrowseCatalog::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Browse))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			ECatalogSortOrder sortOrder = UIConfigToCatalogSortOrder(GetPortInt(pActInfo, eIn_SortOrder));
			if (!pService->BrowseCatalog(sortOrder))
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Request for the catalog failed.");
			}
		}
	}
}

void CFlowNode_GetLicenses::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Get", _HELP("Gets the licenses.")),
		InputPortConfig_Void("Next", _HELP("Gets the next license if any.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to mark against."),"Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		{ 0 }
	};

	static const SOutputPortConfig pOutConfig[] =
	{
		OutputPortConfig<int>("Remaining", _HELP("Number of licenses left to iterate through.")),
		OutputPortConfig<string>("ApplicationId", _HELP("The application license as a string.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Gets the list of application licenses this user owns (eg, DLCs).");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = pOutConfig;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_GetLicenses::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (IsPortActive(pActInfo, eIn_Get))
			{
				m_licenses.clear();
				m_curIndex = 0;
				auto licenses = pService->GetLicenses();

				for (auto& license : licenses)
				{
					string licenseStr;
					if (license.GetAsString(licenseStr))
					{
						m_licenses.emplace_back(licenseStr);
					}
				}

				ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_licenses.size()));
			}
			else if (IsPortActive(pActInfo, eIn_Next))
			{
				if (m_licenses.size() > m_curIndex)
				{
					ActivateOutput(pActInfo, eOut_ApplicationId, m_licenses.at(m_curIndex));
					++m_curIndex;
				}

				ActivateOutput(pActInfo, eOut_Remaining, static_cast<int>(m_licenses.size()) - m_curIndex);
			}
		}
	}
}

void QueryCatalogItemDetails::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Query", _HELP("Query the backend for details on the given items.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to query."), "Platform", _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("SystemContainerId", -1, _HELP("The Id of the System:Container holding the list of Catalog Item Ids."),  "SystemContainerId"),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP(
		"Query the backend for details on each item in the given container. "
		"Listen for OnCatalogItemsReceived event for changes."
	);

	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void QueryCatalogItemDetails::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate)
	{
		if (IsPortActive(pActInfo, eIn_Query))
		{
			const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

			if (IService* pService = Helper::GetService(platform))
			{
				const int containerId = crymath::clamp(GetPortInt(pActInfo, eIn_SystemContainerId), -1, std::numeric_limits<int>::max());
				if (containerId > -1)
				{
					if (IFlowSystemContainerPtr pContainer = gEnv->pFlowSystem->GetContainer(containerId))
					{
						DynArray<ApplicationIdentifier> items;
						for (int i = 0; i < pContainer->GetItemCount(); i++)
						{
							string elem;
							if (!pContainer->GetItem(i).GetValueWithConversion<string>(elem))
							{
								CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Container '%d' type mis-match.", containerId);
								break;
							}

							ApplicationIdentifier applicationId;
							pService->SetIdentifierFromString(applicationId, elem);
							items.push_back(applicationId);
						}

						pService->QueryCatalogItemDetails(items);
					}
					else
					{
						CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Container '%d' does not exist.", containerId);
					}
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Invalid container provided '%d'.", containerId);
				}
			}
		}
	}
}

REGISTER_FLOW_NODE("GamePlatform:Catalog:BrowseCatalog", CFlowNode_BrowseCatalog);
REGISTER_FLOW_NODE("GamePlatform:Catalog:GetLicenses", CFlowNode_GetLicenses);
REGISTER_FLOW_NODE("GamePlatform:Catalog:QueryCatalogItemDetails", QueryCatalogItemDetails);

} // Cry::GamePlatform
