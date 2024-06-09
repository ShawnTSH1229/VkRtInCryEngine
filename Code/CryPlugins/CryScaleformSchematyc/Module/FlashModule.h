// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

/*	FlashModule

	The Flash UI Module handles registration and de-registration of 
	the dynamic Flash components and associated nodes. It is also 
	capable of receiving certain events from the UI system and 
	restricting the UI system itself from reloading elements that 
	would invalidate the registered components.
*/
#pragma once

#include <CrySystem/Scaleform/IFlashUI.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

namespace Cry::Scaleform
{

class CFlashModule final : public IUIModule, NoCopy
{

public:

	CFlashModule();
	virtual ~CFlashModule() override;

	void RegisterSchematyc() const;
	void UnregisterSchematyc() const;
	void SetAllowReload(const bool allow);
	void AdjustPreviewInstances(const int val);

	// IUIModule
	virtual void Shutdown() override;
	virtual bool EditorAllowReload() override;
	virtual void EditorReload() override;
	// ~IUIModule

private:

	static void RegisterFlashElements(Schematyc::IEnvRegistrar& registrar);
	static void RegisterDynamicComponents(Schematyc::CEnvRegistrationScope& packageScope);

	bool m_bAllowReload = false;
	int m_numPreviewInstances = 0;

};

} // Cry::Scaleform