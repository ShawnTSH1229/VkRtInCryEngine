// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/IObject.h>
#include <CrySchematyc/Utils/Scratchpad.h>
#include <queue>

namespace Cry::Scaleform
{

class CFlashUIComponent : public IEntityComponent, public IUIElementEventListener
{

	struct SQueuedObjectSignal
	{
		inline SQueuedObjectSignal() {}

		inline SQueuedObjectSignal(const SQueuedObjectSignal& rhs)
			: signal(rhs.signal.typeGUID, rhs.signal.senderGUID)
		{
			auto visitInput = [this](const Schematyc::CUniqueId& id, const Schematyc::CAnyConstRef& value)
			{
				const uint32 pos = scratchpad.Add(value);
				const Schematyc::CAnyPtr pValue = scratchpad.Get(pos);
				signal.params.BindInput(id, pValue);
			};
			rhs.signal.params.VisitInputs(visitInput);

			auto visitOutput = [this](const Schematyc::CUniqueId& id, const Schematyc::CAnyConstRef& value)
			{
				const uint32 pos = scratchpad.Add(value);
				const Schematyc::CAnyPtr pValue = scratchpad.Get(pos);
				signal.params.BindOutput(id, pValue);
			};
			rhs.signal.params.VisitOutputs(visitOutput);
		}

		Schematyc::SObjectSignal   signal;
		Schematyc::StackScratchpad scratchpad;
	};

public:

	static void ReflectType(Schematyc::CTypeDesc<CFlashUIComponent>& desc)
	{
		// The type description is filled dynamically
		// GUID, TypeName, Label, EditorCategory and Component Flags will be overwritten by generated type description
	}

	CFlashUIComponent() = default;
	virtual ~CFlashUIComponent() override;

	// IEntityComponent
	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual Entity::EventFlags GetEventMask() const override { return EEntityEvent::Update; }
	// ~IEntityComponent

protected:

	// IUIElementEventListener
	virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	virtual void OnInstanceCreated(IUIElement* pSender, IUIElement* pNewInstance) override;
	virtual void OnInstanceDestroyed(IUIElement* pSender, IUIElement* pDeletedInstance) override;
	// ~IUIElementEventListener

	template <typename VALUE_TYPE>
	void BindSignalInput(SQueuedObjectSignal& queuedSignal, const Schematyc::CClassDesc& typeDesc, const uint32 paramIdx, const VALUE_TYPE& value)
	{
		CRY_ASSERT(paramIdx < typeDesc.GetMembers().size(), "Invalid parameter index");

		if (paramIdx < typeDesc.GetMembers().size())
		{
			const Schematyc::CUniqueId uid = Schematyc::CUniqueId::FromUInt32(typeDesc.GetMembers().at(paramIdx).GetId());
			const Schematyc::CAnyConstPtr pValue = queuedSignal.scratchpad.Get(queuedSignal.scratchpad.Add(value));
			queuedSignal.signal.params.BindInput(uid, pValue);
		}
	}

private:

	std::queue<SQueuedObjectSignal> m_signalQueue;

};

} // Cry::Scaleform