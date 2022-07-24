#pragma once
#include "Block.h"

typedef Block TNTBlock;

void TNTBlockCreate(TNTBlock * block, TileSound sound, float particleGravity);
int TNTBlockGetTextureID(TNTBlock * block, int face);
void TNTBlockExplode(TNTBlock * block, struct Level * level, int x, int y, int z);
void TNTBlockSpawnBreakParticles(TNTBlock * block, struct Level * level, int x, int y, int z, ParticleManager * particles);
