#include "Block.h"
#include "GrassBlock.h"
#include "SaplingBlock.h"
#include "LiquidBlock.h"
#include "StillLiquidBlock.h"
#include "SandBlock.h"
#include "LogBlock.h"
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
	BlockCreate(&Blocks.table[BlockTypeStone], BlockTypeStone, 1, TileSounds.stone, 1.0);
	GrassBlockCreate(&Blocks.table[BlockTypeGrass], TileSounds.grass, 1.0);
	BlockCreate(&Blocks.table[BlockTypeDirt], BlockTypeDirt, 2, TileSounds.grass, 1.0);
	BlockCreate(&Blocks.table[BlockTypeCobbleStone], BlockTypeCobbleStone, 16, TileSounds.stone, 1.0);
	BlockCreate(&Blocks.table[BlockTypeWood], BlockTypeWood, 4, TileSounds.wood, 1.0);
	SaplingBlockCreate(&Blocks.table[BlockTypeSapling], TileSounds.none, 1.0);
	BlockCreate(&Blocks.table[BlockTypeBedrock], BlockTypeBedrock, 17, TileSounds.stone, 1.0);
	LiquidBlockCreate(&Blocks.table[BlockTypeWater], BlockTypeWater, LiquidTypeWater, TileSounds.none, 1.0);
	StillLiquidBlockCreate(&Blocks.table[BlockTypeStillWater], BlockTypeStillWater, LiquidTypeWater, TileSounds.none, 1.0);
	LiquidBlockCreate(&Blocks.table[BlockTypeLava], BlockTypeLava, LiquidTypeLava, TileSounds.none, 1.0);
	StillLiquidBlockCreate(&Blocks.table[BlockTypeStillLava], BlockTypeStillLava, LiquidTypeLava, TileSounds.none, 1.0);
	SandBlockCreate(&Blocks.table[BlockTypeSand], BlockTypeSand, 18, TileSounds.gravel, 1.0);
	SandBlockCreate(&Blocks.table[BlockTypeGravel], BlockTypeGravel, 19, TileSounds.gravel, 1.0);
	BlockCreate(&Blocks.table[BlockTypeGoldOre], BlockTypeGoldOre, 32, TileSounds.stone, 1.0);
	BlockCreate(&Blocks.table[BlockTypeIronOre], BlockTypeIronOre, 33, TileSounds.stone, 1.0);
	BlockCreate(&Blocks.table[BlockTypeCoalOre], BlockTypeCoalOre, 34, TileSounds.stone, 1.0);
	LogBlockCreate(&Blocks.table[BlockTypeLog], TileSounds.wood, 1.0);
	LeavesBlockCreate(&Blocks.table[BlockTypeLeaves], TileSounds.grass, 0.4);
	SpongeBlockCreate(&Blocks.table[BlockTypeSponge], TileSounds.cloth, 0.9);
	GlassBlockCreate(&Blocks.table[BlockTypeGlass], TileSounds.metal, 1.0);
	BlockCreate(&Blocks.table[BlockTypeRedWool], BlockTypeRedWool, 64, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeOrangeWool], BlockTypeOrangeWool, 65, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeYellowWool], BlockTypeYellowWool, 66, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeLimeWool], BlockTypeLimeWool, 67, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeGreenWool], BlockTypeGreenWool, 68, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeAquaGreenWool], BlockTypeAquaGreenWool, 69, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeCyanWool], BlockTypeCyanWool, 70, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeBlueWool], BlockTypeBlueWool, 71, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypePurpleWool], BlockTypePurpleWool, 72, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeIndigoWool], BlockTypeIndigoWool, 73, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeVioletWool], BlockTypeVioletWool, 74, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeMagentaWool], BlockTypeMagentaWool, 75, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypePinkWool], BlockTypePinkWool, 76, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeBlackWool], BlockTypeBlackWool, 77, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeGrayWool], BlockTypeGrayWool, 78, TileSounds.cloth, 1.0);
	BlockCreate(&Blocks.table[BlockTypeWhiteWool], BlockTypeWhiteWool, 79, TileSounds.cloth, 1.0);
	FlowerBlockCreate(&Blocks.table[BlockTypeDandelion], BlockTypeDandelion, 13, TileSounds.none, 1.0);
	FlowerBlockCreate(&Blocks.table[BlockTypeRose], BlockTypeRose, 12, TileSounds.none, 1.0);
	MushroomBlockCreate(&Blocks.table[BlockTypeBrownMushroom], BlockTypeBrownMushroom, 29, TileSounds.none, 1.0);
	MushroomBlockCreate(&Blocks.table[BlockTypeRedMushroom], BlockTypeRedMushroom, 28, TileSounds.none, 1.0);
	MetalBlockCreate(&Blocks.table[BlockTypeGold], BlockTypeGold, 40, TileSounds.metal, 1.0);
	MetalBlockCreate(&Blocks.table[BlockTypeIron], BlockTypeIron, 39, TileSounds.metal, 1.0);
	SlabBlockCreate(&Blocks.table[BlockTypeDoubleSlab], BlockTypeDoubleSlab, true, TileSounds.stone, 1.0);
	SlabBlockCreate(&Blocks.table[BlockTypeSlab], BlockTypeSlab, false, TileSounds.stone, 1.0);
	BlockCreate(&Blocks.table[BlockTypeBrick], BlockTypeBrick, 7, TileSounds.stone, 1.0);
	TNTBlockCreate(&Blocks.table[BlockTypeTNT], TileSounds.cloth, 1.0);
	BookshelfBlockCreate(&Blocks.table[BlockTypeBookshelf], TileSounds.wood, 1.0);
	BlockCreate(&Blocks.table[BlockTypeMossyCobbleStone], BlockTypeMossyCobbleStone, 36, TileSounds.stone, 1.0);
	BlockCreate(&Blocks.table[BlockTypeObsidian], BlockTypeObsidian, 37, TileSounds.stone, 1.0);
}

