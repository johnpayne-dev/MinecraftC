#pragma once
#include "AI.h"

typedef AI BasicAI;

typedef enum BasicAIType
{
	BasicAITypeNone,
	BasicAITypeBasicAttack,
	BasicAITypeSheep,
	BasicAITypePlayer,
} BasicAIType;

typedef struct BasicAIData
{
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
	BasicAIType Type;
	void * TypeData;
} * BasicAIData;

BasicAI BasicAICreate(void);
void BasicAITick(BasicAI ai, Level level, Entity mob);
void BasicAIJumpFromGround(BasicAI ai);
void BasicAIUpdate(BasicAI ai);
void BasicAIBeforeRemove(BasicAI ai);
void BasicAIHurt(BasicAI ai, Entity entity, int damage);
void BasicAIDestroy(BasicAI ai);
