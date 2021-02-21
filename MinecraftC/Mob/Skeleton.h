#pragma once
#include "Humanoid.h"

typedef Humanoid Skeleton;

Skeleton SkeletonCreate(Level level, float3 pos);
void SkeletonShootArrow(Skeleton skeleton, Level level);
void SkeletonShootRandomArrow(Skeleton skeleton);
