// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

namespace CryAudio
{
namespace Impl
{
namespace PortAudio
{
class CImpl;
class CEventInstance;

extern CImpl* g_pImpl;

using EventInstances = std::vector<CEventInstance*>;
} // namespace PortAudio
} // namespace Impl
} // namespace CryAudio