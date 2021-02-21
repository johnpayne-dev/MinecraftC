#pragma once
#include "Noise.h"

typedef Noise CombinedNoise;

typedef struct CombinedNoiseData
{
	Noise Noise1;
	Noise Noise2;
} * CombinedNoiseData;

CombinedNoise CombinedNoiseCreate(Noise n1, Noise n2);
float CombinedNoiseCompute(CombinedNoise noise, float2 v);
void CombinedNoiseDestroy(CombinedNoise noise);
