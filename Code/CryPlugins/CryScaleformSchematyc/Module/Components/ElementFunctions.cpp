// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "ElementFunctions.h"

#include <Env/Utils/FlashUtils.h>
#include <Env/Elements/EnvDynamicFunction.h>
#include <CryEntitySystem/IEntitySystem.h>

namespace Cry::Scaleform
{

inline const CryGUID CreateElementGUID(const char * elementName, const char * funcName)
{
	string fullName;
	fullName.Format("%s::%s", elementName, funcName);
	return CTypeNameWithString(fullName.c_str()).ToGUID(Schematyc::EnvDynamicFunction::GUIDHigher);
}

void RegisterElementFunctions(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const Schematyc::CCommonTypeDesc* pComponentDesc)
{
	#define SCHEMATYC_MAKE_ELEMENT_FUNCTION(functionPtr, functionName, sourceinfo) \
			Schematyc::EnvDynamicFunction::MakeShared(functionPtr, CreateElementGUID(pElement->GetName(), functionName), functionName, sourceinfo, pComponentDesc, pElement);

	CRY_ASSERT(pElement != nullptr);
	if (pElement == nullptr)
	{
		return;
	}

	// Define functions
	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::NewInstance, "New Instance", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindOutput(0, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(1, 'aiff', "Auto-Init", "Automatically initialize the first frame of the element instance. If your ActionScript functions are stored in the first key frame, make sure this is checked to be able to use them immediately.", bool(false));
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::DestroyInstance, "Destroy Instance", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::SetVisibility, "Set Visibility", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'vis', "Visible", "Visible", true);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::Advance, "Advance", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'ft', "Frametime", "Frame time to advance the element by", float(0.f));
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::GetAlpha, "Get Alpha", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindOutput(0, 'alph', "Alpha", "Alpha of the element background", 0.f);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::SetAlpha, "Set Alpha", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'alph', "Alpha", "Alpha of the element background", 0.f);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::GetConstraints, "Get Constraints", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindOutput(0, 'cstr', "Constraints", "Positioning and Alignment constraints of the specific element instance", IUIElement::SUIConstraints());
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::SetConstraints, "Set Constraints", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'cstr', "Constraints", "Positioning and Alignment constraints of the specific element instance", IUIElement::SUIConstraints());
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::ExpandConstraints, "Expand Constraints", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'cstr', "Constraints", "Element Constraints", IUIElement::SUIConstraints());
		pFunction->BindOutput(0, 'post', "PositionType", "Position Type", IUIElement::SUIConstraints::EPositionType::ePT_Dynamic);
		pFunction->BindOutput(2, 'ilft', "Left", "Left", 0);
		pFunction->BindOutput(3, 'itop', "Top", "Top", 0);
		pFunction->BindOutput(4, 'wdth', "Width", "Width", 1024);
		pFunction->BindOutput(5, 'heit', "Height", "Height", 768);
		pFunction->BindOutput(6, 'hpa', "HAlign", "Horizontal Alignment", IUIElement::SUIConstraints::EPositionAlign::ePA_Mid);
		pFunction->BindOutput(7, 'vpa', "VAlign", "Vertical Alignment", IUIElement::SUIConstraints::EPositionAlign::ePA_Mid);
		pFunction->BindOutput(8, 'scal', "Scale", "Scale", true);
		pFunction->BindOutput(9, 'max', "Max", "Max", true);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::PerFrameRender, "Per Frame Render", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'eid', "Entity", "The Entity to apply this to", Schematyc::ExplicitEntityId(Schematyc::ExplicitEntityId::Invalid));
		pFunction->BindInput(2, 'enab', "Enable", "Whether to enable the Per Frame Rendering option for Dynamic Textures for the given entity", bool(true));
		pFunction->BindInput(3, 'eslt', "Entity Slot", "If both Entity Slot and Texture Slot is defined (0 or greater), this node will only enable PFR for that specific dynamic texture", -1);
		pFunction->BindInput(4, 'tslt', "Texture Slot", "If both Entity Slot and Texture Slot is defined (0 or greater), this node will only enable PFR for that specific dynamic texture", -1);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::ExpandContext, "Expand Context", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->SetDescription("Accessor for the return context of a UI function call");
		pFunction->BindOutput(0, 'succ', "Success", "True if the function call was successful.", false);
		pFunction->BindInput(1, 'ctx', "Context", "Return context from an UI event or function call.", SFlashReturnContext());
		pFunction->BindOutput(2, 'rtyp', "ReturnType", "Data Type of the return value.", EFlashReturnType::Any);
		pFunction->BindOutput(3, 'bool', "Boolean", "The return data if boolean.", false);
		pFunction->BindOutput(4, 'flt', "Number", "The return data if floating point.", 0.f);
		pFunction->BindOutput(5, 'str', "String", "The return data if string.", Schematyc::CSharedString(""));
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::GetInstanceFlag, "Get Instance Flag", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'flag', "Flag", "Flag", IUIElement::EFlashUIFlags::eFUI_CONSOLE_CURSOR);
		pFunction->BindOutput(0, 'val', "Value", "True if the flag was set. False otherwise", false);
		scope.Register(pFunction);
	}

	{
		auto pFunction = SCHEMATYC_MAKE_ELEMENT_FUNCTION(&FlashFunctions::SetInstanceFlag, "Set Instance Flag", SCHEMATYC_SOURCE_FILE_INFO);
		pFunction->BindInput(1, 'iid', "InstanceId", "Instance Id", uint32(0));
		pFunction->BindInput(2, 'flag', "Flag", "Flag", IUIElement::EFlashUIFlags::eFUI_CONSOLE_CURSOR);
		pFunction->BindInput(3, 'val', "Value", "Value", false);
		scope.Register(pFunction);
	}
}

