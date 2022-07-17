#include "SandBlock.h"
#include "../Level.h"

SandBlock SandBlockCreate(BlockType type, int texture) {
	return BlockCreate(type, texture);
}

static void Fall(SandBlock block, Level level, int x, int y, int z) {
	int vx = x, vy = y, vz = z;
	while (true) {
		BlockType tile = LevelGetTile(level, vx, vy - 1, vz);
		LiquidType liquidTile = tile == BlockTypeNone ? LiquidTypeNone : BlockGetLiquidType(Blocks.table[tile]);
		if (!(tile == BlockTypeNone ? true : (liquidTile == LiquidTypeWater ? true : liquidTile == LiquidTypeLava)) || vy <= 0) {
			if (y != vy) {
				tile = LevelGetTile(level, vx, vy, vz);
				if (tile != BlockTypeNone && BlockGetLiquidType(Blocks.table[tile]) != LiquidTypeNone) {
					LevelSetTileNoUpdate(level, vx, vy, vz, BlockTypeNone);
				}
				LevelSwap(level, x, y, z, vx, vy, vz);
			}
			return;
		}
		vy--;
	}
}

void SandBlockOnNeighborChanged(SandBlock block, Level level, int x, int y, int z, BlockType tile) {
	Fall(block, level, x, y, z);
}

void SandBlockOnPlaced(SandBlock block, Level level, int x, int y, int z) {
	Fall(block, level, x, y, z);
}
