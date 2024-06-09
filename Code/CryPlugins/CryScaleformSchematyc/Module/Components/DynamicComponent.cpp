// Copyright 2021-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "DynamicComponent.h"

#include <Plugin.h>
#include <Env/DynamicRegistry.h>
#include <Env/FlashRegistry.h>
#include <Env/Elements/EnvDynamicSignal.h>
#include <Env/Utils/Utils.h>
#include <Env/Utils/FlashUtils.h>

namespace Cry::Scaleform
{

CFlashUIComponent::~CFlashUIComponent()
{
	auto& pComponentRegistry = CFlashRegistry::GetInstance();
	pComponentRegistry.UnregisterListener(GetClassDesc().GetGUID(), this);

	if (m_pEntity->GetSimulationMode() == EEntitySimulationMode::Preview)
	{
		CPlugin_CryScaleformSchematyc::GetInstance()->GetFlashModule()->AdjustPreviewInstances(-1);
	}
}

void CFlashUIComponent::Initialize()
{
	auto& pComponentRegistry = CFlashRegistry::GetInstance();
	pComponentRegistry.RegisterListener(GetClassDesc().GetGUID(), this);
		
	if (m_pEntity->GetSimulationMode() == EEntitySimulationMode::Preview)
	{
		CPlugin_CryScaleformSchematyc::GetInstance()->GetFlashModule()->AdjustPreviewInstances(1);
	}
}

void CFlashUIComponent::ProcessEvent(const SEntityEvent& event)
{
	if (event.event == EEntityEvent::Update)
	{
		if (Schematyc::IObject* pObject = m_pEntity->GetSchematycObject())
		{
			while (!m_signalQueue.empty())
			{
				SQueuedObjectSignal queuedSignal = m_signalQueue.front();
				m_signalQueue.pop();

				pObject->ProcessSignal(queuedSignal.signal);
			}
		}
		else
		{
			std::queue<SQueuedObjectSignal> emptyQueue;
			m_signalQueue.swap(emptyQueue);
		}
	}
}

void CFlashUIComponent::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
{
	// Get GUID from element name
	string name;
	name.Format("%s::%s", pSender->GetName(), event.sName);
	auto typeName = CTypeNameWithString(std::move(name));
	const CryGUID guid = typeName.ToGUID(EnvDynamicSignal::GUIDHigher);

	auto& dynTypeRegistry = CDynamicTypeDescRegistry::GetInstance();
	const Schematyc::CClassDesc& typeDesc = *(dynTypeRegistry.GetTypeDesc(guid).get());

	if (typeDesc.GetGUID().IsNull())
		return;

	SQueuedObjectSignal queuedSignal;
	queuedSignal.scratchpad.Reserve(typeDesc.GetSize());
	queuedSignal.signal.typeGUID = typeDesc.GetGUID();
	queuedSignal.signal.senderGUID = GetGUID();

	// Instance Id parameter is the first parameter, we handle this before we handle the flash event parameters
	{
		const uint32 instanceId = pSender->GetInstanceID();
		BindSignalInput(queuedSignal, typeDesc, 0, instanceId);
	}
	
	// Copy arguments to signal buffer
	const auto& members = typeDesc.GetMembers();
	for (uint32 i = 1; i < members.size(); i++)
	{
		const TUIData& uiData = args.GetArg(i - 1);

		const EDynamicType memberType = GetDynTypeByGUID(members.at(i).GetTypeDesc().GetGUID());
		switch (memberType)
		{
		case EDynamicType::Boolean:
		{
			bool bValue;
			uiData.GetValueWithConversion(bValue);
			BindSignalInput(queuedSignal, typeDesc, i, bValue);
		} break;
	
		case EDynamicType::Float:
		{
			float fValue;
			uiData.GetValueWithConversion(fValue);
			BindSignalInput(queuedSignal, typeDesc, i, fValue);
		} break;
	
		case EDynamicType::Integer:
		{
			int32 nValue;
			uiData.GetValueWithConversion(nValue);
			BindSignalInput(queuedSignal, typeDesc, i, nValue);
		} break;
	
		case EDynamicType::String:
		{
			string value;
			uiData.GetValueWithConversion(value);
			BindSignalInput(queuedSignal, typeDesc, i, Schematyc::CSharedString(value.c_str()));
		} break;
		}
	}

	m_signalQueue.emplace(std::move(queuedSignal));
}

void CFlashUIComponent::OnInstanceCreated(IUIElement* pSender, IUIElement* pNewInstance)
{
	auto& componentRegistry = CFlashRegistry::GetInstance();
	componentRegistry.RegisterElement(GetClassDesc().GetGUID(), pNewInstance);
	pNewInstance->GetFlashPlayer()->Advance(0);
}

void CFlashUIComponent::OnInstanceDestroyed(IUIElement* pSender, IUIElement* pDeletedInstance)
{
	auto& componentRegistry = CFlashRegistry::GetInstance();
	componentRegistry.UnregisterElement(GetClassDesc().GetGUID(), pDeletedInstance);
}

} // Cry::Scaleform