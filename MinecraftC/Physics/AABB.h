#pragma once
#include <stdbool.h>
#include "../Utilities/LinearMath.h"
#include "../MovingObjectPosition.h"

typedef struct AABB
{
	float3 V0;
	float3 V1;
} AABB;

#define AABBNull ((AABB){ 0 })

AABB AABBExpand(AABB c, float3 a);
AABB AABBGrow(AABB c, float3 a);
AABB AABBMove(AABB c, float3 a);
float AABBClipXCollide(AABB c0, AABB c1, float xa);
float AABBClipYCollide(AABB c0, AABB c1, float ya);
float AABBClipZCollide(AABB c0, AABB c1, float za);
bool AABBIntersects(AABB c0, AABB c1);
bool AABBIntersectsInner(AABB c0, AABB c1);
bool AABBContainsPoint(AABB c, float3 p);
float AABBGetSize(AABB c);
AABB AABBShrink(AABB c, float3 a);
MovingObjectPosition AABBClip(AABB c, float3 v1, float3 v2);
bool AABBIsNull(AABB c);