namespace FlashFunctions
{

inline uint32 NewInstance(void* pUserData, const bool& autoInit)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	uint32 newId = FindUnusedInstanceId(pElement);

	// This creates an instance if the instance doesn't exist.
	IUIElement* pInstance = pElement->GetInstance(newId);
				
	if (autoInit)
	{
		pInstance->GetFlashPlayer()->Advance(0);
	}

	return newId;
}

inline void DestroyInstance(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);

	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->DestroyThis();
	}
}

inline bool GetVisibility(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->IsVisible();
	}

	return false;
}

inline void SetVisibility(void* pUserData, const uint32& instanceId, const bool& visible)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->SetVisible(visible);
	}
}

inline int GetLayer(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->GetLayer();
	}

	return 0;
}

inline void SetLayer(void* pUserData, const uint32& instanceId, const int& layer)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->SetLayer(layer);
	}
}

inline float GetAlpha(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->GetAlpha();
	}

	return 0.f;
}

inline void SetAlpha(void* pUserData, const uint32& instanceId, const float& alpha)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->SetAlpha(alpha);
	}
}

inline IUIElement::SUIConstraints GetConstraints(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->GetConstraints();
	}

	return IUIElement::SUIConstraints();
}

inline void SetConstraints(void* pUserData, const uint32& instanceId, const IUIElement::SUIConstraints& constraints)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->SetConstraints(constraints);
	}
}

inline IUIElement::SUIConstraints::EPositionType ExpandConstraints(void* pUserData, const IUIElement::SUIConstraints& constraints, int& left, int& top, int& width, int& height, IUIElement::SUIConstraints::EPositionAlign& horizontalAlign, IUIElement::SUIConstraints::EPositionAlign& verticalAlign, bool& scaleMode, bool& max)
{
	left = constraints.iLeft;
	top = constraints.iTop;
	width = constraints.iWidth;
	height = constraints.iHeight;
	horizontalAlign = constraints.eHAlign;
	verticalAlign = constraints.eVAlign;
	scaleMode = constraints.bScale;
	max = constraints.bMax;
	return constraints.eType;
}

inline void SetInstanceFlag(void* pUserData, const uint32 instanceId, IUIElement::EFlashUIFlags flag, bool value)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);

	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->SetFlag(flag, value);
	}
}

inline bool GetInstanceFlag(void* pUserData, const uint32 instanceId, IUIElement::EFlashUIFlags flag)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);

	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->HasFlag(flag);
	}

	return false;
}

inline bool IsValid(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->IsValid();
	}

	return false;
}

inline bool IsHiding(void* pUserData, const uint32& instanceId)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		return pInstance->IsHiding();
	}

	return false;
}

