#pragma once
#include "Block.h"

typedef Block SpongeBlock;

void SpongeBlockCreate(SpongeBlock * block, TileSound sound, float particleGravity);
void SpongeBlockOnAdded(SpongeBlock * block, struct Level * level, int x, int y, int z);
void SpongeBlockOnRemoved(SpongeBlock * block, struct Level * level, int x, int y, int z);
