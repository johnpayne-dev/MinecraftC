#pragma once
#include "Block.h"

typedef Block LogBlock;

void LogBlockCreate(LogBlock * block, TileSound sound, float particleGravity);
int LogBlockGetTextureID(LogBlock * block, int face);
