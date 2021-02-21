#include "TileSound.h"
#include "../../Utilities/Random.h"

struct TileSounds TileSounds =
{
	.None = { .Name = "-", .Volume = 0.0, .Pitch = 0.0, .Type = TileSoundTypeNone },
	.Grass = { .Name = "Grass", .Volume = 0.6, .Pitch = 1.0, .Type = TileSoundTypeGrass },
	.Cloth = { .Name = "Grass", .Volume = 0.7, .Pitch = 1.2, .Type = TileSoundTypeCloth },
	.Gravel = { .Name = "Gravel", .Volume = 1.0, .Pitch = 1.0, .Type = TileSoundTypeGravel },
	.Stone = { .Name = "Stone", .Volume = 1.0, .Pitch = 1.0, .Type = TileSoundTypeStone },
	.Metal = { .Name = "Stone", .Volume = 1.0, .Pitch = 2.0, .Type = TileSoundTypeMetal },
	.Wood = { .Name = "Wood", .Volume = 1.0, .Pitch = 1.0, .Type = TileSoundTypeWood },
};

float TileSoundGetVolume(TileSound sound)
{
	return sound.Volume / (RandomUniform() * 0.4 + 1.0) * 0.5;
}

float TileSoundGetPitch(TileSound sound)
{
	return sound.Pitch / (RandomUniform() * 0.2 + 0.9);
}
