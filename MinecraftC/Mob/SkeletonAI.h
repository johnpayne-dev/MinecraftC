#pragma once
#include "AI/BasicAttackAI.h"
#include "Skeleton.h"

typedef BasicAttackAI SkeletonAI;

typedef struct SkeletonAIData
{
	Skeleton Parent;
} * SkeletonAIData;

SkeletonAI SkeletonAICreate(Skeleton parent);
void SkeletonAITick(SkeletonAI ai, Level level, Mob mob);
void SkeletonAIBeforeRemove(SkeletonAI ai);
void SkeletonAIDestroy(SkeletonAI ai);
