#pragma once
#include "Block.h"

typedef Block BookshelfBlock;

void BookshelfBlockCreate(BookshelfBlock * block, TileSound sound, float particleGravity);
int BookshelfBlockGetTextureID(BookshelfBlock * block, int face);
