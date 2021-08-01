#include "Random.h"
#include "Memory.h"
#include <stdlib.h>

RandomGenerator RandomGeneratorCreate(uint64_t seed)
{
	RandomGenerator generator = malloc(sizeof(struct RandomGenerator));
	generator->Seed = seed;
	generator->State = seed;
	generator->LastNormal = INFINITY;
	return generator;
}

uint64_t RandomGeneratorInteger(RandomGenerator generator)
{
	generator->State ^= (generator->State >> 12);
	generator->State ^= (generator->State << 25);
	generator->State ^= (generator->State >> 27);
	return generator->State * 2685821657736338717ULL;
}

int64_t RandomGeneratorIntegerRange(RandomGenerator generator, int64_t min, int64_t max)
{
	return min + RandomGeneratorInteger(generator) % (max + 1 - min);
}

double RandomGeneratorUniform(RandomGenerator generator)
{
	return (double)RandomGeneratorInteger(generator) / (double)UINT64_MAX;
}

double RandomGeneratorUniformRange(RandomGenerator generator, double min, double max)
{
	return min + RandomGeneratorUniform(generator) * (max - min);
}

double RandomGeneratorNormal(RandomGenerator generator, double stddev)
{
	if (generator->LastNormal != INFINITY)
	{
		double r = generator->LastNormal;
		generator->LastNormal = INFINITY;
		return r * stddev;
	}

	double r = sqrt(-2.0 * log(1.0 - RandomGeneratorUniform(generator)));
	double phi = 2.0 * pi * (1.0 - RandomGeneratorUniform(generator));

	generator->LastNormal = r * cos(phi);
	return r * sin(phi) * stddev;
}

void RandomGeneratorDestroy(RandomGenerator generator)
{
	free(generator);
}

void RandomSetSeed(unsigned int seed)
{
	srand(seed);
}

int RandomInteger()
{
	return rand();
}

int RandomIntegerRange(int min, int max)
{
	return RandomInteger() % (max - min) + min;
}

double RandomUniform()
{
	return (double)rand() / (double)RAND_MAX;
}

double RandomUniformRange(double min, double max)
{
	return RandomUniform() * (max - min) + min;
}
