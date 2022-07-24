#pragma once
#include "Physics/Vector3D.h"

typedef struct MovingObjectPosition {
	int entityPosition;
	int x, y, z;
	int face;
	Vector3D vector;
	struct Entity * entity;
	bool null;
} MovingObjectPosition;
