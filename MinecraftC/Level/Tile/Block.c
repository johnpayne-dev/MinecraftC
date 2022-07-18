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
	BlockSetData(StoneBlockCreate(1), TileSounds.stone, 1.0);
	BlockSetData(GrassBlockCreate(), TileSounds.grass, 1.0);
	BlockSetData(BlockCreate(BlockTypeDirt, 2), TileSounds.grass, 1.0);
	BlockSetData(BlockCreate(BlockTypeCobbleStone, 16), TileSounds.stone, 1.0);
	BlockSetData(BlockCreate(BlockTypeWood, 4), TileSounds.wood, 1.0);
	BlockSetData(SaplingBlockCreate(), TileSounds.none, 1.0);
	BlockSetData(BlockCreate(BlockTypeBedrock, 17), TileSounds.stone, 1.0);
	BlockSetData(LiquidBlockCreate(BlockTypeWater, LiquidTypeWater), TileSounds.none, 1.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillWater, LiquidTypeWater), TileSounds.none, 1.0);
	BlockSetData(LiquidBlockCreate(BlockTypeLava, LiquidTypeLava), TileSounds.none, 1.0);
	BlockSetData(StillLiquidBlockCreate(BlockTypeStillLava, LiquidTypeLava), TileSounds.none, 1.0);
	BlockSetData(SandBlockCreate(BlockTypeSand, 18), TileSounds.gravel, 1.0);
	BlockSetData(SandBlockCreate(BlockTypeGravel, 19), TileSounds.gravel, 1.0);
	BlockSetData(OreBlockCreate(BlockTypeGoldOre, 32), TileSounds.stone, 1.0);
	BlockSetData(OreBlockCreate(BlockTypeIronOre, 33), TileSounds.stone, 1.0);
	BlockSetData(OreBlockCreate(BlockTypeCoalOre, 34), TileSounds.stone, 1.0);
	BlockSetData(WoodBlockCreate(), TileSounds.wood, 1.0);
	BlockSetData(LeavesBlockCreate(), TileSounds.grass, 0.4);
	BlockSetData(SpongeBlockCreate(), TileSounds.cloth, 0.9);
	BlockSetData(GlassBlockCreate(), TileSounds.metal, 1.0);
	BlockSetData(BlockCreate(BlockTypeRedWool, 64), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeOrangeWool, 65), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeYellowWool, 66), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeLimeWool, 67), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeGreenWool, 68), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeAquaGreenWool, 69), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeCyanWool, 70), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeBlueWool, 71), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypePurpleWool, 72), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeIndigoWool, 73), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeVioletWool, 74), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeMagentaWool, 75), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypePinkWool, 76), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeBlackWool, 77), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeGrayWool, 78), TileSounds.cloth, 1.0);
	BlockSetData(BlockCreate(BlockTypeWhiteWool, 79), TileSounds.cloth, 1.0);
	BlockSetData(FlowerBlockCreate(BlockTypeDandelion, 13), TileSounds.none, 1.0);
	BlockSetData(FlowerBlockCreate(BlockTypeRose, 12), TileSounds.none, 1.0);
	BlockSetData(MushroomBlockCreate(BlockTypeBrownMushroom, 29), TileSounds.none, 1.0);
	BlockSetData(MushroomBlockCreate(BlockTypeRedMushroom, 28), TileSounds.none, 1.0);
	BlockSetData(MetalBlockCreate(BlockTypeGold, 40), TileSounds.metal, 1.0);
	BlockSetData(MetalBlockCreate(BlockTypeIron, 39), TileSounds.metal, 1.0);
	BlockSetData(SlabBlockCreate(BlockTypeDoubleSlab, true), TileSounds.stone, 1.0);
	BlockSetData(SlabBlockCreate(BlockTypeSlab, false), TileSounds.stone, 1.0);
	BlockSetData(BlockCreate(BlockTypeBrick, 7), TileSounds.stone, 1.0);
	BlockSetData(TNTBlockCreate(), TileSounds.cloth, 1.0);
	BlockSetData(BookshelfBlockCreate(), TileSounds.wood, 1.0);
	BlockSetData(BlockCreate(BlockTypeMossyCobbleStone, 36), TileSounds.stone, 1.0);
	BlockSetData(BlockCreate(BlockTypeObsidian, 37), TileSounds.stone, 1.0);
}

void BlocksDeinitialize() {
	for (int i = 0; i < 256; i++) {
		if (Blocks.table[i] != NULL) { BlockDestroy(Blocks.table[i]); }
	}
}

