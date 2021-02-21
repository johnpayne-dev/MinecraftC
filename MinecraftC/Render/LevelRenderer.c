#include <OpenGL.h>
#include "LevelRenderer.h"
#include "ShapeRenderer.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"

LevelRenderer LevelRendererCreate(Minecraft minecraft, TextureManager textures)
{
	LevelRenderer renderer = MemoryAllocate(sizeof(struct LevelRenderer));
	*renderer = (struct LevelRenderer)
	{
		.Chunks = ListCreate(sizeof(Chunk)),
		.ChunkDataCache = MemoryAllocate(65536 * sizeof(int)),
		.Ticks = 0,
		.LastLoad = one3f * -9999.0,
		.Minecraft = minecraft,
		.Textures = textures,
		.ListID = glGenLists(2),
		.BaseListID = glGenLists(4096 << 6 << 1),
	};
	return renderer;
}

void LevelRendererRefresh(LevelRenderer renderer)
{
	if (renderer->ChunkCache != NULL)
	{
		for (int i = 0; i < renderer->ChunkCacheCount; i++) { ChunkDispose(renderer->ChunkCache[i]); }
		MemoryFree(renderer->ChunkCache);
		MemoryFree(renderer->LoadQueue);
		/*renderer->Chunks = ListClear(renderer->Chunks);
		for (int i = 0; i < 65536; i++) { renderer->ChunkDataCache[i] = 0; }
		glDeleteLists(renderer->BaseListID, 4096 << 6 << 1);
		renderer->BaseListID = glGenLists(4096 << 6 << 1);*/
	}
	
	renderer->ChunkCount = (int3){ renderer->Level->Width, renderer->Level->Depth, renderer->Level->Height } / 16;
	renderer->ChunkCacheCount = renderer->ChunkCount.x * renderer->ChunkCount.y * renderer->ChunkCount.z;
	renderer->ChunkCache = MemoryAllocate(renderer->ChunkCacheCount * sizeof(Chunk));
	renderer->LoadQueue = MemoryAllocate(renderer->ChunkCacheCount * sizeof(Chunk));
	
	int l = 0;
	for (int i = 0; i < renderer->ChunkCount.x; i++)
	{
		for (int j = 0; j < renderer->ChunkCount.y; j++)
		{
			for (int k = 0; k < renderer->ChunkCount.z; k++)
			{
				int c = (k * renderer->ChunkCount.y + j) * renderer->ChunkCount.x + i;
				renderer->ChunkCache[c] = ChunkCreate(renderer->Level, (int3){ i, j, k } << 4, 16, renderer->BaseListID + l);
				renderer->LoadQueue[c] = renderer->ChunkCache[c];
				l += 2;
			}
		}
	}
	
	for (int i = 0; i < ListCount(renderer->Chunks); i++) { renderer->Chunks[i]->Loaded = false; }
	renderer->Chunks = ListClear(renderer->Chunks);
	
	glNewList(renderer->ListID, GL_COMPILE);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	float ground = LevelGetGroundLevel(renderer->Level);
	int a = fmin(128, fmin(renderer->Level->Width, renderer->Level->Height));
	int b = 2048 / a;
	ShapeRendererBegin();
	for (int i = -a * b; i < renderer->Level->Width + a * b; i += a)
	{
		for (int j = -a * b; j < renderer->Level->Height + a * b; j += a)
		{
			float g = ground;
			if (i >= 0 && j >= 0 && i < renderer->Level->Width && j < renderer->Level->Height) { g = 0.0; }
			ShapeRendererVertexUV((float3){ i, g, j + a }, (float2){ 0.0, a });
			ShapeRendererVertexUV((float3){ i + a, g, j + a }, (float2){ a, a });
			ShapeRendererVertexUV((float3){ i + a, g, j }, (float2){ a, 0.0 });
			ShapeRendererVertexUV((float3){ i, g, j }, (float2){ 0.0, 0.0 });
		}
	}
	ShapeRendererEnd();
	
	glColor3f(0.8, 0.8, 0.8);
	ShapeRendererBegin();
	for (int i = 0; i < renderer->Level->Width; i += a)
	{
		ShapeRendererVertexUV((float3){ i, 0.0, 0.0 }, (float2){ 0.0, 0.0 });
		ShapeRendererVertexUV((float3){ i + a, 0.0, 0.0 }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ i + a, ground, 0.0 }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ i, ground, 0.0 }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ i, ground, renderer->Level->Height }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ i + a, ground, renderer->Level->Height }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ i + a, 0.0, renderer->Level->Height }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ i, 0.0, renderer->Level->Height }, (float2){ 0.0, 0.0 });
	}
	glColor3f(0.6, 0.6, 0.6);
	for (int i = 0; i < renderer->Level->Height; i += a)
	{
		ShapeRendererVertexUV((float3){ 0.0, ground, i }, (float2){ 0.0, 0.0 });
		ShapeRendererVertexUV((float3){ 0.0, ground, i + a }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ 0.0, 0.0, i + a }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ 0.0, 0.0, i }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ renderer->Level->Width, 0.0, i }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ renderer->Level->Width, 0.0, i + a }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ renderer->Level->Width, ground, i + a }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ renderer->Level->Width, ground, i }, (float2){ 0.0, 0.0 });
	}
	ShapeRendererEnd();
	glEndList();
	
	glNewList(renderer->ListID + 1, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	float water = LevelGetWaterLevel(renderer->Level);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ShapeRendererBegin();
	for (int i = -a * b; i < renderer->Level->Width + a * b; i += a)
	{
		for (int j = -a * b; j < renderer->Level->Height + a * b; j += a)
		{
			float w = water - 0.1;
			if (i < 0 || j < 0 || i >= renderer->Level->Width || j >= renderer->Level->Height)
			{
				ShapeRendererVertexUV((float3){ i, w, j + a }, (float2){ 0.0, a });
				ShapeRendererVertexUV((float3){ i + a, w, j + a }, (float2){ a, a });
				ShapeRendererVertexUV((float3){ i + a, w, j }, (float2){ a, 0.0 });
				ShapeRendererVertexUV((float3){ i, w, j }, (float2){ 0.0, 0.0 });
				ShapeRendererVertexUV((float3){ i, w, j }, (float2){ 0.0, 0.0 });
				ShapeRendererVertexUV((float3){ i + a, w, j }, (float2){ a, 0.0 });
				ShapeRendererVertexUV((float3){ i + a, w, j + a }, (float2){ a, a });
				ShapeRendererVertexUV((float3){ i, w, j + a }, (float2){ 0.0, a });
			}
		}
	}
	ShapeRendererEnd();
	glDisable(GL_BLEND);
	glEndList();
	LevelRendererQueueChunks(renderer, (int3){ 0, 0, 0 }, (int3){ renderer->Level->Width, renderer->Level->Depth, renderer->Level->Height });
}

