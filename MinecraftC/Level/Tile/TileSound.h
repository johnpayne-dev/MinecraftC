#pragma once

typedef enum TileSoundType {
	TileSoundTypeNone,
	TileSoundTypeGrass,
	TileSoundTypeCloth,
	TileSoundTypeGravel,
	TileSoundTypeStone,
	TileSoundTypeMetal,
	TileSoundTypeWood,
} TileSoundType;

typedef struct TileSound {
	const char * name;
	float volume;
	float pitch;
	TileSoundType type;
} TileSound;

extern struct TileSounds {
	TileSound none;
	TileSound grass;
	TileSound cloth;
	TileSound gravel;
	TileSound stone;
	TileSound metal;
	TileSound wood;
} TileSounds;

float TileSoundGetVolume(TileSound sound);
float TileSoundGetPitch(TileSound sound);
