#if MINECRAFTC_MODS

#include "Octree.h"
#include "../Level/Level.h"

static inline int pow2i(int n) {
	if (n < 0) { return 0; }
	int p = 1;
	while (n-- > 0) { p *= 2; }
	return p;
}

static inline int log2i(int p) {
	int n = 0;
	while (p > 1) {
		n++;
		p /= 2;
	}
	return n;
}

static inline int maxi(int a, int b) {
	return a < b ? b : a;
}

void OctreeCreate(Octree * tree, struct Level * level) {
	*tree = (Octree) {
		.level = level,
		.depth = log2i(maxi(level->width, maxi(level->height, level->depth))),
	};
	// 1 + 8 + 64 + ... + 8^(n-1)
	//  = (8^n - 1) / 7
	//  = (8^log2(size) - 1) / 7
	//  = (size^3 - 1) / 7
	tree->size = pow2i(tree->depth);
	tree->maskCount = (tree->size * tree->size * tree->size - 1) / 7;
	tree->masks = calloc(tree->maskCount, 1);
}

static bool Reset(Octree * tree, int startIndex, int maskOffset, int baseX, int baseY, int baseZ, int size, int depth, int x, int y, int z) {
	uint8_t bit = (x >= baseX + size / 2) + 2 * (y >= baseY + size / 2) + 4 * (z >= baseZ + size / 2);
	if (((tree->masks[startIndex + maskOffset] >> bit) & 1) == 0) {
		return false;
	}
	if (depth == tree->depth - 1) {
		LevelSetTile(tree->level, x, y, z, BlockTypeNone);
		tree->masks[startIndex + maskOffset] &= ~(1 << bit);
		return tree->masks[startIndex + maskOffset] == 0;
	}
	int ix = ((bit >> 0) & 1) * (size / 2);
	int iy = ((bit >> 1) & 1) * (size / 2);
	int iz = ((bit >> 2) & 1) * (size / 2);
	if (Reset(tree, startIndex + pow2i(3 * depth), 8 * maskOffset + bit, baseX + ix, baseY + iy, baseZ + iz, size / 2, depth + 1, x, y, z)) {
		tree->masks[startIndex + maskOffset] &= ~(1 << bit);
		return tree->masks[startIndex + maskOffset] == 0;
	}
	return false;
}

void OctreeSet(Octree * tree, int x, int y, int z, BlockType tile) {
	if (x < 0 || y < 0 || z < 0 || x >= tree->size || y >= tree->size || z >= tree->size) {
		return;
	}
	int startIndex = 0, maskOffset = 0;
	int baseX = 0, baseY = 0, baseZ = 0;
	int size = tree->size;
	if (tile == BlockTypeNone) {
		Reset(tree, startIndex, maskOffset, baseX, baseY, baseZ, size, 0, x, y, z);
	} else {
		for (int depth = 0; depth < tree->depth; depth++) {
			uint8_t bit = (x >= baseX + size / 2) + 2 * (y >= baseY + size / 2) + 4 * (z >= baseZ + size / 2);
			tree->masks[startIndex + maskOffset] |= 1 << bit;
			maskOffset = 8 * maskOffset + bit;
			startIndex += pow2i(3 * depth);
			baseX += ((bit >> 0) & 1) * (size / 2);
			baseY += ((bit >> 1) & 1) * (size / 2);
			baseZ += ((bit >> 2) & 1) * (size / 2);
			size /= 2;
		}
	}
}

BlockType OctreeGet(Octree * tree, int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x >= tree->size || y >= tree->size || z >= tree->size) {
		return;
	}
	int startIndex = 0, maskOffset = 0;
	int baseX = 0, baseY = 0, baseZ = 0;
	int size = tree->size;
	for (int depth = 0; depth < tree->depth; depth++) {
		uint8_t mask = tree->masks[startIndex + maskOffset];
		uint8_t bit = (x >= baseX + size / 2) + 2 * (y >= baseY + size / 2) + 4 * (z >= baseZ + size / 2);
		if (((mask >> bit) & 1) == 0) {
			return BlockTypeNone;
		}
		maskOffset = 8 * maskOffset + bit;
		startIndex += pow2i(3 * depth);
		baseX += ((bit >> 0) & 1) * (size / 2);
		baseY += ((bit >> 1) & 1) * (size / 2);
		baseZ += ((bit >> 2) & 1) * (size / 2);
		size /= 2;
		if (depth == tree->depth - 1) {
			return LevelGetTile(tree->level, x, y, z);
		}
	}
	return BlockTypeNone;
}

void OctreeDestroy(Octree * tree) {
	free(tree->masks);
}

#endif
