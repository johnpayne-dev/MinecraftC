#pragma once
#include "../Utilities/Random.h"
#include "../Level/Level.h"

typedef struct PlayerAI {
	RandomGenerator random;
	float x, y;
	Entity mob;
	bool jumping;
	struct Entity * parent;
} * PlayerAI;

PlayerAI PlayerAICreate(struct Entity * parent);
void PlayerAIUpdate(PlayerAI ai);
void PlayerAITick(PlayerAI ai, Entity mob);
void PlayerAIJumpFromGround(PlayerAI ai);
void PlayerAIDestroy(PlayerAI ai);
