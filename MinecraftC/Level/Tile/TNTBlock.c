#include "TNTBlock.h"
#include "../Level.h"
#include "../../Item/PrimedTNT.h"

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
	if (!level->CreativeMode)
	{
		PrimedTNT tnt = PrimedTNTCreate(level, (float3){ x, y, z } + 0.5);
		((PrimedTNTData)tnt->TypeData)->Life = RandomIntegerRange(5, 15);
		LevelAddEntity(level, tnt);
	}
}

void TNTBlockSpawnBreakParticles(TNTBlock block, Level level, int x, int y, int z, ParticleManager particles)
{
	if (!level->CreativeMode) { LevelAddEntity(level, PrimedTNTCreate(level, (float3){ x, y, z } + 0.5)); }
}
