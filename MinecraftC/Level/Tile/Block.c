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

struct Blocks Blocks = { 0 };

void BlocksInitialize() {
	BlockSetData(StoneBlockCreate(1), TileSounds.stone, 1.0, 1.0, 1.0);
	BlockSetData(GrassBlockCreate(), TileSounds.grass, 0.9, 1.0, 0.6);
	BlockSetData(BlockCreate(BlockTypeDirt, 2), TileSounds.grass, 0.8, 1.0, 0.5);
	BlockSetData(BlockCreate(BlockTypeCobbleStone, 16), TileSounds.stone, 1.0, 1.0, 1.5);
	BlockSetData(BlockCreate(BlockTypeWood, 4), TileSounds.wood, 1.0, 1.0, 1.5);
	BlockSetData(SaplingBlockCreate(), TileSounds.none, 0.7, 1.0, 0.0);
	BlockSetData(BlockCreate(BlockTypeBedrock, 17), TileSounds.stone, 1.0, 1.0, 999.0);
	BlockSetData(LiquidBlockCreate(BlockTypeWater, LiquidTypeWater), TileSounds.none, 1.0, 1.0, 100.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillWater, LiquidTypeWater), TileSounds.none, 1.0, 1.0, 100.0);
	BlockSetData(LiquidBlockCreate(BlockTypeLava, LiquidTypeLava), TileSounds.none, 1.0, 1.0, 100.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillLava, LiquidTypeLava), TileSounds.none, 1.0, 1.0, 100.0);
	BlockSetData(SandBlockCreate(BlockTypeSand, 18), TileSounds.gravel, 0.8, 1.0, 0.5);
	BlockSetData(SandBlockCreate(BlockTypeGravel, 19), TileSounds.gravel, 0.8, 1.0, 0.6);
	BlockSetData(OreBlockCreate(BlockTypeGoldOre, 32), TileSounds.stone, 1.0, 1.0, 3.0);
	BlockSetData(OreBlockCreate(BlockTypeIronOre, 33), TileSounds.stone, 1.0, 1.0, 3.0);
	BlockSetData(OreBlockCreate(BlockTypeCoalOre, 34), TileSounds.stone, 1.0, 1.0, 3.0);
	BlockSetData(WoodBlockCreate(), TileSounds.wood, 1.0, 1.0, 2.5);
	BlockSetData(LeavesBlockCreate(), TileSounds.grass, 1.0, 0.4, 0.2);
	BlockSetData(SpongeBlockCreate(), TileSounds.cloth, 1.0, 0.9, 0.6);
	BlockSetData(GlassBlockCreate(), TileSounds.metal, 1.0, 1.0, 0.3);
	BlockSetData(BlockCreate(BlockTypeRedWool, 64), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeOrangeWool, 65), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeYellowWool, 66), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeLimeWool, 67), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeGreenWool, 68), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeAquaGreenWool, 69), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeCyanWool, 70), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeBlueWool, 71), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypePurpleWool, 72), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeIndigoWool, 73), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeVioletWool, 74), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeMagentaWool, 75), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypePinkWool, 76), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeBlackWool, 77), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeGrayWool, 78), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(BlockCreate(BlockTypeWhiteWool, 79), TileSounds.cloth, 1.0, 1.0, 0.8);
	BlockSetData(FlowerBlockCreate(BlockTypeDandelion, 13), TileSounds.none, 0.7, 1.0, 0.0);
	BlockSetData(FlowerBlockCreate(BlockTypeRose, 12), TileSounds.none, 0.7, 1.0, 0.0);
	BlockSetData(MushroomBlockCreate(BlockTypeBrownMushroom, 29), TileSounds.none, 0.7, 1.0, 0.0);
	BlockSetData(MushroomBlockCreate(BlockTypeRedMushroom, 28), TileSounds.none, 0.8, 1.0, 0.0);
	BlockSetData(MetalBlockCreate(BlockTypeGold, 40), TileSounds.metal, 0.7, 1.0, 3.0);
	BlockSetData(MetalBlockCreate(BlockTypeIron, 39), TileSounds.metal, 0.7, 1.0, 5.0);
	BlockSetData(SlabBlockCreate(BlockTypeDoubleSlab, true), TileSounds.stone, 1.0, 1.0, 2.0);
	BlockSetData(SlabBlockCreate(BlockTypeSlab, false), TileSounds.stone, 1.0, 1.0, 2.0);
	BlockSetData(BlockCreate(BlockTypeBrick, 7), TileSounds.stone, 1.0, 1.0, 2.0);
	BlockSetData(TNTBlockCreate(), TileSounds.cloth, 1.0, 1.0, 0.0);
	BlockSetData(BookshelfBlockCreate(), TileSounds.wood, 1.0, 1.0, 1.5);
	BlockSetData(BlockCreate(BlockTypeMossyCobbleStone, 36), TileSounds.stone, 1.0, 1.0, 1.0);
	BlockSetData(BlockCreate(BlockTypeObsidian, 37), TileSounds.stone, 1.0, 1.0, 10.0);
}

