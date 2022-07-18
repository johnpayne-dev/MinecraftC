#include "FlowerBlock.h"
#include "MushroomBlock.h"
#include "SaplingBlock.h"
#include "../Level.h"
#include "../../Render/ShapeRenderer.h"
#include "../../Utilities/SinTable.h"

FlowerBlock FlowerBlockCreate(BlockType type, int textureID) {
	Block block = BlockCreate(type, textureID);
	block->textureID = textureID;
	BlockSetPhysics(block, true);
	float w = 0.2;
	BlockSetBounds(block, 0.5 - w, 0.0, 0.5 - w, 0.5 + w, 3.0 * w, 0.5 + w);
	return block;
}

void FlowerBlockUpdate(FlowerBlock block, Level level, int x, int y, int z, RandomGenerator random) {
	if (block->type == BlockTypeRedMushroom || block->type == BlockTypeBrownMushroom) { MushroomBlockUpdate(block, level, x, y, z, random); return; }
	if (block->type == BlockTypeSapling) { SaplingBlockUpdate(block, level, x, y, z, random); return; }
	
	BlockType tile = LevelGetTile(level, x, y - 1, z);
	if (!LevelIsLit(level, x, y, z) || (tile != BlockTypeDirt && tile != BlockTypeGrass)) {
		LevelSetTile(level, x, y, z, BlockTypeNone);
	}
}

AABB FlowerBlockGetCollisionAABB(FlowerBlock block, int x, int y, int z) {
	return (AABB){ .null = true };
}

bool FlowerBlockIsOpaque(FlowerBlock block) {
	return false;
}

bool FlowerBlockIsSolid(FlowerBlock block) {
	return false;
}

static void Render(FlowerBlock block, float x, float y, float z) {
	int tex = BlockGetTextureID(block, 15);
	int u0 = (tex % 16) << 4;
	int v0 = (tex / 16) << 4;
	float u1 = u0 / 256.0;
	float v1 = v0 / 256.0;
	float u2 = (u0 + 15.99) / 256.0;
	float v2 = (v0 + 15.99) / 256.0;
	for (int i = 0; i < 2; i++) {
		float s = tsin(i * M_PI / 2.0 + M_PI / 4.0) * 0.5;
		float c = tcos(i * M_PI / 2.0 + M_PI / 4.0) * 0.5;
		float x1 = x + 0.5 - s;
		float y1 = y + 1.0;
		float z1 = z + 0.5 - c;
		s += x + 0.5;
		c += z + 0.5;
		ShapeRendererVertexUV(x1, y1, z1, u2, v1);
		ShapeRendererVertexUV(s, y1, c, u1, v1);
		ShapeRendererVertexUV(s, y, c, u1, v2);
		ShapeRendererVertexUV(x1, y, z1, u2, v2);
		ShapeRendererVertexUV(s, y1, c, u2, v1);
		ShapeRendererVertexUV(x1, y1, z1, u1, v1);
		ShapeRendererVertexUV(x1, y, z1, u1, v2);
		ShapeRendererVertexUV(s, y, c, u2, v2);
	}
}

void FlowerBlockRenderPreview(FlowerBlock block) {
	ShapeRendererNormal(0.0, 1.0, 0.0);
	ShapeRendererBegin();
	Render(block, 0.0, 0.4, -0.3);
	ShapeRendererEnd();
}

bool FlowerBlockIsCube(FlowerBlock block) {
	return false;
}

bool FlowerBlockRender(FlowerBlock block, Level level, int x, int y, int z) {
	float brightness = LevelGetBrightness(level, x, y, z);
	ShapeRendererColorf(brightness, brightness, brightness);
	Render(block, x, y, z);
	return true;
}

void FlowerBlockRenderFullBrightness(FlowerBlock block) {
	ShapeRendererColorf(1.0, 1.0, 1.0);
	Render(block, -2.0, 0.0, 0.0);
}
