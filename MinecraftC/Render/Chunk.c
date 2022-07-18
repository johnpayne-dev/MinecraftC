#include "Chunk.h"
#include "ShapeRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

int ChunkUpdates = 0;

Chunk ChunkCreate(Level level, int x, int y, int z, int chunkSize, int baseListID) {
	Chunk chunk = malloc(sizeof(struct Chunk));
	*chunk = (struct Chunk) {
		.visible = false,
		.level = level,
		.x = x,
		.y = y,
		.z = z,
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
	int x0 = chunk->x, y0 = chunk->y, z0 = chunk->z;
	int x1 = chunk->x + chunk->width;
	int y1 = chunk->y + chunk->height;
	int z1 = chunk->z + chunk->depth;
	
	for (int i = 0; i < 2; i++) {
		chunk->dirty[i] = true;
	}
	for (int i = 0; i < 2; i++) {
		bool b0 = false;
		bool b1 = false;
		glNewList(chunk->baseListID + i, GL_COMPILE);
		ShapeRendererBegin();
		for (int x = x0; x < x1; x++) {
			for (int y = y0; y < y1; y++) {
				for (int z = z0; z < z1; z++) {
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
	return (player->x - chunk->x) * (player->x - chunk->x) + (player->y - chunk->y) * (player->y - chunk->y) + (player->z - chunk->z) * (player->z - chunk->z);
}

void ChunkSetAllDirty(Chunk chunk) {
	for (int i = 0; i < 2; i++) {
		chunk->dirty[i] = true;
	}
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
	chunk->visible = FrustumContainsBox(frustum, chunk->x, chunk->y, chunk->z, chunk->x + chunk->width, chunk->y + chunk->height, chunk->z + chunk->depth);
}

void ChunkDestroy(Chunk chunk) {
	free(chunk);
}

int ChunkDistanceComparator(const void * a, const void * b) {
	float ax = (*(Chunk *)a)->x, ay = (*(Chunk *)a)->y, az = (*(Chunk *)a)->z;
	float apx = (*(Chunk *)a)->level->player->x, apy = (*(Chunk *)a)->level->player->y, apz = (*(Chunk *)a)->level->player->z;
	float bx = (*(Chunk *)b)->x, by = (*(Chunk *)b)->y, bz = (*(Chunk *)b)->z;
	float bpx = (*(Chunk *)b)->level->player->x, bpy = (*(Chunk *)b)->level->player->y, bpz = (*(Chunk *)b)->level->player->z;
	float distA = (ax - apx) * (ax - apx) + (ay - apy) * (ay - apy) + (az - apz) * (az - apz);
	float distB = (bx - bpx) * (bx - bpx) + (by - bpy) * (by - bpy) + (bz - bpz) * (bz - bpz);
	return distA == distB ? 0 : (distA > distB ? 1 : -1);
}

int ChunkVisibleDistanceComparator(const void * a, const void * b) {
	if ((*(Chunk *)a)->visible || !(*(Chunk *)b)->visible) {
		if ((*(Chunk *)b)->visible) {
			float ax = (*(Chunk *)a)->x, ay = (*(Chunk *)a)->y, az = (*(Chunk *)a)->z;
			float apx = (*(Chunk *)a)->level->player->x, apy = (*(Chunk *)a)->level->player->y, apz = (*(Chunk *)a)->level->player->z;
			float bx = (*(Chunk *)b)->x, by = (*(Chunk *)b)->y, bz = (*(Chunk *)b)->z;
			float bpx = (*(Chunk *)b)->level->player->x, bpy = (*(Chunk *)b)->level->player->y, bpz = (*(Chunk *)b)->level->player->z;
			float distA = (ax - apx) * (ax - apx) + (ay - apy) * (ay - apy) + (az - apz) * (az - apz);
			float distB = (bx - bpx) * (bx - bpx) + (by - bpy) * (by - bpy) + (bz - bpz) * (bz - bpz);
			return distA == distB ? 0 : (distA > distB ? 1 : -1);
		} else { return -1; }
	} else { return 1; }
}
