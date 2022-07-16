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

float CombinedNoiseCompute(CombinedNoise noise, float2 v) {
	CombinedNoiseData this = noise->typeData;
	return NoiseCompute(this->noise1, (float2){ v.x + NoiseCompute(this->noise2, v), v.y });
}

void CombinedNoiseDestroy(CombinedNoise noise) {
	CombinedNoiseData this = noise->typeData;
	MemoryFree(this);
}
