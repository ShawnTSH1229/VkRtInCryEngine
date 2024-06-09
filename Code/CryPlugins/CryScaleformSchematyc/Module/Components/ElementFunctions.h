// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <Env/FlashTypes.h>

namespace Cry::Scaleform
{

void RegisterElementFunctions(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const Schematyc::CCommonTypeDesc* pComponentDesc);

namespace FlashFunctions
{

inline uint32 NewInstance(void* pUserData, const bool& autoInit);
inline void   DestroyInstance(void* pUserData, const uint32& instanceId);
inline bool   GetVisibility(void* pUserData, const uint32& instanceId);
inline void   SetVisibility(void* pUserData, const uint32& instanceId, const bool& visible);
inline int    GetLayer(void* pUserData, const uint32& instanceId);
inline void   SetLayer(void* pUserData, const uint32& instanceId, const int& layer);
inline float  GetAlpha(void* pUserData, const uint32& instanceId);
inline void   SetAlpha(void* pUserData, const uint32& instanceId, const float& alpha);

inline IUIElement::SUIConstraints GetConstraints(void* pUserData, const uint32& instanceId);
inline void   SetConstraints(void* pUserData, const uint32& instanceId, const IUIElement::SUIConstraints& constraints);
inline IUIElement::SUIConstraints::EPositionType ExpandConstraints(void* pUserData, const IUIElement::SUIConstraints& constraints, int& left, int& top, int& width, int& height, IUIElement::SUIConstraints::EPositionAlign& horizontalAlign, IUIElement::SUIConstraints::EPositionAlign& verticalAlign, bool& scaleMode, bool& max);

inline void   SetInstanceFlag(void* pUserData, const uint32 instanceId, IUIElement::EFlashUIFlags flag, bool value);
inline bool   GetInstanceFlag(void* pUserData, const uint32 instanceId, IUIElement::EFlashUIFlags flag);

inline bool   IsValid(void* pUserData, const uint32& instanceId);
inline bool   IsHiding(void* pUserData, const uint32& instanceId);
inline void   Advance(void* pUserData, const uint32& instanceId, const float& frameTime);
inline void   ScreenToFlash(void* pUserData, const uint32& instanceId, float& px, float& py, const bool& stageScaleMode);
inline void   WorldToFlash(void* pUserData, const uint32& instanceId, Vec3& pos, Vec3& offset, const bool& stageScaleMode);
inline void   PerFrameRender(void* pUserData, const Schematyc::ExplicitEntityId& entityId, const int& entSlot, const int& texSlot, const bool& enable);
inline bool   ExpandContext(void* pUserData, const SFlashReturnContext& context, EFlashReturnType& returnType, bool& boolean, float& flt, Schematyc::CSharedString& str);

} // namespace FlashFunctions

} // Cry::Scaleform