#include "TNTBlock.h"
#include "../Level.h"
#include "../../Particle/PrimedTNT.h"

TNTBlock TNTBlockCreate() {
	return BlockCreate(BlockTypeTNT, 8);
}

int TNTBlockGetTextureID(TNTBlock block, int face) {
	return face == 0 ? block->textureID + 2 : (face == 1 ? block->textureID + 1 : block->textureID);
}

int TNTBlockGetDropCount(TNTBlock block) {
	return 0;
}

void TNTBlockExplode(TNTBlock block, Level * level, int x, int y, int z) {
	PrimedTNT * tnt = malloc(sizeof(PrimedTNT));
	PrimedTNTCreate(tnt, level, x + 0.5, y + 0.5, z + 0.5);
	tnt->tnt.life = RandomIntegerRange(5, 14);
	LevelAddEntity(level, tnt);
}

void TNTBlockSpawnBreakParticles(TNTBlock block, Level * level, int x, int y, int z, ParticleManager * particles) {
	PrimedTNT * tnt = malloc(sizeof(PrimedTNT));
	PrimedTNTCreate(tnt, level, x + 0.5, y + 0.5, z + 0.5f);
	LevelAddEntity(level, tnt);
}
