#pragma once
#include "Block.h"

typedef Block BookshelfBlock;

BookshelfBlock BookshelfBlockCreate(void);
int BookshelfBlockGetTextureID(BookshelfBlock block, int face);
