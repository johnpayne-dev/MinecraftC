#pragma once
#include "Block.h"

typedef Block GlassBlock;

void GlassBlockCreate(GlassBlock * block, TileSound sound, float particleGravity);
bool GlassBlockCanRenderSide(GlassBlock * block, struct Level * level, int x, int y, int z, int side);
bool GlassBlockIsOpaque(GlassBlock * block);
bool GlassBlockIsSolid(GlassBlock * block);
