#include "LiquidBlock.h"
#include "StillLiquidBlock.h"
#include "../Level.h"

LiquidBlock LiquidBlockCreate(BlockType blockType, LiquidType liquidType)
{
	Block block = BlockCreate(blockType, liquidType == LiquidTypeWater ? 14 : 30);
	Blocks.Liquid[blockType] = true;
	BlockSetBounds(block, (float3){ 0.0, -0.1, 0.0 } + 0.01, (float3){ 1.0, 0.9, 1.0 } + 0.01);
	BlockSetPhysics(block, true);
	LiquidBlockData liquid = MemoryAllocate(sizeof(struct LiquidBlockData));
	*liquid = (struct LiquidBlockData)
	{
		.Type = liquidType,
		.MovingID = blockType,
		.StillID = blockType + 1,
	};
	block->TypeData = liquid;
	if (liquidType == LiquidTypeLava) { BlockSetTickDelay(block, 16); }
	return block;
}

bool LiquidBlockIsCube(LiquidBlock block)
{
	return false;
}

void LiquidBlockOnPlaced(LiquidBlock block, Level level, int x, int y, int z)
{
	LevelAddToNextTick(level, x, y, z, ((LiquidBlockData)block->TypeData)->MovingID);
}

static bool CanFlow(LiquidBlock block, Level level, int x, int y, int z)
{
	if (((LiquidBlockData)block->TypeData)->Type == LiquidTypeWater)
	{
		for (int i = x - 2; i <= x + 2; i++)
		{
			for (int j = y - 2; j <= y + 2; j++)
			{
				for (int k = z - 2; k <= z + 2; k++)
				{
					if (LevelGetTile(level, i, j, k) == BlockTypeSponge) { return false; }
				}
			}
		}
	}
	return true;
}

static bool Flow(LiquidBlock block, Level level, int x, int y, int z)
{
	LiquidBlockData liquid = block->TypeData;
	if (LevelGetTile(level, x, y, z) == BlockTypeNone)
	{
		if (!CanFlow(block, level, x, y, z)) { return false; }
		if (LevelSetTile(level, x, y, z, liquid->MovingID)) { LevelAddToNextTick(level, x, y, z, liquid->MovingID); }
	}
	return false;
}

void LiquidBlockUpdate(LiquidBlock block, Level level, int x, int y, int z, RandomGenerator random)
{
	if (block->Type == BlockTypeStillWater || block->Type == BlockTypeStillLava) { StillLiquidBlockUpdate(block, level, x, y, z, random); return; }
		
	LiquidBlockData liquid = block->TypeData;
	bool set = false;
	bool canSet = false;
	do
	{
		y--;
		if (LevelGetTile(level, x, y, z) != BlockTypeNone || !CanFlow(block, level, x, y, z)) { break; }
		canSet = LevelSetTile(level, x, y, z, liquid->MovingID);
		if (canSet) { set = true; }
		
	} while (canSet && liquid->Type != LiquidTypeLava);
	
	y++;
	if (liquid->Type == LiquidTypeWater || !set) { set = set | Flow(block, level, x - 1, y, z) | Flow(block, level, x + 1, y, z) | Flow(block, level, x, y, z - 1) | Flow(block, level, x, y, z + 1); }
	
	if (!set) { LevelSetTileNoUpdate(level, x, y, z, liquid->StillID); }
	else { LevelAddToNextTick(level, x, y, z, liquid->MovingID); }
}

float LiquidBlockGetBrightness(LiquidBlock block, Level level, int x, int y, int z)
{
	return ((LiquidBlockData)block->TypeData)->Type == LiquidTypeLava ? 100.0 : LevelGetBrightness(level, x, y, z);
}

bool LiquidBlockCanRenderSide(LiquidBlock block, Level level, int x, int y, int z, int side)
{
	LiquidBlockData liquid = block->TypeData;
	if (x >= 0 && y >= 0 && z >= 0 && x < level->Width && z < level->Height)
	{
		BlockType tile = LevelGetTile(level, x, y, z);
		if (tile != liquid->MovingID && tile != liquid->StillID)
		{
			if (side == 1)
			{
				bool shouldRender = LevelGetTile(level, x, y, z) == 0;
				shouldRender |= LevelGetTile(level, x - 1, y, z) == 0;
				shouldRender |= LevelGetTile(level, x + 1, y, z) == 0;
				shouldRender |= LevelGetTile(level, x, y, z - 1) == 0;
				shouldRender |= LevelGetTile(level, x, y, z + 1) == 0;
				shouldRender |= LevelGetTile(level, x - 1, y, z - 1) == 0;
				shouldRender |= LevelGetTile(level, x + 1, y, z - 1) == 0;
				shouldRender |= LevelGetTile(level, x - 1, y, z + 1) == 0;
				shouldRender |= LevelGetTile(level, x + 1, y, z + 1) == 0;
				return  shouldRender;
			}
			else { return !LevelIsSolidTile(level, x, y, z); }
		}
		else { return false; }
	}
	else { return false; }
}

void LiquidBlockRenderInside(LiquidBlock block, int x, int y, int z, int side)
{
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
	BlockRenderSide(block, x, y, z, side);
}

AABB LiquidBlockGetSelectionAABB(LiquidBlock block, int x, int y, int z)
{
	return AABBNull;
}

bool LiquidBlockIsOpaque(LiquidBlock block)
{
	return true;
}

bool LiquidBlockIsSolid(LiquidBlock block)
{
	return false;
}

LiquidType LiquidBlockGetLiquidType(LiquidBlock block)
{
	return ((LiquidBlockData)block->TypeData)->Type;
}

void LiquidBlockOnNeighborChanged(LiquidBlock block, Level level, int x, int y, int z, BlockType tile)
{
	if (block->Type == BlockTypeStillWater || block->Type == BlockTypeStillLava) { StillLiquidBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	
	LiquidBlockData this = block->TypeData;
	if (tile != BlockTypeNone)
	{
		LiquidType liquidType = BlockGetLiquidType(Blocks.Table[tile]);
		if ((this->Type == LiquidTypeWater && liquidType == LiquidTypeLava) || (liquidType == LiquidTypeWater && this->Type == LiquidTypeLava))
		{
			LevelSetTile(level, x, y, z, BlockTypeStone);
			return;
		}
	}
	LevelAddToNextTick(level, x, y, z, tile);
}

int LiquidBlockGetTickDelay(LiquidBlock block)
{
	return ((LiquidBlockData)block->TypeData)->Type == LiquidTypeLava ? 5 : 0;
}

int LiquidBlockGetRenderPass(LiquidBlock block)
{
	return ((LiquidBlockData)block->TypeData)->Type == LiquidTypeWater ? 1 : 0;
}

void LiquidBlockDestroy(LiquidBlock block)
{
	MemoryFree(block->TypeData);
}
