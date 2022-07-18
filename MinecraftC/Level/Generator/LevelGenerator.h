#pragma once
#include "../Level.h"
#include "../../ProgressBarDisplay.h"
#include "../../Utilities/Random.h"

typedef struct LevelGenerator {
	ProgressBarDisplay progressBar;
	int width, depth, height;
	RandomGenerator random;
	uint8_t * blocks;
	int waterLevel;
	int * floodData;
} * LevelGenerator;

LevelGenerator LevelGeneratorCreate(ProgressBarDisplay progressBar);
Level LevelGeneratorGenerate(LevelGenerator generator, const char * userName, int width, int depth);
void LevelGeneratorDestroy(LevelGenerator generator);
