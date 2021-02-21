#pragma once
#include "LinearMath.h"
#include "Time.h"

typedef struct RandomGenerator
{
	unsigned long Seed;
	unsigned long State;
	double LastNormal;
} * RandomGenerator;

RandomGenerator RandomGeneratorCreate(unsigned long seed);

unsigned long RandomGeneratorInteger(RandomGenerator generator);

long RandomGeneratorIntegerRange(RandomGenerator generator, long min, long max);

double RandomGeneratorUniform(RandomGenerator generator);

double RandomGeneratorUniformRange(RandomGenerator generator, double min, double max);

double RandomGeneratorNormal(RandomGenerator generator, double stddev);

void RandomGeneratorDestroy(RandomGenerator generator);

void RandomSetSeed(unsigned int seed);

int RandomInteger(void);

int RandomIntegerRange(int min, int max);

double RandomUniform(void);

double RandomUniformRange(double min, double max);
