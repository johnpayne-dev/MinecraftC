#include "LevelRenderer.h"
#include "ShapeRenderer.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

LevelRenderer LevelRendererCreate(Minecraft minecraft, TextureManager textures) {
	LevelRenderer renderer = MemoryAllocate(sizeof(struct LevelRenderer));
	*renderer = (struct LevelRenderer) {
		.chunks = ListCreate(sizeof(Chunk)),
		.chunkDataCache = MemoryAllocate(65536 * sizeof(int)),
		.ticks = 0,
		.lastLoad = one3f * -9999.0,
		.minecraft = minecraft,
		.textures = textures,
		.listID = glGenLists(2),
		.baseListID = glGenLists(4096 << 6 << 1),
	};
	return renderer;
}

void LevelRendererRefresh(LevelRenderer renderer) {
	if (renderer->chunkCache != NULL) {
		for (int i = 0; i < renderer->chunkCacheCount; i++) { ChunkDispose(renderer->chunkCache[i]); }
		MemoryFree(renderer->chunkCache);
		MemoryFree(renderer->loadQueue);
		/*renderer->Chunks = ListClear(renderer->Chunks);
		for (int i = 0; i < 65536; i++) { renderer->ChunkDataCache[i] = 0; }
		glDeleteLists(renderer->BaseListID, 4096 << 6 << 1);
		renderer->BaseListID = glGenLists(4096 << 6 << 1);*/
	}
	
	renderer->chunkCount = (int3){ renderer->level->width, renderer->level->depth, renderer->level->height } / 16;
	renderer->chunkCacheCount = renderer->chunkCount.x * renderer->chunkCount.y * renderer->chunkCount.z;
	renderer->chunkCache = MemoryAllocate(renderer->chunkCacheCount * sizeof(Chunk));
	renderer->loadQueue = MemoryAllocate(renderer->chunkCacheCount * sizeof(Chunk));
	
	int l = 0;
	for (int i = 0; i < renderer->chunkCount.x; i++) {
		for (int j = 0; j < renderer->chunkCount.y; j++) {
			for (int k = 0; k < renderer->chunkCount.z; k++) {
				int c = (k * renderer->chunkCount.y + j) * renderer->chunkCount.x + i;
				renderer->chunkCache[c] = ChunkCreate(renderer->level, (int3){ i, j, k } << 4, 16, renderer->baseListID + l);
				renderer->loadQueue[c] = renderer->chunkCache[c];
				l += 2;
			}
		}
	}
	
	for (int i = 0; i < ListCount(renderer->chunks); i++) { renderer->chunks[i]->loaded = false; }
	renderer->chunks = ListClear(renderer->chunks);
	
	glNewList(renderer->listID, GL_COMPILE);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	float ground = LevelGetGroundLevel(renderer->level);
	int a = fmin(128, fmin(renderer->level->width, renderer->level->height));
	int b = 2048 / a;
	ShapeRendererBegin();
	for (int i = -a * b; i < renderer->level->width + a * b; i += a) {
		for (int j = -a * b; j < renderer->level->height + a * b; j += a) {
			float g = ground;
			if (i >= 0 && j >= 0 && i < renderer->level->width && j < renderer->level->height) { g = 0.0; }
			ShapeRendererVertexUV((float3){ i, g, j + a }, (float2){ 0.0, a });
			ShapeRendererVertexUV((float3){ i + a, g, j + a }, (float2){ a, a });
			ShapeRendererVertexUV((float3){ i + a, g, j }, (float2){ a, 0.0 });
			ShapeRendererVertexUV((float3){ i, g, j }, (float2){ 0.0, 0.0 });
		}
	}
	ShapeRendererEnd();
	
	glColor3f(0.8, 0.8, 0.8);
	ShapeRendererBegin();
	for (int i = 0; i < renderer->level->width; i += a) {
		ShapeRendererVertexUV((float3){ i, 0.0, 0.0 }, (float2){ 0.0, 0.0 });
		ShapeRendererVertexUV((float3){ i + a, 0.0, 0.0 }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ i + a, ground, 0.0 }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ i, ground, 0.0 }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ i, ground, renderer->level->height }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ i + a, ground, renderer->level->height }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ i + a, 0.0, renderer->level->height }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ i, 0.0, renderer->level->height }, (float2){ 0.0, 0.0 });
	}
	glColor3f(0.6, 0.6, 0.6);
	for (int i = 0; i < renderer->level->height; i += a) {
		ShapeRendererVertexUV((float3){ 0.0, ground, i }, (float2){ 0.0, 0.0 });
		ShapeRendererVertexUV((float3){ 0.0, ground, i + a }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ 0.0, 0.0, i + a }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ 0.0, 0.0, i }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ renderer->level->width, 0.0, i }, (float2){ 0.0, ground });
		ShapeRendererVertexUV((float3){ renderer->level->width, 0.0, i + a }, (float2){ a, ground });
		ShapeRendererVertexUV((float3){ renderer->level->width, ground, i + a }, (float2){ a, 0.0 });
		ShapeRendererVertexUV((float3){ renderer->level->width, ground, i }, (float2){ 0.0, 0.0 });
	}
	ShapeRendererEnd();
	glEndList();
	
	glNewList(renderer->listID + 1, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	float water = LevelGetWaterLevel(renderer->level);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ShapeRendererBegin();
	for (int i = -a * b; i < renderer->level->width + a * b; i += a) {
		for (int j = -a * b; j < renderer->level->height + a * b; j += a) {
			float w = water - 0.1;
			if (i < 0 || j < 0 || i >= renderer->level->width || j >= renderer->level->height) {
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
	LevelRendererQueueChunks(renderer, (int3){ 0, 0, 0 }, (int3){ renderer->level->width, renderer->level->depth, renderer->level->height });
}

int LevelRendererSortChunks(LevelRenderer renderer, Player player, int pass) {
	float3 v = player->position - renderer->lastLoad;
	if (length3f(v) > 8.0) {
		renderer->lastLoad = player->position;
		qsort(renderer->loadQueue, renderer->chunkCacheCount, sizeof(Chunk), ChunkDistanceComparator);
	}
	
	int count = 0;
	for (int i = 0; i < renderer->chunkCacheCount; i++) { count = ChunkAppendLists(renderer->loadQueue[i], renderer->chunkDataCache, count, pass); }
	if (count > 0) {
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(renderer->textures, "Terrain.png"));
		glCallLists(count, GL_INT, renderer->chunkDataCache);
	}
	return count;
}

void LevelRendererQueueChunks(LevelRenderer renderer, int3 v0, int3 v1) {
	v0 /= 16;
	v1 /= 16;
	v0.x = v0.x < 0 ? 0 : v0.x;
	v0.y = v0.y < 0 ? 0 : v0.y;
	v0.z = v0.z < 0 ? 0 : v0.z;
	v1.x = v1.x > renderer->chunkCount.x - 1 ? renderer->chunkCount.x - 1 : v1.x;
	v1.y = v1.y > renderer->chunkCount.y - 1 ? renderer->chunkCount.y - 1 : v1.y;
	v1.z = v1.z > renderer->chunkCount.z - 1 ? renderer->chunkCount.z - 1 : v1.z;
	for (int x = v0.x; x <= v1.x; x++) {
		for (int y = v0.y; y <= v1.y; y++) {
			for (int z = v0.z; z <= v1.z; z++) {
				Chunk chunk = renderer->chunkCache[(z * renderer->chunkCount.y + y) * renderer->chunkCount.x + x];
				if (!chunk->loaded) {
					chunk->loaded = true;
					renderer->chunks = ListPush(renderer->chunks, &chunk);
				}
			}
		}
	}
}

void LevelRendererDestroy(LevelRenderer renderer) {
	glDeleteLists(renderer->baseListID, 4096 << 6 << 1);
	glDeleteLists(renderer->listID, 2);
	ListDestroy(renderer->chunks);
	MemoryFree(renderer->chunkDataCache);
	if (renderer->chunkCache != NULL) {
		for (int i = 0; i < renderer->chunkCacheCount; i++) { ChunkDestroy(renderer->chunkCache[i]); }
		MemoryFree(renderer->chunkCache);
	}
	if (renderer->loadQueue != NULL) { MemoryFree(renderer->loadQueue); }
	MemoryFree(renderer);
}
