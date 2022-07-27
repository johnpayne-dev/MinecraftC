#pragma once
#if MINECRAFTC_MODS

#include "../Level/Tile/Block.h"

typedef struct Octree {
	struct Level * level;
	int depth;
	int size;
	uint8_t * masks;
} Octree;

void OctreeCreate(Octree * tree, struct Level * level);
void OctreeSet(Octree * tree, int x, int y, int z, BlockType tile);
BlockType OctreeGet(Octree * tree, int x, int y, int z);
void OctreeDestroy(Octree * tree);

#endif