Block BlockCreate(BlockType type, int textureID) {
	Block block = malloc(sizeof(struct Block));
	*block = (struct Block) {
		.type = type,
		.textureID = textureID,
		.explodable = true,
	};
	Blocks.table[type] = block;
	BlockSetBounds(block, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
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

Block BlockSetData(Block block, TileSound sound, float particleGravity) {
	block->particleGravity = particleGravity;
	block->sound = sound;
	return block;
}

void BlockSetPhysics(Block block, bool physics) {
	Blocks.physics[block->type] = physics;
}

void BlockSetBounds(Block block, float x0, float y0, float z0, float x1, float y1, float z1) {
	block->x0 = x0;
	block->y0 = y0;
	block->z0 = z0;
	block->x1 = x1;
	block->y1 = y1;
	block->z1 = z1;
}

void BlockRenderFullBrightness(Block block) {
	if (IsFlowerBlock(block->type)) { FlowerBlockRenderFullBrightness(block); return; }
	
	ShapeRendererColorf(0.5, 0.5, 0.5);
	BlockRenderInside(block, -2, 0, 0, 0);
	ShapeRendererColorf(1.0, 1.0, 1.0);
	BlockRenderInside(block, -2, 0, 0, 1);
	ShapeRendererColorf(0.8, 0.8, 0.8);
	BlockRenderInside(block, -2, 0, 0, 2);
	ShapeRendererColorf(0.8, 0.8, 0.8);
	BlockRenderInside(block, -2, 0, 0, 3);
	ShapeRendererColorf(0.6, 0.6, 0.6);
	BlockRenderInside(block, -2, 0, 0, 4);
	ShapeRendererColorf(0.6, 0.6, 0.6);
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
	int u0 = (tex % 16) << 4;
	int v0 = (tex / 16) << 4;
	float u1 = u0 / 256.0;
	float v1 = v0 / 256.0;
	float u2 = (u0 + 15.99) / 256.0;
	float v2 = (v0 + 15.99) / 256.0;
	if (side >= 2 && tex < 240) {
		if (block->y0 >= 0.0 && block->y1 <= 1.0) {
			v1 = (v0 + block->y0 * 15.99) / 256.0;
			v2 = (v0 + block->y1 * 15.99) / 256.0;
		} else {
			v1 = v0 / 256.0;
			v2 = (v0 + 15.99) / 256.0;
		}
	}
	float x0 = x + block->x0;
	float y0 = y + block->y0;
	float z0 = z + block->z0;
	float x1 = x + block->x1;
	float y1 = y + block->y1;
	float z1 = z + block->z1;
	
	if (side == 0) {
		ShapeRendererVertexUV(x0, y0, z1, u1, v2);
		ShapeRendererVertexUV(x0, y0, z0, u1, v1);
		ShapeRendererVertexUV(x1, y0, z0, u2, v1);
		ShapeRendererVertexUV(x1, y0, z1, u2, v2);
	}
	if (side == 1) {
		ShapeRendererVertexUV(x1, y1, z1, u2, v2);
		ShapeRendererVertexUV(x1, y1, z0, u2, v1);
		ShapeRendererVertexUV(x0, y1, z0, u1, v1);
		ShapeRendererVertexUV(x0, y1, z1, u1, v2);
	}
	if (side == 2) {
		ShapeRendererVertexUV(x0, y1, z0, u2, v1);
		ShapeRendererVertexUV(x1, y1, z0, u1, v1);
		ShapeRendererVertexUV(x1, y0, z0, u1, v2);
		ShapeRendererVertexUV(x0, y0, z0, u2, v2);
	}
	if (side == 3) {
		ShapeRendererVertexUV(x0, y1, z1, u1, v1);
		ShapeRendererVertexUV(x0, y0, z1, u1, v2);
		ShapeRendererVertexUV(x1, y0, z1, u2, v2);
		ShapeRendererVertexUV(x1, y1, z1, u2, v1);
	}
	if (side == 4) {
		ShapeRendererVertexUV(x0, y1, z1, u2, v1);
		ShapeRendererVertexUV(x0, y1, z0, u1, v1);
		ShapeRendererVertexUV(x0, y0, z0, u1, v2);
		ShapeRendererVertexUV(x0, y0, z1, u2, v2);
	}
	if (side == 5) {
		ShapeRendererVertexUV(x1, y0, z1, u1, v2);
		ShapeRendererVertexUV(x1, y0, z0, u2, v2);
		ShapeRendererVertexUV(x1, y1, z0, u2, v1);
		ShapeRendererVertexUV(x1, y1, z1, u1, v1);
	}
}

void BlockRenderSide(Block block, int x, int y, int z, int side) {
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

AABB BlockGetSelectionAABB(Block block, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetSelectionAABB(block, x, y, z); }
	return (AABB){ .x0 = x + block->x0, .y0 = y + block->y0, .z0 = z + block->z0, .x1 = x + block->x1, .y1 = y + block->y1, .z1 = z + block->z1 };
}

AABB BlockGetCollisionAABB(Block block, int x, int y, int z) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockGetCollisionAABB(block, x, y, z); }
	if (IsLiquidBlock(block->type)) { return (AABB){ .null = true }; }
	return (AABB){ .x0 = x + block->x0, .y0 = y + block->y0, .z0 = z + block->z0, .x1 = x + block->x1, .y1 = y + block->y1, .z1 = z + block->z1 };
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
				float xd = x + (i + 0.5) / 4.0;
				float yd = y + (j + 0.5) / 4.0;
				float zd = z + (k + 0.5) / 4.0;
				ParticleManagerSpawnParticle(particles, TerrainParticleCreate(level, xd, yd, zd, xd - x - 0.5, yd - y - 0.5, zd - z - 0.5, block));
			}
		}
	}
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

