#pragma once
#include "Block.h"

typedef Block SpongeBlock;

SpongeBlock SpongeBlockCreate(void);
void SpongeBlockOnAdded(SpongeBlock block, struct Level * level, int x, int y, int z);
void SpongeBlockOnRemoved(SpongeBlock block, struct Level * level, int x, int y, int z);
