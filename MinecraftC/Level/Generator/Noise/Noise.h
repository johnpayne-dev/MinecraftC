#pragma once
#include "../../../Utilities/LinearMath.h"

typedef enum NoiseType {
	NoiseTypeNone,
	NoiseTypePerlin,
	NoiseTypeOctave,
	NoiseTypeCombined,
} NoiseType;

typedef struct Noise {
	NoiseType type;
	void * typeData;
} * Noise;

Noise NoiseCreate(void);
float NoiseCompute(Noise noise, float2 v);
void NoiseDestroy(Noise noise);
