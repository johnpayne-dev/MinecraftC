#include "Block.h"
#include "StoneBlock.h"
#include "GrassBlock.h"
#include "SaplingBlock.h"
#include "LiquidBlock.h"
#include "StillLiquidBlock.h"
#include "SandBlock.h"
#include "OreBlock.h"
#include "WoodBlock.h"
#include "LeavesBlock.h"
#include "SpongeBlock.h"
#include "GlassBlock.h"
#include "FlowerBlock.h"
#include "MushroomBlock.h"
#include "MetalBlock.h"
#include "SlabBlock.h"
#include "BookshelfBlock.h"
#include "TNTBlock.h"
#include "../../Level/Level.h"
#include "../../Render/ShapeRenderer.h"
#include "../../Particle/TerrainParticle.h"
#include "../../Item/Item.h"

struct Blocks Blocks = { 0 };

void BlocksInitialize()
{
	BlockSetData(StoneBlockCreate(1), TileSounds.Stone, 1.0, 1.0, 1.0);
	BlockSetData(GrassBlockCreate(), TileSounds.Grass, 0.9, 1.0, 0.6);
	BlockSetData(BlockCreate(BlockTypeDirt, 2), TileSounds.Grass, 0.8, 1.0, 0.5);
	BlockSetData(BlockCreate(BlockTypeCobbleStone, 16), TileSounds.Stone, 1.0, 1.0, 1.5);
	BlockSetData(BlockCreate(BlockTypeWood, 4), TileSounds.Wood, 1.0, 1.0, 1.5);
	BlockSetData(SaplingBlockCreate(), TileSounds.None, 0.7, 1.0, 0.0);
	BlockSetData(BlockCreate(BlockTypeBedrock, 17), TileSounds.Stone, 1.0, 1.0, 999.0);
	BlockSetData(LiquidBlockCreate(BlockTypeWater, LiquidTypeWater), TileSounds.None, 1.0, 1.0, 100.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillWater, LiquidTypeWater), TileSounds.None, 1.0, 1.0, 100.0);
	BlockSetData(LiquidBlockCreate(BlockTypeLava, LiquidTypeLava), TileSounds.None, 1.0, 1.0, 100.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillLava, LiquidTypeLava), TileSounds.None, 1.0, 1.0, 100.0);
	BlockSetData(SandBlockCreate(BlockTypeSand, 18), TileSounds.Gravel, 0.8, 1.0, 0.5);
	BlockSetData(SandBlockCreate(BlockTypeGravel, 19), TileSounds.Gravel, 0.8, 1.0, 0.6);
	BlockSetData(OreBlockCreate(BlockTypeGoldOre, 32), TileSounds.Stone, 1.0, 1.0, 3.0);
	BlockSetData(OreBlockCreate(BlockTypeIronOre, 33), TileSounds.Stone, 1.0, 1.0, 3.0);
	BlockSetData(OreBlockCreate(BlockTypeCoalOre, 34), TileSounds.Stone, 1.0, 1.0, 3.0);
	BlockSetData(WoodBlockCreate(), TileSounds.Wood, 1.0, 1.0, 2.5);
	BlockSetData(LeavesBlockCreate(), TileSounds.Grass, 1.0, 0.4, 0.2);
	BlockSetData(SpongeBlockCreate(), TileSounds.Cloth, 1.0, 0.9, 0.6);
	BlockSetData(GlassBlockCreate(), TileSounds.Metal, 1.0, 1.0, 0.3);
	BlockSetData(BlockCreate(BlockTypeRedWool, 64), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeOrangeWool, 65), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeYellowWool, 66), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeLimeWool, 67), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeGreenWool, 68), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeAquaGreenWool, 69), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeCyanWool, 70), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeBlueWool, 71), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypePurpleWool, 72), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeIndigoWool, 73), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeVioletWool, 74), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeMagentaWool, 75), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypePinkWool, 76), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeBlackWool, 77), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeGrayWool, 78), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeWhiteWool, 79), TileSounds.Cloth, 1.0, 1.0, 0.8);
	BlockSetData(FlowerBlockCreate(BlockTypeDandelion, 13), TileSounds.None, 0.7, 1.0, 0.0);
	BlockSetData(FlowerBlockCreate(BlockTypeRose, 12), TileSounds.None, 0.7, 1.0, 0.0);
	BlockSetData(MushroomBlockCreate(BlockTypeBrownMushroom, 29), TileSounds.None, 0.7, 1.0, 0.0);
	BlockSetData(MushroomBlockCreate(BlockTypeRedMushroom, 28), TileSounds.None, 0.8, 1.0, 0.0);
	BlockSetData(MetalBlockCreate(BlockTypeGold, 40), TileSounds.Metal, 0.7, 1.0, 3.0);
	BlockSetData(MetalBlockCreate(BlockTypeIron, 39), TileSounds.Metal, 0.7, 1.0, 5.0);
	BlockSetData(SlabBlockCreate(BlockTypeDoubleSlab, true), TileSounds.Stone, 1.0, 1.0, 2.0);
	BlockSetData(SlabBlockCreate(BlockTypeSlab, false), TileSounds.Stone, 1.0, 1.0, 2.0);
	BlockSetData(BlockCreate(BlockTypeBrick, 7), TileSounds.Stone, 1.0, 1.0, 2.0);
	BlockSetData(TNTBlockCreate(), TileSounds.Cloth, 1.0, 1.0, 0.0);
	BlockSetData(BookshelfBlockCreate(), TileSounds.Wood, 1.0, 1.0, 1.5);
	BlockSetData(BlockCreate(BlockTypeMossyCobbleStone, 36), TileSounds.Stone, 1.0, 1.0, 1.0);
	BlockSetData(BlockCreate(BlockTypeObsidian, 37), TileSounds.Stone, 1.0, 1.0, 10.0);
}

