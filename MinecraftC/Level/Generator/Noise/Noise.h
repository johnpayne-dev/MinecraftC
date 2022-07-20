#pragma once

typedef enum NoiseType {
	NoiseTypeNone,
	NoiseTypePerlin,
	NoiseTypeOctave,
	NoiseTypeCombined,
} NoiseType;

typedef struct Noise {
	NoiseType type;
	union {
		struct PerlinNoiseData {
			int hash[512];
		} perlin;
		struct OctaveNoiseData {
			int count;
			struct Noise * noises;
		} octave;
		struct CombinedNoiseData {
			struct Noise * noise1;
			struct Noise * noise2;
		} combined;
	};
} Noise;

void NoiseCreate(Noise * noise);
float NoiseCompute(Noise * noise, float x, float y);
void NoiseDestroy(Noise * noise);
