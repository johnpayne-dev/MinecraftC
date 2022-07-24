#include "OctaveNoise.h"
#include <stdlib.h>

void OctaveNoiseCreate(OctaveNoise * noise, RandomGenerator * random, int octaveCount) {
	NoiseCreate(noise);
	noise->type = NoiseTypeOctave;
	noise->octave = (struct OctaveNoiseData) {
		.count = octaveCount,
		.noises = malloc(octaveCount * sizeof(PerlinNoise)),
	};
	for (int i = 0; i < octaveCount; i++) {
		PerlinNoiseCreate(&noise->octave.noises[i], random);
	}
}

float OctaveNoiseCompute(OctaveNoise * noise, float x, float y) {
	float a = 0.0;
	float b = 1.0;
	for (int i = 0; i < noise->octave.count; i++) {
		a += NoiseCompute(&noise->octave.noises[i], x / b, y / b) * b;
		b *= 2;
	}
	return a;
}

void OctaveNoiseDestroy(OctaveNoise * noise) {
	free(noise->octave.noises);
}
