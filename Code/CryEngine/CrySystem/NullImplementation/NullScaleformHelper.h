// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/Scaleform/IScaleformHelper.h>

struct CNullScaleformHelper : IScaleformHelper
{
public:
	CNullScaleformHelper(bool outputWarnings) : m_outputWarnings(outputWarnings) {}

	virtual bool Init() { return true; }
	virtual void Destroy() { delete this; }
	virtual void SetAmpEnabled(bool bEnabled) {}
	virtual void AmpAdvanceFrame() {}
	virtual void SetTranslatorWordWrappingMode(const char* szLanguage) {}
	virtual void OnLanguageChanged() {}
	virtual void ResetMeshCache() {}

	virtual std::shared_ptr<IFlashPlayer> CreateFlashPlayerInstance()
	{
		if (m_outputWarnings)
			CryWarning(VALIDATOR_MODULE_3DENGINE, VALIDATOR_ERROR, "Trying to create a Flash element without CryScaleform plugin. Add CryScaleform plugin to your cryproject file.");
		return nullptr;
	}

	virtual IFlashPlayerBootStrapper* CreateFlashPlayerBootStrapper()
	{
		if (m_outputWarnings)
			CryWarning(VALIDATOR_MODULE_3DENGINE, VALIDATOR_ERROR, "Trying to create a Flash element without CryScaleform plugin. Add CryScaleform plugin to your cryproject file.");
		return nullptr;
	}

	virtual void RenderFlashInfo() {}
	virtual void GetFlashMemoryUsage(ICrySizer* pSizer) {}
	virtual void SetFlashLoadMovieHandler(IFlashLoadMovieHandler* pHandler) {}
	virtual float GetFlashProfileResults() { return 0.0f; }
	virtual void GetFlashRenderStats(unsigned& numDPs, unsigned int& numTris) { numDPs = 0; numTris = 0; }
	virtual void SetRenderThreadIDs(threadID main, threadID render) {}

private:
	bool m_outputWarnings;
};

