#pragma once
#include "Utilities/String.h"
#include "Level/Tile/Block.h"

extern List(BlockType) SessionDataAllowedBlocks;
void SessionDataInitialize(void);
void SessionDataFree(void);
