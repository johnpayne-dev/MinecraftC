#pragma once

typedef enum TileSoundType
{
	TileSoundTypeNone,
	TileSoundTypeGrass,
	TileSoundTypeCloth,
	TileSoundTypeGravel,
	TileSoundTypeStone,
	TileSoundTypeMetal,
	TileSoundTypeWood,
} TileSoundType;

typedef struct TileSound
{
	const char * Name;
	float Volume;
	float Pitch;
	TileSoundType Type;
} TileSound;

extern struct TileSounds
{
	TileSound None;
	TileSound Grass;
	TileSound Cloth;
	TileSound Gravel;
	TileSound Stone;
	TileSound Metal;
	TileSound Wood;
} TileSounds;

float TileSoundGetVolume(TileSound sound);
float TileSoundGetPitch(TileSound sound);
