#pragma once
#include "../Level.h"
#include "../../ProgressBarDisplay.h"
#include "../../Utilities/Random.h"

typedef struct LevelGenerator
{
	ProgressBarDisplay ProgressBar;
	int Width, Depth, Height;
	RandomGenerator Random;
	unsigned char * Blocks;
	int WaterLevel;
	int * FloodData;
} * LevelGenerator;

LevelGenerator LevelGeneratorCreate(ProgressBarDisplay progressBar);
Level LevelGeneratorGenerate(LevelGenerator generator, const char * userName, int width, int depth, int var);
void LevelGeneratorDestroy(LevelGenerator generator);
