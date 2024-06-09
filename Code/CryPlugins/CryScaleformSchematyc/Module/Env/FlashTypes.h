// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySchematyc/Utils/Any.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySystem/Scaleform/IFlashUI.h>

namespace Cry::Scaleform
{

// Types used when defining parameters for functions and events
enum class EFlashParamType
{
	Boolean,
	Float,
	Integer,
	String,

	Any
};

// Types used in return data of UI Function calls
enum class EFlashReturnType
{
	Boolean,
	Number,
	String,

	Any
};

// Return context data type that can be passed to an extract node
struct SFlashReturnContext
{
	bool success = false;
	Schematyc::CAnyValuePtr value = nullptr;
};

inline void ParamTypeToString(Schematyc::IString& output, const EFlashParamType& input)
{
	switch (input)
	{
	case EFlashParamType::Boolean: output.assign("Boolean"); break;
	case EFlashParamType::Integer: output.assign("Integer"); break;
	case EFlashParamType::Float: output.assign("Float"); break;
	case EFlashParamType::String: output.assign("String"); break;
	default: output.assign("Any/Unknown"); break;
	}
}

inline void ReturnTypeToString(Schematyc::IString& output, const EFlashReturnType& input)
{
	switch (input)
	{
	case EFlashReturnType::Boolean: output.assign("Boolean"); break;
	case EFlashReturnType::Number: output.assign("Number"); break;
	case EFlashReturnType::String: output.assign("String"); break;
	default: output.assign("Any/Unknown"); break;
	}
}

inline void ReflectType(Schematyc::CTypeDesc<EFlashParamType>& desc)
{
	desc.SetGUID("{75D0B3B6-D0F0-4022-AB3A-B98E0C51D42B}"_cry_guid);
	desc.SetLabel("Flash Parameter Type");
	desc.SetDescription("Type of data of a flash event or function parameter");
	desc.SetDefaultValue(EFlashParamType::Any);
	desc.AddConstant(EFlashParamType::Boolean, "Boolean", "Boolean");
	desc.AddConstant(EFlashParamType::Float, "Float", "Float");
	desc.AddConstant(EFlashParamType::Integer, "Integer", "Integer");
	desc.AddConstant(EFlashParamType::String, "String", "String");
	desc.SetToStringOperator<&ParamTypeToString>();
}

inline void ReflectType(Schematyc::CTypeDesc<EFlashReturnType>& desc)
{
	desc.SetGUID("{B1C039BA-8640-4617-946E-F3518C9C0559}"_cry_guid);
	desc.SetLabel("Flash Data Type");
	desc.SetDescription("Type of data of a Flash variable");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(EFlashReturnType::Any);
	desc.AddConstant(EFlashReturnType::Boolean, "Boolean", "Boolean");
	desc.AddConstant(EFlashReturnType::Number, "Number", "Number");
	desc.AddConstant(EFlashReturnType::String, "String", "String");
	desc.SetToStringOperator<&ReturnTypeToString>();
}

inline void ReflectType(Schematyc::CTypeDesc<SFlashReturnContext>& desc)
{
	desc.SetGUID("{93BA04BB-F82A-4F5F-AD16-FC62F99983C0}"_cry_guid);
	desc.SetLabel("UINode Return Context");
}

} // Cry::Scaleform

inline void ReflectType(Schematyc::CTypeDesc<IUIElement::SUIConstraints::EPositionType>& desc)
{
	desc.SetGUID("{11C57799-86C6-4D59-8752-E41FFD98E5D7}"_cry_guid);
	desc.SetLabel("Element Position Type");
	desc.SetDescription("Type of positioning of the element");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(IUIElement::SUIConstraints::EPositionType::ePT_Dynamic);
	desc.AddConstant(IUIElement::SUIConstraints::EPositionType::ePT_Dynamic, "Dynamic", "Dynamic");
	desc.AddConstant(IUIElement::SUIConstraints::EPositionType::ePT_Fixed, "Fixed", "Fixed");
	desc.AddConstant(IUIElement::SUIConstraints::EPositionType::ePT_FixedDynTexSize, "FixedTextureSize", "Fixed Texture Size");
	desc.AddConstant(IUIElement::SUIConstraints::EPositionType::ePT_Fullscreen, "Fullscreen", "Fullscreen");
}