void BlocksDeinitialize() {
	for (int i = 0; i < 256; i++) {
		if (Blocks.table[i] != NULL) { BlockDestroy(Blocks.table[i]); }
	}
}

Block BlockCreate(BlockType type, int textureID) {
	Block block = MemoryAllocate(sizeof(struct Block));
	*block = (struct Block) {
		.type = type,
		.textureID = textureID,
		.explodable = true,
	};
	Blocks.table[type] = block;
	BlockSetBounds(block, zero3f, one3f);
	Blocks.opaque[type] = BlockIsSolid(block);
	Blocks.cube[type] = BlockIsCube(block);
	Blocks.liquid[type] = false;
	return block;
}

static bool IsFlowerBlock(BlockType type) { return type == BlockTypeSapling || type == BlockTypeRose || type == BlockTypeDandelion || type == BlockTypeBrownMushroom || type == BlockTypeRedMushroom; }
static bool IsLiquidBlock(BlockType type) { return Blocks.liquid[type]; }
static bool IsMetalBlock(BlockType type) { return type == BlockTypeGold || type == BlockTypeIron; }
static bool IsSandBlock(BlockType type) { return type == BlockTypeSand || type == BlockTypeGravel; }
static bool IsSlabBlock(BlockType type) { return type == BlockTypeSlab || type == BlockTypeDoubleSlab; }

bool BlockIsCube(Block block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsCube(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsCube(block); }
	if (IsSlabBlock(block->type)) { return SlabBlockIsCube(block); }
	return true;
}

Block BlockSetData(Block block, TileSound sound, float var, float particleGravity, float hardness) {
	block->particleGravity = particleGravity;
	block->sound = sound;
	block->hardness = hardness * 20.0;
	return block;
}

void BlockSetPhysics(Block block, bool physics) {
	Blocks.physics[block->type] = physics;
}

void BlockSetBounds(Block block, float3 v0, float3 v1) {
	block->xyz0 = v0;
	block->xyz1 = v1;
}

void BlockSetTickDelay(Block block, int tickDelay) {
	Blocks.tickDelay[block->type] = tickDelay;
}

void BlockRenderFullBrightness(Block block) {
	if (IsFlowerBlock(block->type)) { FlowerBlockRenderFullBrightness(block); return; }
	
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

float BlockGetBrightness(Block block, Level level, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetBrightness(block, level, x, y, z); }
	return LevelGetBrightness(level, x, y, z);
}

bool BlockCanRenderSide(Block block, Level level, int x, int y, int z, int side) {
	if (block->type == BlockTypeGlass) { return GlassBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockCanRenderSide(block, level, x, y, z, side); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->type == BlockTypeSlab) { return SlabBlockCanRenderSide(block, level, x, y, z, side); }
	return !LevelIsSolidTile(level, x, y, z);
}

int BlockGetTextureID(Block block, int face) {
	if (block->type == BlockTypeBookshelf) { return BookshelfBlockGetTextureID(block, face); }
	if (block->type == BlockTypeGrass) { return GrassBlockGetTextureID(block, face); }
	if (IsMetalBlock(block->type)) { return MetalBlockGetTextureID(block, face); }
	if (IsSlabBlock(block->type)) { return SlabBlockGetTextureID(block, face); }
	if (block->type == BlockTypeTNT) { return TNTBlockGetTextureID(block, face); }
	if (block->type == BlockTypeLog) { return WoodBlockGetTextureID(block, face); }
	return block->textureID;
}

