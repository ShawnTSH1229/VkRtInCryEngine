// Copyright 2012-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/File/IFileChangeMonitor.h>

class CMannequinChangeMonitor : public IFileChangeListener
{
public:
	CMannequinChangeMonitor();
	~CMannequinChangeMonitor();

	virtual void OnFileChange(const char* sFilename, EChangeType eType) override;

	class CMannequinFileChangeWriter* m_pFileChangeWriter;

};
