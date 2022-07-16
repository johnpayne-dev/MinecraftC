#include "Chunk.h"
#include "ShapeRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

int ChunkUpdates = 0;

Chunk ChunkCreate(Level level, int3 pos, int chunkSize, int baseListID) {
	Chunk chunk = MemoryAllocate(sizeof(struct Chunk));
	*chunk = (struct Chunk) {
		.visible = false,
		.level = level,
		.position = pos,
		.width = 16,
		.height = 16,
		.depth = 16,
		.baseListID = baseListID,
	};
	ChunkSetAllDirty(chunk);
	return chunk;
}

void ChunkUpdate(Chunk chunk) {
	ChunkUpdates++;
	int3 v0 = chunk->position;
	int3 v1 = chunk->position + (int3){ chunk->width, chunk->height, chunk->depth };
	
	for (int i = 0; i < 2; i++) { chunk->dirty[i] = true; }
	for (int i = 0; i < 2; i++) {
		bool b0 = false;
		bool b1 = false;
		glNewList(chunk->baseListID + i, GL_COMPILE);
		ShapeRendererBegin();
		for (int x = v0.x; x < v1.x; x++) {
			for (int y = v0.y; y < v1.y; y++) {
				for (int z = v0.z; z < v1.z; z++) {
					BlockType tile = LevelGetTile(chunk->level, x, y, z);
					if (tile != BlockTypeNone) {
						Block block = Blocks.table[tile];
						if (BlockGetRenderPass(block) != i) { b0 = true; }
						else { b1 |= BlockRender(block, chunk->level, x, y, z); }
					}
				}
			}
		}
		ShapeRendererEnd();
		glEndList();
		if (b1) { chunk->dirty[i] = false; }
		if (!b0) { break; }
	}
}

float ChunkDistanceSquared(Chunk chunk, Player player) {
	return sqdistance3f(player->position, float3i(chunk->position));
}

void ChunkSetAllDirty(Chunk chunk) {
	for (int i = 0; i < 2; i++) { chunk->dirty[i] = true; }
}

void ChunkDispose(Chunk chunk) {
	ChunkSetAllDirty(chunk);
	ChunkDestroy(chunk);
}

int ChunkAppendLists(Chunk chunk, int dataCache[], int count, int pass) {
	if (!chunk->visible) { return count; }
	else {
		if (!chunk->dirty[pass]) { dataCache[count++] = chunk->baseListID + pass; }
		return count;
	}
}

void ChunkClip(Chunk chunk, Frustum frustum) {
	float3 p = float3i(chunk->position);
	chunk->visible = FrustumContainsBox(frustum, p, p + (float3){ chunk->width, chunk->height, chunk->depth });
}

void ChunkDestroy(Chunk chunk) {
	MemoryFree(chunk);
}

int ChunkDistanceComparator(const void * a, const void * b) {
	float distA = sqdistance3f(float3i((*(Chunk *)a)->position), (*(Chunk *)a)->level->player->position);
	float distB = sqdistance3f(float3i((*(Chunk *)b)->position), (*(Chunk *)a)->level->player->position);
	return distA == distB ? 0 : (distA > distB ? 1 : -1);
}

int ChunkVisibleDistanceComparator(const void * a, const void * b) {
	if ((*(Chunk *)a)->visible || !(*(Chunk *)b)->visible) {
		if ((*(Chunk *)b)->visible) {
			float distA = sqdistance3f(float3i((*(Chunk *)a)->position), (*(Chunk *)a)->level->player->position);
			float distB = sqdistance3f(float3i((*(Chunk *)b)->position), (*(Chunk *)a)->level->player->position);
			return distA == distB ? 0 : (distA > distB ? 1 : -1);
		} else { return -1; }
	} else { return 1; }
}
