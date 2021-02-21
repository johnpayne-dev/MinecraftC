#pragma once
#include "Block.h"

typedef Block TNTBlock;

TNTBlock TNTBlockCreate(void);
int TNTBlockGetTextureID(TNTBlock block, int face);
int TNTBlockGetDropCount(TNTBlock block);
void TNTBlockExplode(TNTBlock block, struct Level * level, int x, int y, int z);
void TNTBlockSpawnBreakParticles(TNTBlock block, struct Level * level, int x, int y, int z, ParticleManager particles);
