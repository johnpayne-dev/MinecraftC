#include "Noise.h"
#include "OctaveNoise.h"
#include "CombinedNoise.h"
#include "PerlinNoise.h"
#include <stdlib.h>

void NoiseCreate(Noise * noise) {
	*noise = (Noise){ 0 };
}

float NoiseCompute(Noise * noise, float x, float y) {
	if (noise->type == NoiseTypeOctave) { return OctaveNoiseCompute(noise, x, y); }
	if (noise->type == NoiseTypePerlin) { return PerlinNoiseCompute(noise, x, y); }
	if (noise->type == NoiseTypeCombined) { return CombinedNoiseCompute(noise, x, y); }
	return 0.0;
}

void NoiseDestroy(Noise * noise) {
	if (noise->type == NoiseTypeOctave) { OctaveNoiseDestroy(noise); }
}
