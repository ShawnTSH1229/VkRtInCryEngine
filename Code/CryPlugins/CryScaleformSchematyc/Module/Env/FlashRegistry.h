// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	FlashRegistry

	This systems serves the purpose of handling the registration and 
	de-registration of UI Event listeners for dynamic flash components. 
*/
#pragma once

#include "EnvTypes.h"
#include <CrySystem/Scaleform/IFlashUI.h>

namespace Cry::Scaleform
{

struct SComponentElementRecord : NoCopy
{

	SComponentElementRecord(const CryGUID& guid) : guid(guid)
	{};

	~SComponentElementRecord()
	{
		// Cleanup and make sure to release any refs added when inserted into the record
		for (auto pListener : listeners)
		{
			for (auto pElement : elements)
			{
				pElement->RemoveEventListener(pListener);
			}
		}

		for (auto pElement : elements)
		{
			pElement->Release();
		}

		elements.clear();
	}

	// Movable
    SComponentElementRecord(SComponentElementRecord&& other) noexcept
	{
		Invalidate();

		guid = other.guid;
		listeners = std::move(other.listeners);
		elements = std::move(other.elements);
	}

    SComponentElementRecord& operator = (SComponentElementRecord&& other) noexcept
	{
		Invalidate();

		guid = other.guid;
		listeners = std::move(other.listeners);
		elements = std::move(other.elements);

		return *this;
	}

	CryGUID guid;
	std::set<IUIElementEventListener*> listeners;
	std::set<IUIElement*> elements;

private:

	// Since this object can only be moved into existing instances, we
	// need to correctly invalidate this object before the move
	void Invalidate()
	{
		for (auto pListener : listeners)
		{
			for (auto pElement : elements)
			{
				pElement->RemoveEventListener(pListener);
			}
		}
	}
};
	
using TRecords = std::vector<SComponentElementRecord>;

class CFlashRegistry
{
	CFlashRegistry() = default;

public:

	CFlashRegistry(const CFlashRegistry& other) = delete;
	~CFlashRegistry();

	static CFlashRegistry& GetInstance();

	void RegisterComponent(const CryGUID& guid);
	void UnregisterComponent(const CryGUID& guid);
	
	void RegisterListener(const CryGUID& guid, IUIElementEventListener* pListener);
	void UnregisterListener(const CryGUID& guid, IUIElementEventListener* pListener);

	void RegisterElement(const CryGUID& guid, IUIElement* pElement);
	void UnregisterElement(const CryGUID& guid, IUIElement* pElement);

	void ClearInstances();
	void Reset();

protected:

	TRecords::iterator Find(const CryGUID& guid);
	std::set<IUIElement*>::iterator Find(TRecords::iterator recordIterator, IUIElement* pBaseElement);
	std::set<IUIElementEventListener*>::iterator Find(TRecords::iterator recordIterator, IUIElementEventListener* pListener);

private:

	TRecords m_records;

};

} // Cry::Scaleform