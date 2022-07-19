#include "LiquidBlock.h"
#include "StillLiquidBlock.h"
#include "../Level.h"

void LiquidBlockCreate(LiquidBlock * block, BlockType blockType, LiquidType liquidType, TileSound sound, float particleGravity) {
	BlockCreate(block, blockType, liquidType == LiquidTypeWater ? 14 : 30, sound, particleGravity);
	Blocks.liquid[blockType] = true;
	BlockSetBounds(block, 0.01, -0.1 + 0.01, 0.01, 1.01, 0.9 + 0.01, 1.0 + 0.01);
	BlockSetPhysics(block, true);
	block->liquid.type = liquidType;
	block->liquid.movingID = blockType;
	block->liquid.stillID = blockType + 1;
}

bool LiquidBlockIsCube(LiquidBlock * block) {
	return false;
}

void LiquidBlockOnPlaced(LiquidBlock * block, Level * level, int x, int y, int z) {
	LevelAddToNextTick(level, x, y, z, block->liquid.movingID);
}

static bool CanFlow(LiquidBlock * block, Level * level, int x, int y, int z) {
	if (block->liquid.type == LiquidTypeWater) {
		for (int i = x - 2; i <= x + 2; i++) {
			for (int j = y - 2; j <= y + 2; j++) {
				for (int k = z - 2; k <= z + 2; k++) {
					if (LevelGetTile(level, i, j, k) == BlockTypeSponge) { return false; }
				}
			}
		}
	}
	return true;
}

static bool Flow(LiquidBlock * block, Level * level, int x, int y, int z) {
	if (LevelGetTile(level, x, y, z) == BlockTypeNone) {
		if (!CanFlow(block, level, x, y, z)) { return false; }
		if (LevelSetTile(level, x, y, z, block->liquid.movingID)) { LevelAddToNextTick(level, x, y, z, block->liquid.movingID); }
	}
	return false;
}

void LiquidBlockUpdate(LiquidBlock * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	if (block->type == BlockTypeStillWater || block->type == BlockTypeStillLava) { StillLiquidBlockUpdate(block, level, x, y, z, random); return; }
	
	bool set = false;
	bool canSet = false;
	do {
		y--;
		if (LevelGetTile(level, x, y, z) != BlockTypeNone || !CanFlow(block, level, x, y, z)) { break; }
		canSet = LevelSetTile(level, x, y, z, block->liquid.movingID);
		if (canSet) { set = true; }
	} while (canSet && block->liquid.type != LiquidTypeLava);
	
	y++;
	if (block->liquid.type == LiquidTypeWater || !set) { set = set | Flow(block, level, x - 1, y, z) | Flow(block, level, x + 1, y, z) | Flow(block, level, x, y, z - 1) | Flow(block, level, x, y, z + 1); }
	
	if (!set) { LevelSetTileNoUpdate(level, x, y, z, block->liquid.stillID); }
	else { LevelAddToNextTick(level, x, y, z, block->liquid.movingID); }
}

float LiquidBlockGetBrightness(LiquidBlock * block, Level * level, int x, int y, int z) {
	return block->liquid.type == LiquidTypeLava ? 100.0 : LevelGetBrightness(level, x, y, z);
}

bool LiquidBlockCanRenderSide(LiquidBlock * block, Level * level, int x, int y, int z, int side) {
	if (x >= 0 && y >= 0 && z >= 0 && x < level->width && z < level->height) {
		BlockType tile = LevelGetTile(level, x, y, z);
		if (tile != block->liquid.movingID && tile != block->liquid.stillID) {
			if (side == 1) {
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
			} else {
				return !LevelIsSolidTile(level, x, y, z);
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void LiquidBlockRenderInside(LiquidBlock * block, int x, int y, int z, int side) {
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
	BlockRenderSide(block, x, y, z, side);
}

AABB LiquidBlockGetSelectionAABB(LiquidBlock * block, int x, int y, int z) {
	return (AABB){ .null = true };
}

bool LiquidBlockIsOpaque(LiquidBlock * block) {
	return true;
}

bool LiquidBlockIsSolid(LiquidBlock * block) {
	return false;
}

LiquidType LiquidBlockGetLiquidType(LiquidBlock * block) {
	return block->liquid.type;
}

void LiquidBlockOnNeighborChanged(LiquidBlock * block, Level * level, int x, int y, int z, BlockType tile) {
	if (block->type == BlockTypeStillWater || block->type == BlockTypeStillLava) { StillLiquidBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	
	if (tile != BlockTypeNone) {
		LiquidType liquidType = BlockGetLiquidType(&Blocks.table[tile]);
		if ((block->liquid.type == LiquidTypeWater && liquidType == LiquidTypeLava) || (liquidType == LiquidTypeWater && block->liquid.type == LiquidTypeLava)) {
			LevelSetTile(level, x, y, z, BlockTypeStone);
			return;
		}
	}
	LevelAddToNextTick(level, x, y, z, tile);
}

int LiquidBlockGetTickDelay(LiquidBlock * block) {
	return block->liquid.type == LiquidTypeLava ? 5 : 0;
}

int LiquidBlockGetRenderPass(LiquidBlock * block) {
	return block->liquid.type == LiquidTypeWater ? 1 : 0;
}
