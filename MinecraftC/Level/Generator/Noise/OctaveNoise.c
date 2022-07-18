#include "OctaveNoise.h"
#include <stdlib.h>

OctaveNoise OctaveNoiseCreate(RandomGenerator random, int octaveCount) {
	Noise noise = NoiseCreate();
	noise->type = NoiseTypeOctave;
	noise->typeData = malloc(sizeof(struct OctaveNoiseData));
	OctaveNoiseData this = noise->typeData;
	*this = (struct OctaveNoiseData) {
		.octaveCount = octaveCount,
		.octaves = malloc(octaveCount * sizeof(PerlinNoise)),
	};
	for (int i = 0; i < octaveCount; i++) { this->octaves[i] = PerlinNoiseCreate(random); }
	return noise;
}

float OctaveNoiseCompute(OctaveNoise noise, float x, float y) {
	OctaveNoiseData this = noise->typeData;
	float a = 0.0;
	float b = 1.0;
	for (int i = 0; i < this->octaveCount; i++) {
		a += NoiseCompute(this->octaves[i], x / b, y / b) * b;
		b *= 2;
	}
	return a;
}

void OctaveNoiseDestroy(OctaveNoise noise) {
	OctaveNoiseData this = noise->typeData;
	for (int i = 0; i < this->octaveCount; i++) { NoiseDestroy(this->octaves[i]); }
	free(this->octaves);
	free(this);
}
