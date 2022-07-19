#include "TNTBlock.h"
#include "../Level.h"
#include "../../Particle/PrimedTNT.h"

void TNTBlockCreate(TNTBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeTNT, 8, sound, particleGravity);
}

int TNTBlockGetTextureID(TNTBlock * block, int face) {
	return face == 0 ? block->textureID + 2 : (face == 1 ? block->textureID + 1 : block->textureID);
}

void TNTBlockExplode(TNTBlock * block, Level * level, int x, int y, int z) {
	PrimedTNT * tnt = malloc(sizeof(PrimedTNT));
	PrimedTNTCreate(tnt, level, x + 0.5, y + 0.5, z + 0.5);
	tnt->tnt.life = RandomIntegerRange(5, 14);
	LevelAddEntity(level, tnt);
}

void TNTBlockSpawnBreakParticles(TNTBlock * block, Level * level, int x, int y, int z, ParticleManager * particles) {
	PrimedTNT * tnt = malloc(sizeof(PrimedTNT));
	PrimedTNTCreate(tnt, level, x + 0.5, y + 0.5, z + 0.5f);
	LevelAddEntity(level, tnt);
}
