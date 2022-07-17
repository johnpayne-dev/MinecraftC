#include "CombinedNoise.h"
#include "../../../Utilities/Memory.h"

CombinedNoise CombinedNoiseCreate(Noise n1, Noise n2) {
	Noise noise = NoiseCreate();
	noise->type = NoiseTypeCombined;
	noise->typeData = MemoryAllocate(sizeof(struct CombinedNoiseData));
	CombinedNoiseData this = noise->typeData;
	*this = (struct CombinedNoiseData) {
		.noise1 = n1,
		.noise2 = n2,
	};
	return noise;
}

float CombinedNoiseCompute(CombinedNoise noise, float x, float y) {
	CombinedNoiseData this = noise->typeData;
	return NoiseCompute(this->noise1, x + NoiseCompute(this->noise2, x, y), y);
}

void CombinedNoiseDestroy(CombinedNoise noise) {
	CombinedNoiseData this = noise->typeData;
	MemoryFree(this);
}
