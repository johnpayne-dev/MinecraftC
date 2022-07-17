#pragma once
#include "../Utilities/Random.h"
#include "../Level/Level.h"

typedef struct PlayerAI {
	int defaultLookAngle;
	RandomGenerator random;
	float x, y;
	float rotation;
	Level level;
	Entity mob;
	bool jumping;
	int attackDelay;
	float runSpeed;
	int noActionTime;
	Entity attackTarget;
	struct Entity * parent;
} * PlayerAI;

PlayerAI PlayerAICreate(struct Entity * parent);
void PlayerAIUpdate(PlayerAI ai);
void PlayerAITick(PlayerAI ai, Level level, Entity mob);
void PlayerAIJumpFromGround(PlayerAI ai);
void PlayerAIBeforeRemove(PlayerAI ai);
void PlayerAIHurt(PlayerAI ai, Entity entity, int damage);
void PlayerAIDestroy(PlayerAI ai);
