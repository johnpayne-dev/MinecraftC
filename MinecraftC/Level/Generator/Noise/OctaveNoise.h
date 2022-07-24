#pragma once
#include "Noise.h"
#include "PerlinNoise.h"

typedef Noise OctaveNoise;

typedef struct OctaveNoiseData OctaveNoiseData;

void OctaveNoiseCreate(OctaveNoise * noise, RandomGenerator * random, int octaveCount);
float OctaveNoiseCompute(OctaveNoise * noise, float x, float y);
void OctaveNoiseDestroy(OctaveNoise * noise);
