#pragma once
#include <stdbool.h>
#include "../Utilities/LinearMath.h"

typedef struct PositionUpdate
{
	float3 XYZ;
	float Yaw;
	float Pitch;
	bool Rotation;
	bool Position;
} PositionUpdate;
