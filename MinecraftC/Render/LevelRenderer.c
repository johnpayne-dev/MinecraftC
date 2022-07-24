#include "LevelRenderer.h"
#include "ShapeRenderer.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

void LevelRendererCreate(LevelRenderer * renderer, Minecraft * minecraft, TextureManager * textures) {
	*renderer = (LevelRenderer) {
		.chunks = ListCreate(sizeof(Chunk *)),
		.chunkDataCache = malloc(65536 * sizeof(int)),
		.ticks = 0,
		.lastLoadX = -9999.0,
		.lastLoadY = -9999.0,
		.lastLoadZ = -9999.0,
		.textures = textures,
		.listID = glGenLists(2),
		.baseListID = glGenLists(4096 << 6 << 1),
	};
}

void LevelRendererRefresh(LevelRenderer * renderer) {
	if (renderer->chunkCache != NULL) {
		for (int i = 0; i < renderer->chunkCacheCount; i++) { ChunkDispose(renderer->chunkCache[i]); }
		free(renderer->chunkCache);
		free(renderer->loadQueue);
	}
	
	renderer->xChunks = renderer->level->width / 16;
	renderer->yChunks = renderer->level->depth / 16;
	renderer->zChunks = renderer->level->height / 16;
	renderer->chunkCacheCount = renderer->xChunks * renderer->yChunks * renderer->zChunks;
	renderer->chunkCache = malloc(renderer->chunkCacheCount * sizeof(Chunk *));
	renderer->loadQueue = malloc(renderer->chunkCacheCount * sizeof(Chunk *));
	
	int l = 0;
	for (int i = 0; i < renderer->xChunks; i++) {
		for (int j = 0; j < renderer->yChunks; j++) {
			for (int k = 0; k < renderer->zChunks; k++) {
				int c = (k * renderer->yChunks + j) * renderer->xChunks + i;
				renderer->chunkCache[c] = malloc(sizeof(Chunk));
				ChunkCreate(renderer->chunkCache[c], renderer->level, i << 4, j << 4, k << 4, 16, renderer->baseListID + l);
				renderer->loadQueue[c] = renderer->chunkCache[c];
				l += 2;
			}
		}
	}
	
	for (int i = 0; i < ListLength(renderer->chunks); i++) { renderer->chunks[i]->loaded = false; }
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
			ShapeRendererVertexUV(i, g, j + a, 0.0, a);
			ShapeRendererVertexUV(i + a, g, j + a, a, a);
			ShapeRendererVertexUV(i + a, g, j, a, 0.0);
			ShapeRendererVertexUV(i, g, j, 0.0, 0.0);
		}
	}
	ShapeRendererEnd();
	
	glColor3f(0.8, 0.8, 0.8);
	ShapeRendererBegin();
	for (int i = 0; i < renderer->level->width; i += a) {
		ShapeRendererVertexUV(i, 0.0, 0.0, 0.0, 0.0);
		ShapeRendererVertexUV(i + a, 0.0, 0.0, a, 0.0);
		ShapeRendererVertexUV(i + a, ground, 0.0, a, ground);
		ShapeRendererVertexUV(i, ground, 0.0, 0.0, ground);
		ShapeRendererVertexUV(i, ground, renderer->level->height, 0.0, ground);
		ShapeRendererVertexUV(i + a, ground, renderer->level->height, a, ground);
		ShapeRendererVertexUV(i + a, 0.0, renderer->level->height, a, 0.0);
		ShapeRendererVertexUV(i, 0.0, renderer->level->height, 0.0, 0.0);
	}
	glColor3f(0.6, 0.6, 0.6);
	for (int i = 0; i < renderer->level->height; i += a) {
		ShapeRendererVertexUV(0.0, ground, i, 0.0, 0.0);
		ShapeRendererVertexUV(0.0, ground, i + a, a, 0.0);
		ShapeRendererVertexUV(0.0, 0.0, i + a, a, ground);
		ShapeRendererVertexUV(0.0, 0.0, i, 0.0, ground);
		ShapeRendererVertexUV(renderer->level->width, 0.0, i, 0.0, ground);
		ShapeRendererVertexUV(renderer->level->width, 0.0, i + a, a, ground);
		ShapeRendererVertexUV(renderer->level->width, ground, i + a, a, 0.0);
		ShapeRendererVertexUV(renderer->level->width, ground, i, 0.0, 0.0);
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
				ShapeRendererVertexUV(i, w, j + a, 0.0, a);
				ShapeRendererVertexUV(i + a, w, j + a, a, a);
				ShapeRendererVertexUV(i + a, w, j, a, 0.0);
				ShapeRendererVertexUV(i, w, j, 0.0, 0.0);
				ShapeRendererVertexUV(i, w, j, 0.0, 0.0);
				ShapeRendererVertexUV(i + a, w, j, a, 0.0);
				ShapeRendererVertexUV(i + a, w, j + a, a, a);
				ShapeRendererVertexUV(i, w, j + a, 0.0, a);
			}
		}
	}
	ShapeRendererEnd();
	glDisable(GL_BLEND);
	glEndList();
	LevelRendererQueueChunks(renderer, 0, 0, 0, renderer->level->width, renderer->level->depth, renderer->level->height);
}

