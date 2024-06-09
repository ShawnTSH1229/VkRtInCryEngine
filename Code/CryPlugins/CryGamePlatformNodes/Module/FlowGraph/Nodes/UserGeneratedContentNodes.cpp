// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "UserGeneratedContentNodes.h"
#include <GamePlatformHelper.h>
#include <FlowGraph/PlatformConfigs.h>
#include <CryGamePlatform/Interface/IPlatformRemoteStorage.h>
#include <CryGamePlatform/Interface/IPlatformUserGeneratedContent.h>

#define FLOWGRAPH_UGC_UICONFIG   \
	"enum_int:"                  \
	"Community=0,"               \
	"Microtransaction=1,"        \
	"Collection=2,"              \
	"Art=3,"                     \
	"Video=4,"                   \
	"Screenshot=5,"              \
	"Game=6,"                    \
	"Software=7,"                \
	"Concept=8,"                 \
	"WebGuide=9,"                \
	"IntegratedGuide=10,"        \
	"Merch=11,"                  \
	"ControllerBinding=12,"      \
	"SteamworksAccessInvite=13," \
	"SteamVideo=14"

namespace Cry::GamePlatform
{

void CFlowNode_CreateContent::GetConfiguration(SFlowNodeConfig& nodeConfig)
{
	static const SInputPortConfig pInConfig[] =
	{
		InputPortConfig_Void("Create", _HELP("Create the content.")),
		InputPortConfig<int>("Platform", 0, _HELP("The platform to check against."),"Platform",    _UICONFIG(FLOWGRAPH_PLATFORM_UICONFIG)),
		InputPortConfig<int>("ContentType", 0, _HELP("Type of content to create."),  "ContentType", _UICONFIG(FLOWGRAPH_UGC_UICONFIG)),
		InputPortConfig<string>("Title", _HELP("Title of the content.")),
		InputPortConfig<string>("Description", _HELP("Description of the content.")),
		InputPortConfig<int>("Visibility", 0, _HELP("Visibility of the content."),  "Visibility",  _UICONFIG("enum_int:Public=0,FriendsOnly=1,Private=2")),
		InputPortConfig<string>("Tags", _HELP("List of tags separated by comma. Each tag length must not exceed 255 characters.")),
		InputPortConfig<string>("FolderPath", _HELP("The absolute path to a local folder containing the content for the item.")),
		InputPortConfig<string>("PreviewFilePath", _HELP("The absolute path to a local preview image file for the item. Suggested formats include JPG, PNG and GIF.")),
		{ 0 }
	};

	nodeConfig.sDescription = _HELP("Creates a new user generated content item on the specified platform.");
	nodeConfig.pInputPorts = pInConfig;
	nodeConfig.pOutputPorts = nullptr;
	nodeConfig.SetCategory(EFLN_ADVANCED);
}

void CFlowNode_CreateContent::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	if (event == eFE_Activate && IsPortActive(pActInfo, eIn_Create))
	{
		const EGamePlatform platform = UIConfigToPlatform(GetPortInt(pActInfo, eIn_Platform));

		if (IService* pService = Helper::GetService(platform))
		{
			if (auto pRemoteStorage = pService->GetRemoteStorage())
			{
				if (auto pUGCMan = pRemoteStorage->GetUserGeneratedContentManager())
				{
					auto contentType = IUserGeneratedContent::EType::Community;
					switch (GetPortInt(pActInfo, eIn_ContentType))
					{
					case 1: contentType = IUserGeneratedContent::EType::Microtransaction; break;
					case 2: contentType = IUserGeneratedContent::EType::Collection; break;
					case 3: contentType = IUserGeneratedContent::EType::Art; break;
					case 4: contentType = IUserGeneratedContent::EType::Video; break;
					case 5: contentType = IUserGeneratedContent::EType::Screenshot; break;
					case 6: contentType = IUserGeneratedContent::EType::Game; break;
					case 7: contentType = IUserGeneratedContent::EType::Software; break;
					case 8: contentType = IUserGeneratedContent::EType::Concept; break;
					case 9: contentType = IUserGeneratedContent::EType::WebGuide; break;
					case 10: contentType = IUserGeneratedContent::EType::IntegratedGuide; break;
					case 11: contentType = IUserGeneratedContent::EType::Merch; break;
					case 12: contentType = IUserGeneratedContent::EType::ControllerBinding; break;
					case 13: contentType = IUserGeneratedContent::EType::SteamworksAccessInvite; break;
					case 14: contentType = IUserGeneratedContent::EType::SteamVideo; break;
					}

					auto visibility = IUserGeneratedContent::EVisibility::Public;
					switch (GetPortInt(pActInfo, eIn_Visibility))
					{
					case 1: visibility = IUserGeneratedContent::EVisibility::FriendsOnly; break;
					case 2: visibility = IUserGeneratedContent::EVisibility::Private; break;
					}

					const DynArray<string> tags = ParseTagList(GetPortString(pActInfo, eIn_Tags));
					DynArray<const char*> tagList;
					tagList.reserve(tags.size());

					for (const string& tag : tags)
					{
						tagList.push_back(tag.c_str());
					}

					const string title = GetPortString(pActInfo, eIn_Title);
					const string description = GetPortString(pActInfo, eIn_Description);
					const string contentFolder = GetPortString(pActInfo, eIn_FolderPath);
					const string previewFile = GetPortString(pActInfo, eIn_PreviewPath);

					pUGCMan->CreateDirect(pService->GetApplicationIdentifier(), contentType, title.c_str(), description.c_str(), visibility, tagList.data(), tagList.size(), contentFolder.c_str(), previewFile.c_str());
				}
			}
		}
	}
}

DynArray<string> CFlowNode_CreateContent::ParseTagList(const string& tags) const
{
	DynArray<string> list;

	int pos = 0;
	string tag;
	while (!(tag = tags.Tokenize(",", pos)).empty())
	{
		list.push_back(std::move(tag));
	}

	return list;
}

REGISTER_FLOW_NODE("GamePlatform:UserGeneratedContent:CreateContent", CFlowNode_CreateContent);

} // Cry::GamePlatform