void BlockCreate(Block * block, BlockType type, int textureID, TileSound sound, float particleGravity) {
	*block = (Block) {
		.type = type,
		.textureID = textureID,
		.explodable = true,
		.sound = sound,
		.particleGravity = particleGravity,
	};
	BlockSetBounds(block, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	Blocks.liquid[type] = false;
}

static bool IsFlowerBlock(BlockType type) { return type == BlockTypeSapling || type == BlockTypeRose || type == BlockTypeDandelion || type == BlockTypeBrownMushroom || type == BlockTypeRedMushroom; }
static bool IsLiquidBlock(BlockType type) { return Blocks.liquid[type]; }
static bool IsMetalBlock(BlockType type) { return type == BlockTypeGold || type == BlockTypeIron; }
static bool IsSandBlock(BlockType type) { return type == BlockTypeSand || type == BlockTypeGravel; }
static bool IsSlabBlock(BlockType type) { return type == BlockTypeSlab || type == BlockTypeDoubleSlab; }

bool BlockIsCube(Block * block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsCube(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsCube(block); }
	if (IsSlabBlock(block->type)) { return SlabBlockIsCube(block); }
	return true;
}

void BlockSetPhysics(Block * block, bool physics) {
	Blocks.physics[block->type] = physics;
}

void BlockSetBounds(Block * block, float x0, float y0, float z0, float x1, float y1, float z1) {
	block->x0 = x0;
	block->y0 = y0;
	block->z0 = z0;
	block->x1 = x1;
	block->y1 = y1;
	block->z1 = z1;
}

void BlockRenderFullBrightness(Block * block) {
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

float BlockGetBrightness(Block * block, Level * level, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetBrightness(block, level, x, y, z); }
	return LevelGetBrightness(level, x, y, z);
}

bool BlockCanRenderSide(Block * block, Level * level, int x, int y, int z, int side) {
	if (block->type == BlockTypeGlass) { return GlassBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockCanRenderSide(block, level, x, y, z, side); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockCanRenderSide(block, level, x, y, z, side); }
	if (block->type == BlockTypeSlab) { return SlabBlockCanRenderSide(block, level, x, y, z, side); }
	return !LevelIsSolidTile(level, x, y, z);
}

int BlockGetTextureID(Block * block, int face) {
	if (block->type == BlockTypeBookshelf) { return BookshelfBlockGetTextureID(block, face); }
	if (block->type == BlockTypeGrass) { return GrassBlockGetTextureID(block, face); }
	if (IsMetalBlock(block->type)) { return MetalBlockGetTextureID(block, face); }
	if (IsSlabBlock(block->type)) { return SlabBlockGetTextureID(block, face); }
	if (block->type == BlockTypeTNT) { return TNTBlockGetTextureID(block, face); }
	if (block->type == BlockTypeLog) { return LogBlockGetTextureID(block, face); }
	return block->textureID;
}

void BlockRenderInside(Block * block, int x, int y, int z, int side) {
	if (IsLiquidBlock(block->type)) {
		LiquidBlockRenderInside(block, x, y, z, side);
		return;
	}
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

void BlockRenderSideWithTexture(Block * block, int x, int y, int z, int side, int tex) {
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

void BlockRenderSide(Block * block, int x, int y, int z, int side) {
	BlockRenderSideWithTexture(block, x, y, z, side, BlockGetTextureID(block, side));
}

AABB BlockGetSelectionAABB(Block * block, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetSelectionAABB(block, x, y, z); }
	return (AABB){ .x0 = x + block->x0, .y0 = y + block->y0, .z0 = z + block->z0, .x1 = x + block->x1, .y1 = y + block->y1, .z1 = z + block->z1 };
}

AABB BlockGetCollisionAABB(Block * block, int x, int y, int z) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockGetCollisionAABB(block, x, y, z); }
	if (IsLiquidBlock(block->type)) { return (AABB){ .null = true }; }
	return (AABB){ .x0 = x + block->x0, .y0 = y + block->y0, .z0 = z + block->z0, .x1 = x + block->x1, .y1 = y + block->y1, .z1 = z + block->z1 };
}

