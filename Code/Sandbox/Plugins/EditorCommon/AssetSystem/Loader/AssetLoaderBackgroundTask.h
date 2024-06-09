// Copyright 2016-2022 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <functional>

class CAsset;

namespace AssetLoaderBackgroundTask
{
	// Run a background task to load assets from provided paths, then calls \p finalize in main thread passing the results.
	void RunAsync(const std::vector<string>& assetRootPaths, std::function<void(const std::vector<CAsset*>&)> finalize);

	// Load assets from provided paths, then calls \p finalize passing the results.
	void Run(const std::vector<string>& assetRootPaths, std::function<void(const std::vector<CAsset*>&)> finalize);
};
