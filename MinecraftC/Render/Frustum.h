#pragma once
#include "../Physics/AABB.h"

typedef struct Frustum
{
	float Planes[6][16];
	float Projection[16];
	float ModelView[16];
	float Clip[16];
} Frustum;

Frustum FrustumUpdate(void);
bool FrustumContainsBox(Frustum frustum, float3 v0, float3 v1);
