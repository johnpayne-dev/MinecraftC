#pragma once
#include "Noise.h"
#include "PerlinNoise.h"

typedef Noise OctaveNoise;

typedef struct OctaveNoiseData {
	int octaveCount;
	PerlinNoise * octaves;
} * OctaveNoiseData;

OctaveNoise OctaveNoiseCreate(RandomGenerator random, int octaveCount);
float OctaveNoiseCompute(OctaveNoise noise, float2 v);
void OctaveNoiseDestroy(OctaveNoise noise);
