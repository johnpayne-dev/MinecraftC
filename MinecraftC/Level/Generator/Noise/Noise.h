#pragma once
#include "../../../Utilities/LinearMath.h"

typedef enum NoiseType
{
	NoiseTypeNone,
	NoiseTypePerlin,
	NoiseTypeOctave,
	NoiseTypeCombined,
} NoiseType;

typedef struct Noise
{
	NoiseType Type;
	void * TypeData;
} * Noise;

Noise NoiseCreate(void);
float NoiseCompute(Noise noise, float2 v);
void NoiseDestroy(Noise noise);
