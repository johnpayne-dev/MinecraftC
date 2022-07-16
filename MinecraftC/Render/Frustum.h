#pragma once
#include "../Physics/AABB.h"

typedef struct Frustum {
	float planes[6][16];
	float projection[16];
	float modelView[16];
	float clip[16];
} Frustum;

Frustum FrustumUpdate(void);
bool FrustumContainsBox(Frustum frustum, float3 v0, float3 v1);
