// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	DynamicRegistry

	This system allows users to register type descriptions and refer to them 
	later. For example, with dynamic signals so that the signal data buffer 
	can be constructed when sending the signal.
*/
#pragma once

#include "EnvTypes.h"
#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/IObject.h>
#include <CrySystem/Scaleform/IFlashUI.h>

namespace Cry::Scaleform
{

// Allows us to store type descriptions that we can retrieve via GUID alone for dynamic functions and signals
class CDynamicTypeDescRegistry
{

	CDynamicTypeDescRegistry() = default;

public:

	CDynamicTypeDescRegistry(const CDynamicTypeDescRegistry& other) = delete;
	~CDynamicTypeDescRegistry();

	static CDynamicTypeDescRegistry& GetInstance();

	void RegisterTypeDesc(std::weak_ptr<Schematyc::CClassDesc> pTypeDesc);
	void UnregisterTypeDesc(const CryGUID& guid);
	bool IsTypeRegistered(const CryGUID& guid) const;
	const std::shared_ptr<Schematyc::CClassDesc>& GetTypeDesc(const CryGUID& guid) const;

	void Reset();

private:

	std::set<std::shared_ptr<Schematyc::CClassDesc>> m_registry;

};

} // Cry::Scaleform