int LevelRendererSortChunks(LevelRenderer renderer, Player player, int pass)
{
	float3 v = player->Position - renderer->LastLoad;
	if (length3f(v) > 8.0)
	{
		renderer->LastLoad = player->Position;
		qsort(renderer->LoadQueue, renderer->ChunkCacheCount, sizeof(Chunk), ChunkDistanceComparator);
	}
	
	int count = 0;
	for (int i = 0; i < renderer->ChunkCacheCount; i++) { count = ChunkAppendLists(renderer->LoadQueue[i], renderer->ChunkDataCache, count, pass); }
	if (count > 0)
	{
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(renderer->Textures, "Terrain.png"));
		glCallLists(count, GL_INT, renderer->ChunkDataCache);
	}
	return count;
}

void LevelRendererQueueChunks(LevelRenderer renderer, int3 v0, int3 v1)
{
	v0 /= 16;
	v1 /= 16;
	v0.x = v0.x < 0 ? 0 : v0.x;
	v0.y = v0.y < 0 ? 0 : v0.y;
	v0.z = v0.z < 0 ? 0 : v0.z;
	v1.x = v1.x > renderer->ChunkCount.x - 1 ? renderer->ChunkCount.x - 1 : v1.x;
	v1.y = v1.y > renderer->ChunkCount.y - 1 ? renderer->ChunkCount.y - 1 : v1.y;
	v1.z = v1.z > renderer->ChunkCount.z - 1 ? renderer->ChunkCount.z - 1 : v1.z;
	for (int x = v0.x; x <= v1.x; x++)
	{
		for (int y = v0.y; y <= v1.y; y++)
		{
			for (int z = v0.z; z <= v1.z; z++)
			{
				Chunk chunk = renderer->ChunkCache[(z * renderer->ChunkCount.y + y) * renderer->ChunkCount.x + x];
				if (!chunk->Loaded)
				{
					chunk->Loaded = true;
					renderer->Chunks = ListPush(renderer->Chunks, &chunk);
				}
			}
		}
	}
}

void LevelRendererDestroy(LevelRenderer renderer)
{
	glDeleteLists(renderer->BaseListID, 4096 << 6 << 1);
	glDeleteLists(renderer->ListID, 2);
	ListDestroy(renderer->Chunks);
	MemoryFree(renderer->ChunkDataCache);
	if (renderer->ChunkCache != NULL)
	{
		for (int i = 0; i < renderer->ChunkCacheCount; i++) { ChunkDestroy(renderer->ChunkCache[i]); }
		MemoryFree(renderer->ChunkCache);
	}
	if (renderer->LoadQueue != NULL) { MemoryFree(renderer->LoadQueue); }
	MemoryFree(renderer);
}