void BlockRenderInside(Block block, int x, int y, int z, int side) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockRenderInside(block, x, y, z, side); }
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

void BlockRenderSideWithTexture(Block block, int x, int y, int z, int side, int tex) {
	int2 uv0 = (int2){ tex % 16, tex / 16 } << 4;
	float2 uv1 = float2i(uv0) / 256.0;
	float2 uv2 = (float2i(uv0) + 15.99) / 256.0;
	if (side >= 2 && tex < 240) {
		if (block->xyz0.y >= 0.0 && block->xyz1.y <= 1.0) {
			uv1.y = (uv0.y + block->xyz0.y * 15.99) / 256.0;
			uv2.y = (uv0.y + block->xyz1.y * 15.99) / 256.0;
		} else {
			uv1.y = uv0.y / 256.0;
			uv2.y = (uv0.y + 15.99) / 256.0;
		}
	}
	float3 v0 = (float3){ x, y, z } + block->xyz0;
	float3 v1 = (float3){ x, y, z } + block->xyz1;
	
	if (side == 0) {
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 1) {
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv1.x, uv2.y });
	}
	if (side == 2) {
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 3) {
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv2.x, uv1.y });
	}
	if (side == 4) {
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v1.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v1.y, v0.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v0.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v0.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
	}
	if (side == 5) {
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v0.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v0.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv1.x, uv1.y });
	}
}

void BlockRenderSide(Block block, int x, int y, int z, int side) {
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

AABB BlockGetSelectionAABB(Block block, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetSelectionAABB(block, x, y, z); }
	return (AABB){ .v0 = (float3){ x, y, z } + block->xyz0, .v1 = (float3){ x, y, z } + block->xyz1 };
}

AABB BlockGetCollisionAABB(Block block, int x, int y, int z) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockGetCollisionAABB(block, x, y, z); }
	if (IsLiquidBlock(block->type)) { return AABBNull; }
	return (AABB){ .v0 = (float3){ x, y, z } + block->xyz0, .v1 = (float3){ x, y, z } + block->xyz1 };
}

bool BlockIsOpaque(Block block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsOpaque(block); }
	if (block->type == BlockTypeGlass) { return GlassBlockIsOpaque(block); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockIsOpaque(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsOpaque(block); }
	return true;
}

bool BlockIsSolid(Block block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsSolid(block); }
	if (block->type == BlockTypeGlass) { return GlassBlockIsSolid(block); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockIsSolid(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsSolid(block); }
	if (IsSlabBlock(block->type)) { return SlabBlockIsSolid(block); }
	return true;
}

void BlockUpdate(Block block, Level level, int x, int y, int z, RandomGenerator random) {
	if (IsFlowerBlock(block->type)) { FlowerBlockUpdate(block, level, x, y, z, random); return; }
	if (block->type == BlockTypeGrass) { GrassBlockUpdate(block, level, x, y, z, random); return; }
	if (IsLiquidBlock(block->type)) { LiquidBlockUpdate(block, level, x, y, z, random); return; }
}

void BlockSpawnBreakParticles(Block block, Level level, int x, int y, int z, ParticleManager particles) {
	if (block->type == BlockTypeTNT) { TNTBlockSpawnBreakParticles(block, level, x, y, z, particles); }
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				float3 v = (float3){ x, y, z } + ((float3){ i, j, k } + 0.5) / 4.0;
				ParticleManagerSpawnParticle(particles, TerrainParticleCreate(level, v, v - (float3){ x, y, z } - 0.5, block));
			}
		}
	}
}

void BlockSpawnBlockParticles(Block block, Level level, int x, int y, int z, int side, ParticleManager particles) {
	float f = 0.1;
	float3 v = (float3){ x, y, z } + RandomUniform() * (block->xyz1 - block->xyz0 - f * 2.0) + f + block->xyz0;
	if (side == 0) { v.y = y + block->xyz0.y - f; }
	if (side == 1) { v.y = y + block->xyz1.y + f; }
	if (side == 2) { v.z = z + block->xyz0.z - f; }
	if (side == 3) { v.z = z + block->xyz1.z + f; }
	if (side == 4) { v.x = x + block->xyz0.x - f; }
	if (side == 5) { v.x = x + block->xyz1.x + f; }
	ParticleManagerSpawnParticle(particles, ParticleScale(ParticleSetPower(TerrainParticleCreate(level, v, 0.0, block), 0.2), 0.6));
}

