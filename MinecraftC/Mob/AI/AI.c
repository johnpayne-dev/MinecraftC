#include "AI.h"
#include "BasicAI.h"

AI AICreate(void)
{
	AI ai = MemoryAllocate(sizeof(struct AI));
	*ai = (struct AI){ .DefaultLookAngle = 0, };
	return ai;
}

void AITick(AI ai, Level level, Entity mob)
{
	if (ai->Type == AITypeBasic) { return BasicAITick(ai, level, mob); }
}

void AIBeforeRemove(AI ai)
{
	if (ai->Type == AITypeBasic) { return BasicAIBeforeRemove(ai); }
}

void AIHurt(AI ai, Entity entity, int damage)
{
	if (ai->Type == AITypeBasic) { return BasicAIHurt(ai, entity, damage); }
}

void AIDestroy(AI ai)
{
	if (ai->Type == AITypeBasic) { BasicAIDestroy(ai); }
	MemoryFree(ai);
}
