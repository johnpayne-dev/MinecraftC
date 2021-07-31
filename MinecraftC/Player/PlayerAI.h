#pragma once
#include "../Utilities/Random.h"
#include "../Level/Level.h"

typedef struct PlayerAI
{
	int DefaultLookAngle;
	RandomGenerator Random;
	float2 XY;
	float Rotation;
	Level Level;
	Entity Mob;
	bool Jumping;
	int AttackDelay;
	float RunSpeed;
	int NoActionTime;
	Entity AttackTarget;
	struct Entity * Parent;
} * PlayerAI;

PlayerAI PlayerAICreate(struct Entity * parent);
void PlayerAIUpdate(PlayerAI ai);
void PlayerAITick(PlayerAI ai, Level level, Entity mob);
void PlayerAIJumpFromGround(PlayerAI ai);
void PlayerAIBeforeRemove(PlayerAI ai);
void PlayerAIHurt(PlayerAI ai, Entity entity, int damage);
void PlayerAIDestroy(PlayerAI ai);
