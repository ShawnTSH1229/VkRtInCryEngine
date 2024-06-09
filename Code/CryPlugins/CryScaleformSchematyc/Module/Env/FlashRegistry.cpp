// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "FlashRegistry.h"

#define COMPONENT_LISTENER_NAME "DynamicComponentListener"

namespace Cry::Scaleform
{

CFlashRegistry::~CFlashRegistry()
{
	m_records.clear();
}

CFlashRegistry& CFlashRegistry::GetInstance()
{
	static CFlashRegistry registry;
	return registry;
}

void CFlashRegistry::RegisterComponent(const CryGUID& guid)
{
	auto recordIterator = Find(guid);
	if (recordIterator == m_records.end())
	{
		m_records.emplace_back(guid);
	}
}

void CFlashRegistry::UnregisterComponent(const CryGUID& guid)
{
	auto recordIterator = Find(guid);
	if (recordIterator != m_records.end())
	{
		m_records.erase(recordIterator);
	}
}

void CFlashRegistry::RegisterListener(const CryGUID& guid, IUIElementEventListener* pListener)
{
	auto recordIterator = Find(guid);
	if (recordIterator != m_records.end())
	{
		auto listenerIterator = Find(recordIterator, pListener);
		if (listenerIterator == recordIterator->listeners.end())
		{
			for (auto pElement : recordIterator->elements)
			{
				pElement->AddEventListener(pListener, COMPONENT_LISTENER_NAME);
			}

			recordIterator->listeners.emplace(pListener);
		}
	}
}

void CFlashRegistry::UnregisterListener(const CryGUID& guid, IUIElementEventListener* pListener)
{
	auto recordIterator = Find(guid);
	if (recordIterator != m_records.end())
	{
		auto listenerIterator = Find(recordIterator, pListener);
		if (listenerIterator != recordIterator->listeners.end())
		{
			for (auto pElement : recordIterator->elements)
			{
				pElement->RemoveEventListener(pListener);
			}

			recordIterator->listeners.erase(listenerIterator);
		}
	}
}

void CFlashRegistry::RegisterElement(const CryGUID& guid, IUIElement* pElement)
{
	auto recordIterator = Find(guid);
	if (recordIterator != m_records.end())
	{
		auto elementIterator = Find(recordIterator, pElement);
		if (elementIterator == recordIterator->elements.end())
		{
			for (auto pListener : recordIterator->listeners)
			{
				pElement->AddEventListener(pListener, COMPONENT_LISTENER_NAME);
			}

			pElement->AddRef();
			recordIterator->elements.emplace(pElement);
		}
	}
}

void CFlashRegistry::UnregisterElement(const CryGUID& guid, IUIElement* pElement)
{
	auto recordIterator = Find(guid);
	if (recordIterator != m_records.end())
	{
		auto elementIterator = Find(recordIterator, pElement);
		if (elementIterator != recordIterator->elements.end())
		{
			for (auto pListener : recordIterator->listeners)
			{
				(*elementIterator)->RemoveEventListener(pListener);
			}

			(*elementIterator)->Release();
			recordIterator->elements.erase(elementIterator);
		}
	}
}

void CFlashRegistry::ClearInstances()
{
	for (auto& record : m_records)
	{
		for (auto it = record.elements.begin(); it != record.elements.end(); it++)
		{
			if ((*it)->GetInstanceID() != 0)
			{
				it = record.elements.erase(it);
			}

		}
	}
}

void CFlashRegistry::Reset()
{
	m_records.clear();
}

TRecords::iterator CFlashRegistry::Find(const CryGUID& guid)
{
	return std::find_if(m_records.begin(), m_records.end(), [&guid](const SComponentElementRecord& record){ return guid == record.guid; });
}

std::set<IUIElement*>::iterator CFlashRegistry::Find(TRecords::iterator recordIterator, IUIElement* pBaseElement)
{
	auto& elements = recordIterator->elements;
	return std::find_if(elements.begin(), elements.end(), [pBaseElement](const IUIElement* pElem){ return pElem == pBaseElement; });
}

std::set<IUIElementEventListener*>::iterator CFlashRegistry::Find(TRecords::iterator recordIterator, IUIElementEventListener* pListener)
{
	auto& listeners = recordIterator->listeners;
	return std::find_if(listeners.begin(), listeners.end(), [pListener](const IUIElementEventListener* pListenEntry){ return pListenEntry == pListener; });
}

} // Cry::Scaleform

#undef COMPONENT_LISTENER_NAME