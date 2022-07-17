#pragma once
#include "Noise.h"
#include "../../../Utilities/Random.h"

typedef Noise PerlinNoise;

typedef struct PerlinNoiseData {
	int hash[512];
} * PerlinNoiseData;

PerlinNoise PerlinNoiseCreate(RandomGenerator random);
float PerlinNoiseCompute(PerlinNoise noise, float x, float y);
void PerlinNoiseDestroy(PerlinNoise noise);
