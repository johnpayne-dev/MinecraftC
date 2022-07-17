#pragma once
#include "Time.h"

typedef struct RandomGenerator
{
	uint64_t seed;
	uint64_t state;
	double lastNormal;
} * RandomGenerator;

RandomGenerator RandomGeneratorCreate(uint64_t seed);

uint64_t RandomGeneratorInteger(RandomGenerator generator);

int64_t RandomGeneratorIntegerRange(RandomGenerator generator, int64_t min, int64_t max);

double RandomGeneratorUniform(RandomGenerator generator);

double RandomGeneratorUniformRange(RandomGenerator generator, double min, double max);

double RandomGeneratorNormal(RandomGenerator generator, double stddev);

void RandomGeneratorDestroy(RandomGenerator generator);

void RandomSetSeed(unsigned int seed);

int RandomInteger(void);

int RandomIntegerRange(int min, int max);

double RandomUniform(void);

double RandomUniformRange(double min, double max);
