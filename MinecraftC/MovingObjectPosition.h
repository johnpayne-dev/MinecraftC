#pragma once
#include "Utilities/LinearMath.h"

typedef struct MovingObjectPosition {
	int entityPosition;
	int3 xyz;
	int face;
	float3 vector;
	struct Entity * entity;
	bool null;
} MovingObjectPosition;