inline void ReflectType(Schematyc::CTypeDesc<IUIElement::SUIConstraints::EPositionAlign>& desc)
{
	desc.SetGUID("{14640F70-DC80-4F60-82BA-B0B62E427A8C}"_cry_guid);
	desc.SetLabel("Element Position Alignment");
	desc.SetDescription("Alignment of the element position");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(IUIElement::SUIConstraints::EPositionAlign::ePA_Mid);
	desc.AddConstant(IUIElement::SUIConstraints::EPositionAlign::ePA_Lower, "Lower", "Lower");
	desc.AddConstant(IUIElement::SUIConstraints::EPositionAlign::ePA_Mid, "Mid", "Mid");
	desc.AddConstant(IUIElement::SUIConstraints::EPositionAlign::ePA_Upper, "Upper", "Upper");
}

inline void ReflectType(Schematyc::CTypeDesc<IUIElement::SUIConstraints>& desc)
{
	desc.SetGUID("{9A57CA10-B441-4A3D-B791-33D3C1139523}"_cry_guid);
	desc.SetLabel("Flash Element Constraints");
	desc.SetDescription("Constraints given to specific flash element instances");
	desc.SetDefaultValue(IUIElement::SUIConstraints());
	
	desc.AddMember(&IUIElement::SUIConstraints::eType, 'etyp', "PositionType", "Position Type", "", IUIElement::SUIConstraints::EPositionType::ePT_Dynamic);
	desc.AddMember(&IUIElement::SUIConstraints::iLeft, 'ilft', "Left", "Left", "", 0);
	desc.AddMember(&IUIElement::SUIConstraints::iTop, 'itop', "Top", "Top", "", 0);
	desc.AddMember(&IUIElement::SUIConstraints::iWidth, 'iwdt', "Width", "Width", "", 1024);
	desc.AddMember(&IUIElement::SUIConstraints::iHeight, 'ihet', "Height", "Height", "", 768);
	desc.AddMember(&IUIElement::SUIConstraints::eHAlign, 'haln', "HAlign", "Horizontal Alignment", "", IUIElement::SUIConstraints::EPositionAlign::ePA_Mid);
	desc.AddMember(&IUIElement::SUIConstraints::eVAlign, 'valn', "VAlign", "Vertical Alignment", "", IUIElement::SUIConstraints::EPositionAlign::ePA_Mid);
	desc.AddMember(&IUIElement::SUIConstraints::bScale, 'scal', "Scale", "Scale", "", true);
	desc.AddMember(&IUIElement::SUIConstraints::bMax, 'max', "Max", "Max", "", false);
}

inline void ReflectType(Schematyc::CTypeDesc<IUIElement::EFlashUIFlags>& desc)
{
	desc.SetGUID("{42772328-6F51-4ED6-8856-4F865BEE8594}"_cry_guid);
	desc.SetLabel("Flash element instance flags");
	desc.SetDescription("Flash element instance flags to control various options of the element instance");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(IUIElement::EFlashUIFlags::eFUI_CONSOLE_CURSOR);

	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_NOT_CHANGEABLE, "NotChangeable", "Not Changeable");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_HARDWARECURSOR, "HardwareCursor", "Hardware Cursor");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_MOUSEEVENTS, "MouseEvents", "Mouse Events");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_KEYEVENTS, "KeyEvents", "Key Events");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_CONSOLE_MOUSE, "ConsoleMouse", "Console Mouse");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_CONSOLE_CURSOR, "ConsoleCursor", "Console Cursor");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_CONTROLLER_INPUT, "ControllerInput", "Controller Input");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_EVENTS_EXCLUSIVE, "EventsExclusive", "Events Exclusive");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_RENDER_LOCKLESS, "RenderLockless", "Render Lockless");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_FIXED_PROJ_DEPTH, "FixedProjDepth", "Fixed Projection Depth");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_IS_HUD, "IsHUD", "Is HUD");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_SHARED_RT, "SharedRT", "Shared RT");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_LAZY_UPDATE, "LazyUpdate", "Lazy Update");
	desc.AddConstant(IUIElement::EFlashUIFlags::eFUI_NO_AUTO_UPDATE, "NoAutoUpdate", "No Auto Update");
}