void BlocksDeinitialize()
{
	for (int i = 0; i < 256; i++) { if (Blocks.Table[i] != NULL) { BlockDestroy(Blocks.Table[i]); } }
}

Block BlockCreate(BlockType type, int textureID)
{
	Block block = MemoryAllocate(sizeof(struct Block));
	*block = (struct Block)
	{
		.Type = type,
		.TextureID = textureID,
		.Explodable = true,
	};
	Blocks.Table[type] = block;
	BlockSetBounds(block, zero3f, one3f);
	Blocks.Opaque[type] = BlockIsSolid(block);
	Blocks.Cube[type] = BlockIsCube(block);
	Blocks.Liquid[type] = false;
	return block;
}

static bool IsFlowerBlock(BlockType type) { return type == BlockTypeSapling || type == BlockTypeRose || type == BlockTypeDandelion || type == BlockTypeBrownMushroom || type == BlockTypeRedMushroom; }
static bool IsLiquidBlock(BlockType type) { return Blocks.Liquid[type]; }
static bool IsMetalBlock(BlockType type) { return type == BlockTypeGold || type == BlockTypeIron; }
static bool IsOreBlock(BlockType type) { return type == BlockTypeGoldOre || type == BlockTypeIronOre || type == BlockTypeCoalOre; }
static bool IsSandBlock(BlockType type) { return type == BlockTypeSand || type == BlockTypeGravel; }
static bool IsSlabBlock(BlockType type) { return type == BlockTypeSlab || type == BlockTypeDoubleSlab; }

bool BlockIsCube(Block block)
{
	if (IsFlowerBlock(block->Type)) { return FlowerBlockIsCube(block); }
	if (IsLiquidBlock(block->Type)) { return LiquidBlockIsCube(block); }
	if (IsSlabBlock(block->Type)) { return SlabBlockIsCube(block); }
	return true;
}

Block BlockSetData(Block block, TileSound sound, float var, float particleGravity, float hardness)
{
	block->ParticleGravity = particleGravity;
	block->Sound = sound;
	block->Hardness = hardness * 20.0;
	return block;
}

void BlockSetPhysics(Block block, bool physics)
{
	Blocks.Physics[block->Type] = physics;
}

void BlockSetBounds(Block block, float3 v0, float3 v1)
{
	block->XYZ0 = v0;
	block->XYZ1 = v1;
}

void BlockSetTickDelay(Block block, int tickDelay)
{
	Blocks.TickDelay[block->Type] = tickDelay;
}

void BlockRenderFullBrightness(Block block)
{
	if (IsFlowerBlock(block->Type)) { FlowerBlockRenderFullBrightness(block); return; }
	
	ShapeRendererColor(one3f * 0.5);
	BlockRenderInside(block, -2, 0, 0, 0);
	ShapeRendererColor(one3f);
	BlockRenderInside(block, -2, 0, 0, 1);
	ShapeRendererColor(one3f * 0.8);
	BlockRenderInside(block, -2, 0, 0, 2);
	ShapeRendererColor(one3f * 0.8);
	BlockRenderInside(block, -2, 0, 0, 3);
	ShapeRendererColor(one3f * 0.6);
	BlockRenderInside(block, -2, 0, 0, 4);
	ShapeRendererColor(one3f * 0.6);
	BlockRenderInside(block, -2, 0, 0, 5);
}

