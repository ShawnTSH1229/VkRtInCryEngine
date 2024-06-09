// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

enum class EFileStatFilter
{
	None = 0,
	AddedLocally,
	AddedRemotely,
	CheckedOutByOthers,
	Tracked,
	DeletedRemotely
};
