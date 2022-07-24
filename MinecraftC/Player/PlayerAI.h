#pragma once
#include "../Utilities/Random.h"
#include <stdbool.h>

typedef struct PlayerAI {
	RandomGenerator random;
	float x, y;
	struct Entity * mob;
	bool jumping;
	struct Entity * parent;
} PlayerAI;

void PlayerAICreate(PlayerAI * ai, struct Entity * parent);
void PlayerAIUpdate(PlayerAI * ai);
void PlayerAITick(PlayerAI * ai, struct Entity * mob);
void PlayerAIJumpFromGround(PlayerAI * ai);