float BlockGetBrightness(Block block, Level level, int x, int y, int z)
{
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetBrightness(block, level, x, y, z); }
	return LevelGetBrightness(level, x, y, z);
}

bool BlockCanRenderSide(Block block, Level level, int x, int y, int z, int side)
{
	if (block->Type == BlockTypeGlass) { return GlassBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->Type == BlockTypeLeaves) { return LeavesBlockCanRenderSide(block, level, x, y, z, side); }
	if (IsLiquidBlock(block->Type)) { return LiquidBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->Type == BlockTypeSlab) { return SlabBlockCanRenderSide(block, level, x, y, z, side); }
	return !LevelIsSolidTile(level, x, y, z);
}

int BlockGetTextureID(Block block, int face)
{
	if (block->Type == BlockTypeBookshelf) { return BookshelfBlockGetTextureID(block, face); }
	if (block->Type == BlockTypeGrass) { return GrassBlockGetTextureID(block, face); }
	if (IsMetalBlock(block->Type)) { return MetalBlockGetTextureID(block, face); }
	if (IsSlabBlock(block->Type)) { return SlabBlockGetTextureID(block, face); }
	if (block->Type == BlockTypeTNT) { return TNTBlockGetTextureID(block, face); }
	if (block->Type == BlockTypeLog) { return WoodBlockGetTextureID(block, face); }
	return block->TextureID;
}

void BlockRenderInside(Block block, int x, int y, int z, int side)
{
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
	if (IsLiquidBlock(block->Type)) { LiquidBlockRenderInside(block, x, y, z, side); }
}

void BlockRenderSideWithTexture(Block block, int x, int y, int z, int side, int tex)
{
	int2 uv0 = (int2){ tex % 16, tex / 16 } << 4;
	float2 uv1 = float2i(uv0) / 256.0;
	float2 uv2 = (float2i(uv0) + 15.99) / 256.0;
	if (side >= 2 && tex < 240)
	{
		if (block->XYZ0.y >= 0.0 && block->XYZ1.y <= 1.0)
		{
			uv1.y = (uv0.y + block->XYZ0.y * 15.99) / 256.0;
			uv2.y = (uv0.y + block->XYZ1.y * 15.99) / 256.0;
		}
		else
		{
			uv1.y = uv0.y / 256.0;
			uv2.y = (uv0.y + 15.99) / 256.0;
		}
	}
	float3 v0 = (float3){ x, y, z } + block->XYZ0;
	float3 v1 = (float3){ x, y, z } + block->XYZ1;
	
	if (side == 0)
	{
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 1)
	{
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv1.x, uv2.y });
	}
	if (side == 2)
	{
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 3)
	{
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv2.x, uv1.y });
	}
	if (side == 4)
	{
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 5)
	{
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv1.x, uv1.y });
	}
}

void BlockRenderSide(Block block, int x, int y, int z, int side)
{
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

AABB BlockGetSelectionAABB(Block block, int x, int y, int z)
{
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetSelectionAABB(block, x, y, z); }
	return (AABB){ .V0 = (float3){ x, y, z } + block->XYZ0, .V1 = (float3){ x, y, z } + block->XYZ1 };
}

AABB BlockGetCollisionAABB(Block block, int x, int y, int z)
{
	if (IsFlowerBlock(block->Type)) { return FlowerBlockGetCollisionAABB(block, x, y, z); }
	if (IsLiquidBlock(block->Type)) { return AABBNull; }
	return (AABB){ .V0 = (float3){ x, y, z } + block->XYZ0, .V1 = (float3){ x, y, z } + block->XYZ1 };
}

bool BlockIsOpaque(Block block)
{
	if (IsFlowerBlock(block->Type)) { return FlowerBlockIsOpaque(block); }
	if (block->Type == BlockTypeGlass) { return GlassBlockIsOpaque(block); }
	if (block->Type == BlockTypeLeaves) { return LeavesBlockIsOpaque(block); }
	if (IsLiquidBlock(block->Type)) { return LiquidBlockIsOpaque(block); }
	return true;
}

bool BlockIsSolid(Block block)
{
	if (IsFlowerBlock(block->Type)) { return FlowerBlockIsSolid(block); }
	if (block->Type == BlockTypeGlass) { return GlassBlockIsSolid(block); }
	if (block->Type == BlockTypeLeaves) { return LeavesBlockIsSolid(block); }
	if (IsLiquidBlock(block->Type)) { return LiquidBlockIsSolid(block); }
	if (IsSlabBlock(block->Type)) { return SlabBlockIsSolid(block); }
	return true;
}