int LevelRendererSortChunks(LevelRenderer * renderer, Player * player, int pass) {
	float vx = player->x - renderer->lastLoadX;
	float vy = player->y - renderer->lastLoadY;
	float vz = player->z - renderer->lastLoadZ;
	if (vx * vx + vy * vy + vz * vz > 64.0) {
		renderer->lastLoadX = player->x;
		renderer->lastLoadY = player->y;
		renderer->lastLoadZ = player->z;
		qsort(renderer->loadQueue, renderer->chunkCacheCount, sizeof(Chunk *), ChunkDistanceComparator);
	}
	
	int count = 0;
	for (int i = 0; i < renderer->chunkCacheCount; i++) { count = ChunkAppendLists(renderer->loadQueue[i], renderer->chunkDataCache, count, pass); }
	if (count > 0) {
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(renderer->textures, "Terrain.png"));
		glCallLists(count, GL_INT, renderer->chunkDataCache);
	}
	return count;
}

void LevelRendererQueueChunks(LevelRenderer * renderer, int x0, int y0, int z0, int x1, int y1, int z1) {
	x0 /= 16;
	y0 /= 16;
	z0 /= 16;
	x1 /= 16;
	y1 /= 16;
	z1 /= 16;
	x0 = x0 < 0 ? 0 : x0;
	y0 = y0 < 0 ? 0 : y0;
	z0 = z0 < 0 ? 0 : z0;
	x1 = x1 > renderer->xChunks - 1 ? renderer->xChunks - 1 : x1;
	y1 = y1 > renderer->yChunks - 1 ? renderer->yChunks - 1 : y1;
	z1 = z1 > renderer->zChunks - 1 ? renderer->zChunks - 1 : z1;
	for (int x = x0; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {
			for (int z = z0; z <= z1; z++) {
				Chunk * chunk = renderer->chunkCache[(z * renderer->yChunks + y) * renderer->xChunks + x];
				if (!chunk->loaded) {
					chunk->loaded = true;
					renderer->chunks = ListPush(renderer->chunks, &chunk);
				}
			}
		}
	}
}

void LevelRendererDestroy(LevelRenderer * renderer) {
	glDeleteLists(renderer->baseListID, 4096 << 6 << 1);
	glDeleteLists(renderer->listID, 2);
	ListFree(renderer->chunks);
	free(renderer->chunkDataCache);
	if (renderer->chunkCache != NULL) {
		for (int i = 0; i < renderer->chunkCacheCount; i++) {
			ChunkDestroy(renderer->chunkCache[i]);
			free(renderer->chunkCache[i]);
		}
		free(renderer->chunkCache);
	}
	if (renderer->loadQueue != NULL) { free(renderer->loadQueue); }
	free(renderer);
}
