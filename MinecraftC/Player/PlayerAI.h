#pragma once
#include "../Mob/AI/BasicAI.h"
#include "Player.h"

typedef BasicAI PlayerAI;

typedef struct PlayerAIData
{
	Player Parent;
} * PlayerAIData;

PlayerAI PlayerAICreate(Player parent);
void PlayerAIUpdate(PlayerAI ai);
void PlayerAIDestroy(PlayerAI ai);
