// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "DynamicFunctions.h"

#include <Env/Utils/FlashUtils.h>
#include <Env/Elements/EnvDynamicFunction.h>
#include <CryEntitySystem/IEntitySystem.h>

namespace Cry::Scaleform
{

static inline void ExecuteScaleformFunction(IUIElement* pElement, const char* szFuncName, const Schematyc::EnvFunctionUtilsEx::SBinding& binding, Schematyc::CRuntimeParamMap& params)
{
	CRY_ASSERT(pElement && szFuncName, "Element or Function Name was nullptr.");

	// Setup the Flash UI arguments from the pin data provided in the CRuntimeParamMap
	uint32 idx = 0;
	const uint32 instanceId = Schematyc::EnvFunctionUtilsEx::ReadParam<uint32>(binding, idx++, params);

	VERIFY_INSTANCE_EXISTS(pElement, instanceId);

	IUIElement* pInstance = pElement->GetInstance(instanceId);

	SUIArguments args;
	params.VisitInputs([&binding, &args, &params, &idx](const Schematyc::CUniqueId& id, const Schematyc::CAnyConstRef value)
		{
			switch (GetDynTypeByGUID(value.GetTypeDesc().GetGUID()))
			{

			case EDynamicType::Boolean:
			{
				args.AddArgument(Schematyc::EnvFunctionUtilsEx::ReadParam<bool>(binding, idx++, params));
			} break;

			case EDynamicType::Float:
			{
				args.AddArgument(Schematyc::EnvFunctionUtilsEx::ReadParam<float>(binding, idx++, params));
			} break;

			case EDynamicType::Integer:
			{
				args.AddArgument(Schematyc::EnvFunctionUtilsEx::ReadParam<int32>(binding, idx++, params));
			} break;

			case EDynamicType::String:
			{
				args.AddArgument(Schematyc::EnvFunctionUtilsEx::ReadParam<Schematyc::CSharedString>(binding, idx++, params).c_str());
			} break;

			}
		});

	// Call the UI Function and store the returned result in a shared pointer so we can pass it to other nodes
	TUIData res;
	SFlashReturnContext context;
	if (pInstance->IsValid())
	{
		if (pInstance->CallFunction(szFuncName, args, &res))
		{
			context.success = true;
			context.value = ConvertDataToAny(res);
		}
	}

	params.SetOutput(Schematyc::CUniqueId::FromUInt32('ctx'), context);
}

void RegisterDynamicFunctions(Schematyc::CEnvRegistrationScope& scope, IUIElement* pElement, const Schematyc::CCommonTypeDesc* pComponentDesc)
{
	CRY_ASSERT(pElement && pComponentDesc, "Element or Component Desc was nullptr.");

	// Here we loop every function description from this element and create dynamic Schematyc function descriptions to match
	uint32 descIndex = 0;
	while (const SUIEventDesc* pFunctionDesc = pElement->GetFunctionDesc(descIndex++))
	{
		// Get a unique GUID for this function
		string name;
		name.Format("%s::%s", pElement->GetName(), pFunctionDesc->sName);
		const CryGUID guid = CTypeNameWithString(Schematyc::EnvDynamicFunction::TypeString(name)).ToGUID(Schematyc::EnvDynamicFunction::GUIDHigher);
		
		string functionName;
		functionName.Format("%s::%s", "Dynamic", pFunctionDesc->sDisplayName);

		auto pFunction = Schematyc::EnvDynamicFunction::MakeSharedWithStub(guid, functionName, SCHEMATYC_SOURCE_FILE_INFO, pComponentDesc,
			[=](const Schematyc::EnvFunctionUtilsEx::SBinding& binding, Schematyc::CRuntimeParamMap& params, void* pObject, void* pUserData)
			{
				ExecuteScaleformFunction(pElement, pFunctionDesc->sDisplayName, binding, params);
			}
		);
		
		// Bind parameters
		uint32 paramIndex = 0;
		pFunction->BindInput(paramIndex++, 'iid', "InstanceId", "Element Instance Id.", uint32(0));
		
		// We go through every described parameter here for the function
		for (const SUIParameterDesc& inputParam : pFunctionDesc->InputParams.Params)
		{
			EDynamicType paramType = GetDynTypeFromFlash(inputParam.eType);
			if (paramType == EDynamicType::Any)
			{
				paramType = EDynamicType::String;
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Any types are not supported in ScaleformSchematyc plugin. '%s' parameter treated as String.", inputParam.sName);
			}

			pFunction->BindInput(
				paramType,
				paramIndex, paramIndex, inputParam.sDisplayName, inputParam.sDesc);
			++paramIndex;
		}

		// We use a custom data type here so we don't waste any parameter pins that could be used by the function itself
		pFunction->BindOutput<SFlashReturnContext>(paramIndex, 'ctx', "Return Context", "Return Value Context Data. Use 'ExtractReturnValue' node to get the value.");

		scope.Register(pFunction);
	}
}

} // Cry::Scaleform