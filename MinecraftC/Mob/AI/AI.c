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
	if (ai->Type == AITypeBasic) { BasicAITick(ai, level, mob); return; }
}

void AIBeforeRemove(AI ai)
{
	if (ai->Type == AITypeBasic) { BasicAIBeforeRemove(ai); return; }
}

void AIHurt(AI ai, Entity entity, int damage)
{
	if (ai->Type == AITypeBasic) { BasicAIHurt(ai, entity, damage); return; }
}

void AIDestroy(AI ai)
{
	if (ai->Type == AITypeBasic) { BasicAIDestroy(ai); }
	MemoryFree(ai);
}
