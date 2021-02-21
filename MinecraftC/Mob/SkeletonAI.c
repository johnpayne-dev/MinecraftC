#include "SkeletonAI.h"

SkeletonAI SkeletonAICreate(Skeleton parent)
{
	SkeletonAI ai = BasicAttackAICreate();
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	attackAI->Type = BasicAttackAITypeSkeleton;
	SkeletonAIData skeleton = MemoryAllocate(sizeof(struct SkeletonAIData));
	*skeleton = (struct SkeletonAIData){ .Parent = parent, };
	attackAI->TypeData = skeleton;
	return ai;
}

void SkeletonAITick(SkeletonAI ai, Level level, Mob mob)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	SkeletonAIData this = attackAI->TypeData;
	if (((MobData)mob->TypeData)->Health > 0 && RandomGeneratorIntegerRange(basicAI->Random, 0, 29) == 0 && basicAI->AttackTarget != NULL)
	{
		SkeletonShootArrow(this->Parent, level);
	}
}

void SkeletonAIBeforeRemove(SkeletonAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	SkeletonAIData this = attackAI->TypeData;
	SkeletonShootRandomArrow(this->Parent);
}

void SkeletonAIDestroy(SkeletonAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	SkeletonAIData this = attackAI->TypeData;
	MemoryFree(this);
}
