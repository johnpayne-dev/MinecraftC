#pragma once
#include "Noise.h"

typedef Noise CombinedNoise;

typedef struct CombinedNoiseData CombinedNoiseData;

void CombinedNoiseCreate(CombinedNoise * noise, Noise * n1, Noise * n2);
float CombinedNoiseCompute(CombinedNoise * noise, float x, float y);
