#pragma once
#include "../../Level/Level.h"

typedef enum AIType
{
	AITypeBasic,
	AITypeBasicAttack,
	AITypeJumpAttack,
} AIType;

typedef struct AI
{
	int DefaultLookAngle;
	AIType Type;
	void * TypeData;
} * AI;

AI AICreate(void);
void AITick(AI ai, Level level, Entity mob);
void AIBeforeRemove(AI ai);
void AIHurt(AI ai, Entity entity, int damage);
void AIDestroy(AI ai);
