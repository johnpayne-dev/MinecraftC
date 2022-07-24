#pragma once
#include "../../ProgressBarDisplay.h"
#include "../../Utilities/Random.h"

struct Level;

typedef struct LevelGenerator {
	ProgressBarDisplay * progressBar;
	int width, depth, height;
	RandomGenerator random;
	uint8_t * blocks;
	int waterLevel;
	int * floodData;
} LevelGenerator;

void LevelGeneratorCreate(LevelGenerator * generator, ProgressBarDisplay * progressBar);
void LevelGeneratorGenerate(LevelGenerator * generator, int width, int depth, struct Level * level);
void LevelGeneratorDestroy(LevelGenerator * generator);