void BlockUpdate(Block block, Level level, int x, int y, int z, RandomGenerator random)
{
	if (IsFlowerBlock(block->Type)) { FlowerBlockUpdate(block, level, x, y, z, random); return; }
	if (block->Type == BlockTypeGrass) { GrassBlockUpdate(block, level, x, y, z, random); return; }
	if (IsLiquidBlock(block->Type)) { LiquidBlockUpdate(block, level, x, y, z, random); return; }
}

void BlockSpawnBreakParticles(Block block, Level level, int x, int y, int z, ParticleManager particles)
{
	if (block->Type == BlockTypeTNT)
	{
		TNTBlockSpawnBreakParticles(block, level, x, y, z, particles);
		if (!level->CreativeMode) { return; }
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				float3 v = (float3){ x, y, z } + ((float3){ i, j, k } + 0.5) / 4.0;
				ParticleManagerSpawnParticle(particles, TerrainParticleCreate(level, v, v - (float3){ x, y, z } - 0.5, block));
			}
		}
	}
}

void BlockSpawnBlockParticles(Block block, Level level, int x, int y, int z, int side, ParticleManager particles)
{
	float f = 0.1;
	float3 v = (float3){ x, y, z } + RandomUniform() * (block->XYZ1 - block->XYZ0 - f * 2.0) + f + block->XYZ0;
	if (side == 0) { v.y = y + block->XYZ0.y - f; }
	if (side == 1) { v.y = y + block->XYZ1.y + f; }
	if (side == 2) { v.z = z + block->XYZ0.z - f; }
	if (side == 3) { v.z = z + block->XYZ1.z + f; }
	if (side == 4) { v.x = x + block->XYZ0.x - f; }
	if (side == 5) { v.x = x + block->XYZ1.x + f; }
	ParticleManagerSpawnParticle(particles, ParticleScale(ParticleSetPower(TerrainParticleCreate(level, v, 0.0, block), 0.2), 0.6));
}

LiquidType BlockGetLiquidType(Block block)
{
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetLiquidType(block); }
	return LiquidTypeNone;
}

void BlockOnNeighborChanged(Block block, Level level, int x, int y, int z, BlockType tile)
{
	if (IsLiquidBlock(block->Type)) { LiquidBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSandBlock(block->Type)) { SandBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSlabBlock(block->Type)) { SlabBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
}

void BlockOnPlaced(Block block, Level level, int x, int y, int z)
{
	if (IsLiquidBlock(block->Type)) { LiquidBlockOnPlaced(block, level, x, y, z); return; }
	if (IsSandBlock(block->Type)) { SandBlockOnPlaced(block, level, x, y, z); return; }
}

int BlockGetTickDelay(Block block)
{
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetTickDelay(block); }
	return 0;
}

void BlockOnAdded(Block block, Level level, int x, int y, int z)
{
	if (IsSlabBlock(block->Type)) { SlabBlockOnAdded(block, level, x, y, z); return; }
	if (block->Type == BlockTypeSponge) { SpongeBlockOnAdded(block, level, x, y, z); return; }
}

void BlockOnRemoved(Block block, Level level, int x, int y, int z)
{
	if (block->Type == BlockTypeSponge) { SpongeBlockOnRemoved(block, level, x, y, z); return; }
}

int BlockGetDropCount(Block block)
{
	if (block->Type == BlockTypeBookshelf) { return BookshelfBlockGetDropCount(block); }
	if (block->Type == BlockTypeLeaves) { return LeavesBlockGetDropCount(block); }
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetDropCount(block); }
	if (IsOreBlock(block->Type)) { return OreBlockGetDropCount(block); }
	if (block->Type == BlockTypeTNT) { return TNTBlockGetDropCount(block); }
	if (block->Type == BlockTypeLog) { return WoodBlockGetDropCount(block); }
	return 1;
}

BlockType BlockGetDrop(Block block)
{
	if (block->Type == BlockTypeGrass) { return GrassBlockGetDrop(block); }
	if (block->Type == BlockTypeLeaves) { return LeavesBlockGetDrop(block); }
	if (IsOreBlock(block->Type)) { return OreBlockGetDrop(block); }
	if (IsSlabBlock(block->Type)) { return SlabBlockGetDrop(block); }
	if (block->Type == BlockTypeStone) { return StoneBlockGetDrop(block); }
	if (block->Type == BlockTypeLog) { return WoodBlockGetDrop(block); }
	return block->Type;
}

int BlockGetHardness(Block block)
{
	return block->Hardness;
}