bool BlockIsOpaque(Block * block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsOpaque(block); }
	if (block->type == BlockTypeGlass) { return GlassBlockIsOpaque(block); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockIsOpaque(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsOpaque(block); }
	return true;
}

bool BlockIsSolid(Block * block) {
	if (IsFlowerBlock(block->type)) { return FlowerBlockIsSolid(block); }
	if (block->type == BlockTypeGlass) { return GlassBlockIsSolid(block); }
	if (block->type == BlockTypeLeaves) { return LeavesBlockIsSolid(block); }
	if (IsLiquidBlock(block->type)) { return LiquidBlockIsSolid(block); }
	if (IsSlabBlock(block->type)) { return SlabBlockIsSolid(block); }
	return true;
}

void BlockUpdate(Block * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	if (IsFlowerBlock(block->type)) { FlowerBlockUpdate(block, level, x, y, z, random); return; }
	if (block->type == BlockTypeGrass) { GrassBlockUpdate(block, level, x, y, z, random); return; }
	if (IsLiquidBlock(block->type)) { LiquidBlockUpdate(block, level, x, y, z, random); return; }
}

void BlockSpawnBreakParticles(Block * block, Level * level, int x, int y, int z, ParticleManager * particles, GameSettings * settings) {
#if MINECRAFTC_MODS
	if (block->type == BlockTypeTNT && settings->explodingTNT) {
		TNTBlockSpawnBreakParticles(block, level, x, y, z, particles);
	}
#endif
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				float xd = x + (i + 0.5) / 4.0;
				float yd = y + (j + 0.5) / 4.0;
				float zd = z + (k + 0.5) / 4.0;
				TerrainParticle * particle = malloc(sizeof(TerrainParticle));
				TerrainParticleCreate(particle, level, xd, yd, zd, xd - x - 0.5, yd - y - 0.5, zd - z - 0.5, block);
				ParticleManagerSpawnParticle(particles, particle);
			}
		}
	}
}

LiquidType BlockGetLiquidType(Block * block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetLiquidType(block); }
	return LiquidTypeNone;
}

void BlockOnNeighborChanged(Block * block, Level * level, int x, int y, int z, BlockType tile) {
	if (IsLiquidBlock(block->type)) { LiquidBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSandBlock(block->type)) { SandBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
	if (IsSlabBlock(block->type)) { SlabBlockOnNeighborChanged(block, level, x, y, z, tile); return; }
}

void BlockOnPlaced(Block * block, Level * level, int x, int y, int z) {
	if (IsLiquidBlock(block->type)) { LiquidBlockOnPlaced(block, level, x, y, z); return; }
	if (IsSandBlock(block->type)) { SandBlockOnPlaced(block, level, x, y, z); return; }
}

int BlockGetTickDelay(Block * block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetTickDelay(block); }
	return 0;
}

void BlockOnAdded(Block * block, Level * level, int x, int y, int z) {
	if (IsSlabBlock(block->type)) { SlabBlockOnAdded(block, level, x, y, z); return; }
	if (block->type == BlockTypeSponge) { SpongeBlockOnAdded(block, level, x, y, z); return; }
}

void BlockOnRemoved(Block * block, Level * level, int x, int y, int z) {
	if (block->type == BlockTypeSponge) { SpongeBlockOnRemoved(block, level, x, y, z); return; }
}

void BlockRenderPreview(Block * block) {
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

bool BlockCanExplode(Block * block) {
	return block->explodable;
}

MovingObjectPosition BlockClip(Block * block, int x, int y, int z, Vector3D v1, Vector3D v2) {
	MovingObjectPosition pos = AABBClip((AABB){ block->x0, block->y0, block->z0, block->x1, block->y1, block->z1 }, Vector3DSubtract(v1, (Vector3D){ x, y, z }), Vector3DSubtract(v2, (Vector3D){ x, y, z }));
	if (!pos.null) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		pos.vector = Vector3DAdd(pos.vector, (Vector3D){ x, y, z });
	}
	return pos;
}

void BlockExplode(Block * block, Level * level, int x, int y, int z) {
	if (block->type == BlockTypeTNT) { TNTBlockExplode(block, level, x, y, z); return; }
}

bool BlockRender(Block * block, Level * level, int x, int y, int z) {
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

int BlockGetRenderPass(Block * block) {
	if (IsLiquidBlock(block->type)) { return LiquidBlockGetRenderPass(block); }
	return 0;
}
