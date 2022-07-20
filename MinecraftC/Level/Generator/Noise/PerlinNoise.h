#pragma once
#include "Noise.h"
#include "../../../Utilities/Random.h"

typedef Noise PerlinNoise;

typedef struct PerlinNoiseData PerlinNoiseData;

void PerlinNoiseCreate(PerlinNoise * noise, RandomGenerator * random);
float PerlinNoiseCompute(PerlinNoise * noise, float x, float y);
