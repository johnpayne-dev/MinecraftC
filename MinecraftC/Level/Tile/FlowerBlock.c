#include "FlowerBlock.h"
#include "MushroomBlock.h"
#include "SaplingBlock.h"
#include "../Level.h"
#include "../../Render/ShapeRenderer.h"

FlowerBlock FlowerBlockCreate(BlockType type, int textureID)
{
	Block block = BlockCreate(type, textureID);
	block->TextureID = textureID;
	BlockSetPhysics(block, true);
	float w = 0.2;
	BlockSetBounds(block, (float3){ 0.5 - w, 0.0, 0.5 - w }, (float3){ 0.5 + w, 3.0 * w, 0.5 + w });
	return block;
}

void FlowerBlockUpdate(FlowerBlock block, Level level, int x, int y, int z, RandomGenerator random)
{
	if (block->Type == BlockTypeRedMushroom || block->Type == BlockTypeBrownMushroom) { return MushroomBlockUpdate(block, level, x, y, z, random); }
	if (block->Type == BlockTypeSapling) { return SaplingBlockUpdate(block, level, x, y, z, random); }
	
	if (!level->GrowTrees)
	{
		BlockType tile = LevelGetTile(level, x, y - 1, z);
		if (!LevelIsLit(level, x, y, z) || (tile != BlockTypeDirt && tile != BlockTypeGrass))
		{
			LevelSetTile(level, x, y, z, BlockTypeNone);
		}
	}
}

AABB FlowerBlockGetCollisionAABB(FlowerBlock block, int x, int y, int z)
{
	return AABBNull;
}

bool FlowerBlockIsOpaque(FlowerBlock block)
{
	return false;
}

bool FlowerBlockIsSolid(FlowerBlock block)
{
	return false;
}

static void Render(FlowerBlock block, float3 v0)
{
	int tex = BlockGetTextureID(block, 15);
	int2 uv0 = (int2){ tex % 16, tex / 16 } << 4;
	float2 uv1 = float2i(uv0) / 256.0;
	float2 uv2 = (float2i(uv0) + 15.99) / 256.0;
	
	for (int i = 0; i < 2; i++)
	{
		float2 sc = (float2){ sin(i * pi / 2.0 + pi / 4.0), cos(i * pi / 2.0 + pi / 4.0) } * 0.5;
		float3 v1 = v0 + (float3){ 0.5 - sc.x, 1.0, 0.5 - sc.y };
		sc += v0.xz + 0.5;
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ sc.x, v1.y, sc.y }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ sc.x, v0.y, sc.y }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv2.x, uv2.y });
		ShapeRendererVertexUV((float3){ sc.x, v1.y, sc.y }, (float2){ uv2.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v1.y, v1.z }, (float2){ uv1.x, uv1.y });
		ShapeRendererVertexUV((float3){ v1.x, v0.y, v1.z }, (float2){ uv1.x, uv2.y });
		ShapeRendererVertexUV((float3){ sc.x, v0.y, sc.y }, (float2){ uv2.x, uv2.y });
	}
}

void FlowerBlockRenderPreview(FlowerBlock block)
{
	ShapeRendererNormal(up3f);
	ShapeRendererBegin();
	Render(block, (float3){ 0.0, 0.4, -0.3 });
	ShapeRendererEnd();
}

bool FlowerBlockIsCube(FlowerBlock block)
{
	return false;
}

bool FlowerBlockRender(FlowerBlock block, Level level, int x, int y, int z)
{
	float brightness = LevelGetBrightness(level, x, y, z);
	ShapeRendererColor(one3f * brightness);
	Render(block, (float3){ x, y, z });
	return true;
}

void FlowerBlockRenderFullBrightness(FlowerBlock block)
{
	ShapeRendererColor(one3f);
	Render(block, right3f * -2.0);
}
