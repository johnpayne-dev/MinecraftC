#pragma once
#include "Block.h"

typedef Block StoneBlock;

StoneBlock StoneBlockCreate(int textureID);
BlockType StoneBlockGetDrop(StoneBlock block);
