#include "Chunk.h"
#include "ShapeRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

int ChunkUpdates = 0;

void ChunkCreate(Chunk * chunk, Level * level, int x, int y, int z, int chunkSize, int baseListID) {
	*chunk = (Chunk) {
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
}

void ChunkUpdate(Chunk * chunk) {
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

float ChunkDistanceSquared(Chunk * chunk, Player * player) {
	return (player->x - chunk->x) * (player->x - chunk->x) + (player->y - chunk->y) * (player->y - chunk->y) + (player->z - chunk->z) * (player->z - chunk->z);
}

void ChunkSetAllDirty(Chunk * chunk) {
	for (int i = 0; i < 2; i++) {
		chunk->dirty[i] = true;
	}
}

void ChunkDispose(Chunk * chunk) {
	ChunkSetAllDirty(chunk);
	ChunkDestroy(chunk);
}

int ChunkAppendLists(Chunk * chunk, int dataCache[], int count, int pass) {
	if (!chunk->visible) { return count; }
	else {
		if (!chunk->dirty[pass]) { dataCache[count++] = chunk->baseListID + pass; }
		return count;
	}
}

void ChunkClip(Chunk * chunk, Frustum frustum) {
	chunk->visible = FrustumContainsBox(frustum, chunk->x, chunk->y, chunk->z, chunk->x + chunk->width, chunk->y + chunk->height, chunk->z + chunk->depth);
}

void ChunkDestroy(Chunk * chunk) {
	free(chunk);
}

int ChunkDistanceComparator(const void * a, const void * b) {
	Chunk * ca = *(Chunk **)a;
	Chunk * cb = *(Chunk **)b;
	float apx = ca->level->player->x, apy = ca->level->player->y, apz = ca->level->player->z;
	float bpx = cb->level->player->x, bpy = cb->level->player->y, bpz = cb->level->player->z;
	float distA = (ca->x - apx) * (ca->x - apx) + (ca->y - apy) * (ca->y - apy) + (ca->z - apz) * (ca->z - apz);
	float distB = (cb->x - bpx) * (cb->x - bpx) + (cb->y - bpy) * (cb->y - bpy) + (cb->z - bpz) * (cb->z - bpz);
	return distA == distB ? 0 : (distA > distB ? 1 : -1);
}

int ChunkVisibleDistanceComparator(const void * a, const void * b) {
	Chunk * ca = *(Chunk **)a;
	Chunk * cb = *(Chunk **)b;
	if (ca->visible || !cb->visible) {
		if (cb->visible) {
			float apx = ca->level->player->x, apy = ca->level->player->y, apz = ca->level->player->z;
			float bpx = cb->level->player->x, bpy = cb->level->player->y, bpz = cb->level->player->z;
			float distA = (ca->x - apx) * (ca->x - apx) + (ca->y - apy) * (ca->y - apy) + (ca->z - apz) * (ca->z - apz);
			float distB = (cb->x - bpx) * (cb->x - bpx) + (cb->y - bpy) * (cb->y - bpy) + (cb->z - bpz) * (cb->z - bpz);
			return distA == distB ? 0 : (distA > distB ? 1 : -1);
		} else { return -1; }
	} else { return 1; }
}