void BlockOnBreak(Block block, Level level, int x, int y, int z)
{
	if (IsLiquidBlock(block->Type)) { LiquidBlockOnBreak(block, level, x, y, z); return; }
	BlockDropItems(block, level, x, y, z, 1.0);
}

void BlockDropItems(Block block, Level level, int x, int y, int z, float probability)
{
	if (IsLiquidBlock(block->Type)) { LiquidBlockDropItems(block, level, x, y, z, probability); return; }
	
	if (!level->CreativeMode)
	{
		int count = BlockGetDropCount(block);
		for (int i = 0; i < count; i++)
		{
			if (RandomUniform() <= probability)
			{
				float range = 0.7;
				float3 v = (float3){ RandomUniform(), RandomUniform(), RandomUniform() } * range + (1.0 - range) * 0.5;
				LevelAddEntity(level, ItemCreate(level, (float3){ x, y, z } + v, BlockGetDrop(block)));
			}
		}
	}
}

void BlockRenderPreview(Block block)
{
	if (IsFlowerBlock(block->Type)) { FlowerBlockRenderPreview(block); return; }
	ShapeRendererBegin();
	for (int i = 0; i < 6; i++)
	{
		if (i == 0) { ShapeRendererNormal(up3f); }
		if (i == 1) { ShapeRendererNormal(-up3f); }
		if (i == 2) { ShapeRendererNormal(forward3f); }
		if (i == 3) { ShapeRendererNormal(-forward3f); }
		if (i == 4) { ShapeRendererNormal(right3f); }
		if (i == 5) { ShapeRendererNormal(-right3f); }
		BlockRenderInside(block, 0, 0, 0, i);
	}
	ShapeRendererEnd();
}

bool BlockCanExplode(Block block)
{
	return block->Explodable;
}

MovingObjectPosition BlockClip(Block block, int x, int y, int z, float3 v1, float3 v2)
{
	MovingObjectPosition pos = AABBClip((AABB){ block->XYZ0, block->XYZ1 }, v1 - (float3){ x, y, z }, v2 - (float3){ x, y, z });
	if (!pos.Null)
	{
		pos.XYZ = (int3){ x, y, z };
		pos.Vector += float3i(pos.XYZ);
	}
	return pos;
}

void BlockExplode(Block block, Level level, int x, int y, int z)
{
	if (block->Type == BlockTypeTNT) { TNTBlockExplode(block, level, x, y, z); return; }
}

bool BlockRender(Block block, Level level, int x, int y, int z)
{
	if (IsFlowerBlock(block->Type)) { return FlowerBlockRender(block, level, x, y, z); }
	
	bool rendered = false;
	float3 col = { 0.5, 0.8, 0.6 };
	if (BlockCanRenderSide(block, level, x, y - 1, z, 0))
	{
		float brightness = BlockGetBrightness(block, level, x, y - 1, z);
		ShapeRendererColor(one3f * col.x * brightness);
		BlockRenderInside(block, x, y, z, 0);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y + 1, z, 1))
	{
		float brightness = BlockGetBrightness(block, level, x, y + 1, z);
		ShapeRendererColor(one3f * brightness);
		BlockRenderInside(block, x, y, z, 1);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z - 1, 2))
	{
		float brightness = BlockGetBrightness(block, level, x, y, z - 1);
		ShapeRendererColor(one3f * col.y * brightness);
		BlockRenderInside(block, x, y, z, 2);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z + 1, 3))
	{
		float brightness = BlockGetBrightness(block, level, x, y, z + 1);
		ShapeRendererColor(one3f * col.y * brightness);
		BlockRenderInside(block, x, y, z, 3);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x - 1, y, z, 4))
	{
		float brightness = BlockGetBrightness(block, level, x - 1, y, z);
		ShapeRendererColor(one3f * col.z * brightness);
		BlockRenderInside(block, x, y, z, 4);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x + 1, y, z, 5))
	{
		float brightness = BlockGetBrightness(block, level, x + 1, y, z);
		ShapeRendererColor(one3f * col.z * brightness);
		BlockRenderInside(block, x, y, z, 5);
		rendered = true;
	}
	return rendered;
}

int BlockGetRenderPass(Block block)
{
	if (IsLiquidBlock(block->Type)) { return LiquidBlockGetRenderPass(block); }
	return 0;
}

void BlockDestroy(Block block)
{
	if (IsLiquidBlock(block->Type)) { LiquidBlockDestroy(block); }
	if (IsSlabBlock(block->Type)) { SlabBlockDestroy(block); }
	MemoryFree(block);
}
