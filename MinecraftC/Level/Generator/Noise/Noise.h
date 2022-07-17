#pragma once

typedef enum NoiseType {
	NoiseTypeNone,
	NoiseTypePerlin,
	NoiseTypeOctave,
	NoiseTypeCombined,
} NoiseType;

typedef struct Noise {
	NoiseType type;
	void * typeData;
} * Noise;

Noise NoiseCreate(void);
float NoiseCompute(Noise noise, float x, float y);
void NoiseDestroy(Noise noise);
