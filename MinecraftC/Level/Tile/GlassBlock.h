#pragma once
#include "Block.h"

typedef Block GlassBlock;

GlassBlock GlassBlockCreate(void);
bool GlassBlockCanRenderSide(GlassBlock block, struct Level * level, int x, int y, int z, int side);
bool GlassBlockIsOpaque(GlassBlock block);
bool GlassBlockIsSolid(GlassBlock block);
