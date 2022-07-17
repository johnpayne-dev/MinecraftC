#pragma once
#include <stdbool.h>
#include "../MovingObjectPosition.h"

typedef struct AABB {
	float x0, y0, z0;
	float x1, y1, z1;
} AABB;

#define AABBNull ((AABB){ 0 })

AABB AABBExpand(AABB c, float x, float y, float z);
AABB AABBGrow(AABB c, float x, float y, float z);
AABB AABBMove(AABB c, float x, float y, float z);
float AABBClipXCollide(AABB c0, AABB c1, float xa);
float AABBClipYCollide(AABB c0, AABB c1, float ya);
float AABBClipZCollide(AABB c0, AABB c1, float za);
bool AABBIntersects(AABB c0, AABB c1);
bool AABBIntersectsInner(AABB c0, AABB c1);
bool AABBContainsPoint(AABB c, Vector3D p);
float AABBGetSize(AABB c);
AABB AABBShrink(AABB c, float x, float y, float z);
MovingObjectPosition AABBClip(AABB c, Vector3D v1, Vector3D v2);
bool AABBIsNull(AABB c);
