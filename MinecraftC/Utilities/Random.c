#include "Random.h"
#include "Memory.h"
#include <stdlib.h>
#include <math.h>

void RandomGeneratorCreate(RandomGenerator * generator, uint64_t seed) {
	generator->seed = seed;
	generator->state = seed;
	generator->lastNormal = INFINITY;
}

uint64_t RandomGeneratorInteger(RandomGenerator * generator) {
	generator->state ^= (generator->state >> 12);
	generator->state ^= (generator->state << 25);
	generator->state ^= (generator->state >> 27);
	return generator->state * 2685821657736338717ULL;
}

int64_t RandomGeneratorIntegerRange(RandomGenerator * generator, int64_t min, int64_t max) {
	return min + RandomGeneratorInteger(generator) % (max + 1 - min);
}

double RandomGeneratorUniform(RandomGenerator * generator) {
	return (double)RandomGeneratorInteger(generator) / (double)UINT64_MAX;
}

double RandomGeneratorUniformRange(RandomGenerator * generator, double min, double max) {
	return min + RandomGeneratorUniform(generator) * (max - min);
}

double RandomGeneratorNormal(RandomGenerator * generator, double stddev) {
	if (generator->lastNormal != INFINITY) {
		double r = generator->lastNormal;
		generator->lastNormal = INFINITY;
		return r * stddev;
	}

	double r = sqrt(-2.0 * log(1.0 - RandomGeneratorUniform(generator)));
	double phi = 2.0 * M_PI * (1.0 - RandomGeneratorUniform(generator));

	generator->lastNormal = r * cos(phi);
	return r * sin(phi) * stddev;
}

void RandomSetSeed(unsigned int seed) {
	srand(seed);
}

int RandomInteger() {
	return rand();
}

int RandomIntegerRange(int min, int max) {
	return RandomInteger() % (max - min) + min;
}

double RandomUniform() {
	return (double)rand() / (double)RAND_MAX;
}

double RandomUniformRange(double min, double max) {
	return RandomUniform() * (max - min) + min;
}
