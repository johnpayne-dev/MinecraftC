#include "OctaveNoise.h"
#include "../../../Utilities/Memory.h"

OctaveNoise OctaveNoiseCreate(RandomGenerator random, int octaveCount)
{
	Noise noise = NoiseCreate();
	noise->Type = NoiseTypeOctave;
	noise->TypeData = MemoryAllocate(sizeof(struct OctaveNoiseData));
	OctaveNoiseData this = noise->TypeData;
	*this = (struct OctaveNoiseData)
	{
		.OctaveCount = octaveCount,
		.Octaves = MemoryAllocate(octaveCount * sizeof(PerlinNoise)),
	};
	for (int i = 0; i < octaveCount; i++) { this->Octaves[i] = PerlinNoiseCreate(random); }
	return noise;
}

float OctaveNoiseCompute(OctaveNoise noise, float2 v)
{
	OctaveNoiseData this = noise->TypeData;
	float a = 0.0;
	float b = 1.0;
	for (int i = 0; i < this->OctaveCount; i++)
	{
		a += NoiseCompute(this->Octaves[i], v / b) * b;
		b *= 2;
	}
	return a;
}

void OctaveNoiseDestroy(OctaveNoise noise)
{
	OctaveNoiseData this = noise->TypeData;
	for (int i = 0; i < this->OctaveCount; i++) { NoiseDestroy(this->Octaves[i]); }
	MemoryFree(this->Octaves);
	MemoryFree(this);
}
