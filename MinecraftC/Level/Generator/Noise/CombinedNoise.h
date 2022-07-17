#pragma once
#include "Noise.h"

typedef Noise CombinedNoise;

typedef struct CombinedNoiseData {
	Noise noise1;
	Noise noise2;
} * CombinedNoiseData;

CombinedNoise CombinedNoiseCreate(Noise n1, Noise n2);
float CombinedNoiseCompute(CombinedNoise noise, float x, float y);
void CombinedNoiseDestroy(CombinedNoise noise);
