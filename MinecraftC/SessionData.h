#pragma once
#include "Utilities/String.h"
#include "Level/Tile/Block.h"

extern List(Block) SessionDataAllowedBlocks;
void SessionDataInitialize(void);
void SessionDataFree(void);
