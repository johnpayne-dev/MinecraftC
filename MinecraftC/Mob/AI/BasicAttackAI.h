#pragma once
#include "BasicAI.h"

typedef BasicAI BasicAttackAI;

typedef enum BasicAttackAIType
{
	BasicAttackAITypeNone,
	BasicAttackAIJumpAttack,
	BasicAttackAITypeCreeper,
	BasicAttackAITypeSkeleton,
} BasicAttackAIType;

typedef struct BasicAttackAIData
{
	int Damage;
	BasicAttackAIType Type;
	void * TypeData;
} * BasicAttackAIData;

BasicAttackAI BasicAttackAICreate(void);
void BasicAttackAIUpdate(BasicAttackAI ai);
void BasicAttackAIDoAttack(BasicAttackAI ai);
bool BasicAttackAIAttack(BasicAttackAI ai, Entity entity);
void BasicAttackAIHurt(BasicAttackAI ai, Entity entity, int damage);
void BasicAttackAIDestroy(BasicAttackAI ai);