inline void Advance(void* pUserData, const uint32& instanceId, const float& frameTime)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->GetFlashPlayer()->Advance(frameTime);
	}
}

inline void ScreenToFlash(void* pUserData, const uint32& instanceId, float& px, float& py, const bool& stageScaleMode)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		pInstance->ScreenToFlash(px, py, px, py, stageScaleMode);
	}
}

inline void WorldToFlash(void* pUserData, const uint32& instanceId, Vec3& pos, Vec3& offset, const bool& stageScaleMode)
{
	IUIElement* pElement = static_cast<IUIElement*>(pUserData);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);
	
	if (IUIElement* pInstance = pElement->GetInstance(instanceId))
	{
		// get current camera matrix
		const CCamera& cam = GetISystem()->GetViewCamera();
		const Matrix34& camMat = cam.GetMatrix();

		// add offset to position
		const Vec3 vFaceingPos = camMat.GetTranslation() - camMat.GetColumn1() * 1000.f;
		const Vec3 vDir = (pos - vFaceingPos).GetNormalizedFast();
		const Vec3 vOffsetX = vDir.Cross(Vec3Constants<float>::fVec3_OneZ).GetNormalizedFast() * offset.x;
		const Vec3 vOffsetY = vDir * offset.y;
		const Vec3 vOffsetZ = Vec3(0, 0, offset.z);
		const Vec3 vOffset = vOffsetX + vOffsetY + vOffsetZ;
		pos += vOffset;

		Vec2 borders;
		float scale;
		Vec3 flashPos;
		pInstance->WorldToFlash(camMat, pos, flashPos, borders, scale, stageScaleMode);

		// return flashpos in pos
		pos = flashPos;

		// store overflow in offset x/y and scale in z
		offset.x = borders.x;
		offset.y = borders.y;
		offset.z = scale;
	}
}

inline void PerFrameRender(void* pUserData, const Schematyc::ExplicitEntityId& entityId, const int& entSlot, const int& texSlot, const bool& enable)
{
	if (IEntity* pEntity = gEnv->pEntitySystem->GetEntity(static_cast<EntityId>(entityId)))
	{
		if (entSlot >= 0 && texSlot >= 0)
		{
			if (IRenderNode* pRenderNode = pEntity->GetSlotRenderNode(entSlot))
			{
				if (IMaterial* pMaterial = pRenderNode->GetMaterial())
				{
					if (SEfResTexture* pTex = pMaterial->GetShaderItem().m_pShaderResources->GetTexture(texSlot))
					{
						if (IDynTextureSource* pDynTexSrc = pTex->m_Sampler.m_pDynTexSource)
						{
							pDynTexSrc->EnablePerFrameRendering(enable);
						}
					}
				}
			}
		}
		else
		{
			for (int iSlot = 0; iSlot < pEntity->GetSlotCount(); iSlot++)
			{
				if (IRenderNode* pRenderNode = pEntity->GetSlotRenderNode(iSlot))
				{
					if (IMaterial* pMaterial = pRenderNode->GetMaterial())
					{
						for (int iTex = 0; iTex < EFTT_MAX; iTex++)
						{
							if (SEfResTexture* pTex = pMaterial->GetShaderItem().m_pShaderResources->GetTexture(iTex))
							{
								if (IDynTextureSource* pDynTexSrc = pTex->m_Sampler.m_pDynTexSource)
								{
									pDynTexSrc->EnablePerFrameRendering(enable);
								}
							}
						}
					}
				}
			}
		}
	}
}

inline bool ExpandContext(void* pUserData, const SFlashReturnContext& context, EFlashReturnType& returnType, bool& boolean, float& flt, Schematyc::CSharedString& str)
{
	if (context.value)
	{
		returnType = GetReturnTypeByGUID(context.value->GetTypeDesc().GetGUID());

		switch (returnType)
		{
		case EFlashReturnType::Boolean:
		{
			boolean = *static_cast<const bool*>(context.value->GetValue());
		} break;

		case EFlashReturnType::Number:
		{
			flt = *static_cast<const float*>(context.value->GetValue());
		} break;

		case EFlashReturnType::String:
		{
			str = *static_cast<const Schematyc::CSharedString*>(context.value->GetValue());
		} break;
		}
	}

	return context.success;
}
} // namespace FlashFunctions

} // Cry::Scaleform