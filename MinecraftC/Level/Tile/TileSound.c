#include "TileSound.h"
#include "../../Utilities/Random.h"

struct TileSounds TileSounds = {
	.none = { .name = "-", .volume = 0.0, .pitch = 0.0, .type = TileSoundTypeNone },
	.grass = { .name = "Grass", .volume = 0.6, .pitch = 1.0, .type = TileSoundTypeGrass },
	.cloth = { .name = "Grass", .volume = 0.7, .pitch = 1.2, .type = TileSoundTypeCloth },
	.gravel = { .name = "Gravel", .volume = 1.0, .pitch = 1.0, .type = TileSoundTypeGravel },
	.stone = { .name = "Stone", .volume = 1.0, .pitch = 1.0, .type = TileSoundTypeStone },
	.metal = { .name = "Stone", .volume = 1.0, .pitch = 2.0, .type = TileSoundTypeMetal },
	.wood = { .name = "Wood", .volume = 1.0, .pitch = 1.0, .type = TileSoundTypeWood },
};

float TileSoundGetVolume(TileSound sound) {
	return sound.volume / (RandomUniform() * 0.4 + 1.0) * 0.5;
}

float TileSoundGetPitch(TileSound sound) {
	return sound.pitch / (RandomUniform() * 0.2 + 0.9);
}
