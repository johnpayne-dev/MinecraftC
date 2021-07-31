#include "TNTBlock.h"
#include "../Level.h"

TNTBlock TNTBlockCreate()
{
	return BlockCreate(BlockTypeTNT, 8);
}

int TNTBlockGetTextureID(TNTBlock block, int face)
{
	return face == 0 ? block->TextureID + 2 : (face == 1 ? block->TextureID + 1 : block->TextureID);
}

int TNTBlockGetDropCount(TNTBlock block)
{
	return 0;
}

void TNTBlockExplode(TNTBlock block, Level level, int x, int y, int z)
{
}

void TNTBlockSpawnBreakParticles(TNTBlock block, Level level, int x, int y, int z, ParticleManager particles)
{
}
