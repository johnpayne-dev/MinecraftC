#include "CombinedNoise.h"
#include <stdlib.h>

void CombinedNoiseCreate(CombinedNoise * noise, Noise * n1, Noise * n2) {
	NoiseCreate(noise);
	noise->type = NoiseTypeCombined;
	noise->combined = (struct CombinedNoiseData) {
		.noise1 = n1,
		.noise2 = n2,
	};
}

float CombinedNoiseCompute(CombinedNoise * noise, float x, float y) {
	return NoiseCompute(noise->combined.noise1, x + NoiseCompute(noise->combined.noise2, x, y), y);
}
