#include "Noise.h"
#include "OctaveNoise.h"
#include "CombinedNoise.h"
#include "PerlinNoise.h"
#include "../../../Utilities/Memory.h"

Noise NoiseCreate() {
	Noise noise = MemoryAllocate(sizeof(struct Noise));
	*noise = (struct Noise){ 0 };
	return noise;
}

float NoiseCompute(Noise noise, float2 v) {
	if (noise->type == NoiseTypeOctave) { return OctaveNoiseCompute(noise, v); }
	if (noise->type == NoiseTypePerlin) { return PerlinNoiseCompute(noise, v); }
	if (noise->type == NoiseTypeCombined) { return CombinedNoiseCompute(noise, v); }
	return 0.0;
}

void NoiseDestroy(Noise noise) {
	if (noise->type == NoiseTypeOctave) { OctaveNoiseDestroy(noise); }
	if (noise->type == NoiseTypePerlin) { PerlinNoiseDestroy(noise); }
	if (noise->type == NoiseTypeCombined) { CombinedNoiseDestroy(noise); }
	MemoryFree(noise);
}
