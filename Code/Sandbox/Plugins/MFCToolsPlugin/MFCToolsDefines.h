// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#if defined(MFCToolsPlugin_EXPORTS)
	#define MFC_TOOLS_PLUGIN_API DLL_EXPORT
#else
	#define MFC_TOOLS_PLUGIN_API DLL_IMPORT
#endif
