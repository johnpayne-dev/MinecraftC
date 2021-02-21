#include "CombinedNoise.h"
#include "../../../Utilities/Memory.h"

CombinedNoise CombinedNoiseCreate(Noise n1, Noise n2)
{
	Noise noise = NoiseCreate();
	noise->Type = NoiseTypeCombined;
	noise->TypeData = MemoryAllocate(sizeof(struct CombinedNoiseData));
	CombinedNoiseData this = noise->TypeData;
	*this = (struct CombinedNoiseData)
	{
		.Noise1 = n1,
		.Noise2 = n2,
	};
	return noise;
}

float CombinedNoiseCompute(CombinedNoise noise, float2 v)
{
	CombinedNoiseData this = noise->TypeData;
	return NoiseCompute(this->Noise1, (float2){ v.x + NoiseCompute(this->Noise2, v), v.y });
}

void CombinedNoiseDestroy(CombinedNoise noise)
{
	CombinedNoiseData this = noise->TypeData;
	MemoryFree(this);
}