void BlockRenderPreview(Block block) {
	if (IsFlowerBlock(block->type)) { FlowerBlockRenderPreview(block); return; }
	ShapeRendererBegin();
	for (int i = 0; i < 6; i++) {
		if (i == 0) { ShapeRendererNormal(0.0, 1.0, 0.0); }
		if (i == 1) { ShapeRendererNormal(0.0, -1.0, 0.0); }
		if (i == 2) { ShapeRendererNormal(0.0, 0.0, 1.0); }
		if (i == 3) { ShapeRendererNormal(0.0, 0.0, -1.0); }
		if (i == 4) { ShapeRendererNormal(1.0, 0.0, 0.0); }
		if (i == 5) { ShapeRendererNormal(-1.0, 0.0, 0.0); }
		BlockRenderInside(block, 0, 0, 0, i);
	}
	ShapeRendererEnd();
}

bool BlockCanExplode(Block block) {
	return block->explodable;
}

MovingObjectPosition BlockClip(Block block, int x, int y, int z, Vector3D v1, Vector3D v2) {
	MovingObjectPosition pos = AABBClip((AABB){ block->x0, block->y0, block->z0, block->x1, block->y1, block->z1 }, Vector3DSubtract(v1, (Vector3D){ x, y, z }), Vector3DSubtract(v2, (Vector3D){ x, y, z }));
	if (!pos.null) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		pos.vector = Vector3DAdd(pos.vector, (Vector3D){ x, y, z });
	}
	return pos;
}

void BlockExplode(Block block, Level level, int x, int y, int z) {
	if (block->type == BlockTypeTNT) { TNTBlockExplode(block, level, x, y, z); return; }
}

bool BlockRender(Block block, Level level, int x, int y, int z) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockRender(block, level, x, y, z); }
	
	bool rendered = false;
	//float3 col = { 0.5, 0.8, 0.6 };
	if (BlockCanRenderSide(block, level, x, y - 1, z, 0)) {
		float brightness = BlockGetBrightness(block, level, x, y - 1, z);
		ShapeRendererColorf(0.5 * brightness, 0.5 * brightness, 0.5 * brightness);
		BlockRenderInside(block, x, y, z, 0);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y + 1, z, 1)) {
		float brightness = BlockGetBrightness(block, level, x, y + 1, z);
		ShapeRendererColorf(brightness, brightness, brightness);
		BlockRenderInside(block, x, y, z, 1);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z - 1, 2)) {
		float brightness = BlockGetBrightness(block, level, x, y, z - 1);
		ShapeRendererColorf(0.8 * brightness, 0.8 * brightness, 0.8 * brightness);
		BlockRenderInside(block, x, y, z, 2);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x, y, z + 1, 3)) {
		float brightness = BlockGetBrightness(block, level, x, y, z + 1);
		ShapeRendererColorf(0.8 * brightness, 0.8 * brightness, 0.8 * brightness);
		BlockRenderInside(block, x, y, z, 3);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x - 1, y, z, 4)) {
		float brightness = BlockGetBrightness(block, level, x - 1, y, z);
		ShapeRendererColorf(0.6 * brightness, 0.6 * brightness, 0.6 * brightness);
		BlockRenderInside(block, x, y, z, 4);
		rendered = true;
	}
	if (BlockCanRenderSide(block, level, x + 1, y, z, 5)) {
		float brightness = BlockGetBrightness(block, level, x + 1, y, z);
		ShapeRendererColorf(0.6 * brightness, 0.6 * brightness, 0.6 * brightness);
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
	free(block);
}
