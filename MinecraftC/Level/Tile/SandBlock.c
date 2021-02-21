#include "SandBlock.h"
#include "../Level.h"

SandBlock SandBlockCreate(BlockType type, int texture)
{
	return BlockCreate(type, texture);
}

static void Fall(SandBlock block, Level level, int x, int y, int z)
{
	int3 v = { x, y, z };
	while (true)
	{
		BlockType tile = LevelGetTile(level, v.x, v.y - 1, v.z);
		LiquidType liquidTile = tile == BlockTypeNone ? LiquidTypeNone : BlockGetLiquidType(Blocks.Table[tile]);
		if (!(tile == BlockTypeNone ? true : (liquidTile == LiquidTypeWater ? true : liquidTile == LiquidTypeLava)) || v.y <= 0)
		{
			if (y != v.y)
			{
				tile = LevelGetTile(level, v.x, v.y, v.z);
				if (tile != BlockTypeNone && BlockGetLiquidType(Blocks.Table[tile]) != LiquidTypeNone)
				{
					LevelSetTileNoUpdate(level, v.x, v.y, v.z, BlockTypeNone);
				}
				LevelSwap(level, (int3){ x, y, z }, v);
			}
			return;
		}
		v.y--;
	}
}

void SandBlockOnNeighborChanged(SandBlock block, Level level, int x, int y, int z, BlockType tile)
{
	Fall(block, level, x, y, z);
}

void SandBlockOnPlaced(SandBlock block, Level level, int x, int y, int z)
{
	Fall(block, level, x, y, z);
}
