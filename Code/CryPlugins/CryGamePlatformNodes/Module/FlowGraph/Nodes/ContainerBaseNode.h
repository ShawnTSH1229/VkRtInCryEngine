// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryFlowGraph/IFlowBaseNode.h>
#include <CrySchematyc/Utils/Array.h>

// Containers are unique and persistent, similar to Flow Graph system containers.
// Containers (lists) must be deleted/removed manually. There is no automatic cleanup except when unloading the module itself.

namespace Cry::GamePlatform
{

template<class TElement>
class CFlowContainerBaseNode : public CFlowBaseNode<eNCT_Instanced>
{

protected:

	using Base = CFlowContainerBaseNode<TElement>;

	enum EBaseInputs
	{
		eIn_ContainerId,
		eIn_DeleteContainer,
		eIn_NextElement,
		eIn_GetCount,

		eIn_CustomInput
	};

	enum EBaseOutputs
	{
		eOut_Count,
		eOut_Remaining,

		eOut_CustomOutput
	};

public:

	CFlowContainerBaseNode(SActivationInfo*)
		: m_containerId(0)
		, m_curIndex(-1) 
	{}
	~CFlowContainerBaseNode() {}

	static inline bool ContainerExists(int id)
	{
		return s_lists.count(id) > 0;
	}

	static inline int AddList(const DynArray<TElement>& list)
	{
		int i = 0;
		while (s_lists.count(i))
			++i;

		s_lists[i] = list;
		return i;
	}

	static inline void RemoveList(int id)
	{
		if (s_lists.count(id) != 0)
		{
			s_lists.erase(id);
		}
	}

	static inline TElement* GetListElement(int id, int index)
	{
		if (s_lists.count(id) > 0 && s_lists[id].size() > index)
			return &s_lists[id].at(index);

		return nullptr;
	}

	static inline int GetListSize(int id)
	{
		if (s_lists.count(id) > 0)
			return s_lists[id].size();

		return 0;
	}

	static inline void ClearAllLists()
	{
		s_lists.clear();
	}

protected:

	static inline void GetConfigurationBase(std::vector<SInputPortConfig>& inputs, std::vector<SOutputPortConfig>& outputs)
	{
		CRY_ASSERT(inputs.size() == 0 && outputs.size() == 0, "Expected empty parameters. Inputs & Outputs may be duplicated for this node, or in an incorrect order.");

		inputs.push_back(InputPortConfig<int>("ContainerId", 0, _HELP("Container Id to interact with, when changed will output the first element.")));
		inputs.push_back(InputPortConfig_Void("DeleteContainer", _HELP("Deletes the container matching the given container id.")));
		inputs.push_back(InputPortConfig_Void("NextElement", _HELP("Gets the next element in the container (wraps to first element after the last element). If there are no elements to retrieve no outputs will fire.")));
		inputs.push_back(InputPortConfig_Void("GetCount", _HELP("Gets the number of elements the container is holding.")));

		outputs.push_back(OutputPortConfig<int>("ElementCount", _HELP("Total number of elements in this container.")));
		outputs.push_back(OutputPortConfig<int>("ElementRemaining", _HELP("Number of elements left to iterate over.")));
	}

	static inline void ProcessEventBase(Base* pBase, EFlowEvent event, SActivationInfo* pActInfo)
	{
		CRY_ASSERT(pBase != nullptr, "No parent provided");

		if (event == eFE_Initialize)
		{
			Base::ClearAllLists();
		}
		else if (event == eFE_Activate)
		{
			if (IsPortActive(pActInfo, eIn_ContainerId))
			{
				const int oldId = pBase->m_containerId;
				pBase->m_containerId = GetPortInt(pActInfo, eIn_ContainerId);

				if (pBase->m_containerId != oldId)
				{
					pBase->m_curIndex = -1;
				}
			}
			
			if (IsPortActive(pActInfo, eIn_DeleteContainer))
			{
				if (Base::ContainerExists(pBase->m_containerId))
				{
					Base::RemoveList(pBase->m_containerId);
				}
			}
			else if (IsPortActive(pActInfo, eIn_NextElement))
			{
				if (Base::ContainerExists(pBase->m_containerId))
				{
					if (++(pBase->m_curIndex) >= Base::GetListSize(pBase->m_containerId))
					{
						pBase->m_curIndex = 0;
					}
				}
			}
			else if (IsPortActive(pActInfo, eIn_GetCount))
			{
				ActivateOutput(pActInfo, eOut_Count, Base::GetListSize(pBase->m_containerId));
			}
		}
	}

protected:

	int m_containerId;
	int m_curIndex;

private:

	static std::map<int, DynArray<TElement>> s_lists;

};

template<class TElement>
std::map<int, DynArray<TElement>> CFlowContainerBaseNode<TElement>::s_lists = {};

} // Cry::GamePlatform
