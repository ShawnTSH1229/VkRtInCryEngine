// Copyright 2019-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CrySystem/ConsoleRegistration.h>

struct SAIConsoleVarsMNMPathfinder
{
	void Init();

	DeclareConstIntCVar(BeautifyPath, 1);
	DeclareConstIntCVar(PathStringPullingIterations, 5);
	DeclareConstIntCVar(MNMPathfinderMT, 1);
	DeclareConstIntCVar(MNMPathfinderConcurrentRequests, 4);
	DeclareConstIntCVar(MNMPathfinderPositionInTrianglePredictionType, 1);
	DeclareConstIntCVar(PathfinderDangerCostForAttentionTarget, 5);
	DeclareConstIntCVar(PathfinderDangerCostForExplosives, 2);
	DeclareConstIntCVar(PathfinderAvoidanceCostForGroupMates, 2);

	int   MNMPathFinderDebug;
	float MNMPathFinderQuota;
	float AllowedTimeForPathfinding;
	float PathfinderExplosiveDangerRadius;
	float PathfinderExplosiveDangerMaxThreatDistance;
	float PathfinderGroupMatesAvoidanceRadius;
};