LiquidType BlockGetLiquidType(Block block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetLiquidType(block); }
	return LiquidTypeNone;
}

void BlockOnNeighborChanged(Block block, Level level, int x, int y, int z, BlockType tile) {
	if (IsLiquidBlock(block->type)) { LiquidBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSandBlock(block->type)) { SandBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSlabBlock(block->type)) { SlabBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
}

void BlockOnPlaced(Block block, Level level, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { LiquidBlockOnPlaced(block, level, x, y, z); return; }
	if (IsSandBlock(block->type)) { SandBlockOnPlaced(block, level, x, y, z); return; }
}

int BlockGetTickDelay(Block block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetTickDelay(block); }
	return 0;
}

void BlockOnAdded(Block block, Level level, int x, int y, int z) {
	if (IsSlabBlock(block->type)) { SlabBlockOnAdded(block, level, x, y, z); return; }
	if (block->type == BlockTypeSponge) { SpongeBlockOnAdded(block, level, x, y, z); return; }
}

void BlockOnRemoved(Block block, Level level, int x, int y, int z) {
	if (block->type == BlockTypeSponge) { SpongeBlockOnRemoved(block, level, x, y, z); return; }
}

int BlockGetHardness(Block block) {
	return block->hardness;
}

void BlockRenderPreview(Block block) {
	if (IsFlowerBlock(block->type)) { FlowerBlockRenderPreview(block); return; }
	ShapeRendererBegin();
	for (int i = 0; i < 6; i++) {
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

bool BlockCanExplode(Block block) {
	return block->explodable;
}

MovingObjectPosition BlockClip(Block block, int x, int y, int z, float3 v1, float3 v2) {
	MovingObjectPosition pos = AABBClip((AABB){ block->xyz0, block->xyz1 }, v1 - (float3){ x, y, z }, v2 - (float3){ x, y, z });
	if (!pos.null) {
		pos.xyz = (int3){ x, y, z };
		pos.vector += float3i(pos.xyz);
	}
	return pos;
}

void BlockExplode(Block block, Level level, int x, int y, int z) {
	if (block->type == BlockTypeTNT) { TNTBlockExplode(block, level, x, y, z); return; }
}

bool BlockRender(Block block, Level level, int x, int y, int z) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockRender(block, level, x, y, z); }
	
	bool rendered = false;
	float3 col = { 0.5, 0.8, 0.6 };
	if (BlockCanRenderSide(block, level, x, y - 1, z, 0)) {
		float brightness = BlockGetBrightness(block, level, x, y - 1, z);
		ShapeRendererColor(one3f * col.x * brightness);
		BlockRenderInside(block, x, y, z, 0);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y + 1, z, 1)) {
		float brightness = BlockGetBrightness(block, level, x, y + 1, z);
		ShapeRendererColor(one3f * brightness);
		BlockRenderInside(block, x, y, z, 1);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z - 1, 2)) {
		float brightness = BlockGetBrightness(block, level, x, y, z - 1);
		ShapeRendererColor(one3f * col.y * brightness);
		BlockRenderInside(block, x, y, z, 2);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z + 1, 3)) {
		float brightness = BlockGetBrightness(block, level, x, y, z + 1);
		ShapeRendererColor(one3f * col.y * brightness);
		BlockRenderInside(block, x, y, z, 3);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x - 1, y, z, 4)) {
		float brightness = BlockGetBrightness(block, level, x - 1, y, z);
		ShapeRendererColor(one3f * col.z * brightness);
		BlockRenderInside(block, x, y, z, 4);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x + 1, y, z, 5)) {
		float brightness = BlockGetBrightness(block, level, x + 1, y, z);
		ShapeRendererColor(one3f * col.z * brightness);
		BlockRenderInside(block, x, y, z, 5);
		rendered = true;
	}
	return rendered;
}

int BlockGetRenderPass(Block block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetRenderPass(block); }
	return 0;
}

void BlockDestroy(Block block) {
	if (IsLiquidBlock(block->type)) { LiquidBlockDestroy(block); }
	if (IsSlabBlock(block->type)) { SlabBlockDestroy(block); }
	MemoryFree(block);
}
