// Copyright 2011-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryAISystem/IAIBubblesSystem.h>

bool AIQueueBubbleMessage(const char* messageName, const EntityId entityID,
                          const char* message, uint32 flags, float duration = 0, SAIBubbleRequest::ERequestType requestType = SAIBubbleRequest::eRT_ErrorMessage);
