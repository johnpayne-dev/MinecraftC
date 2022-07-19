#pragma once
#include "../Level/Level.h"
#include "../Player/Player.h"
#include "../Render/Frustum.h"

typedef struct Chunk {
	Level * level;
	int baseListID;
	int x, y, z;
	int width, depth, height;
	bool visible;
	bool dirty[2];
	bool loaded;
} Chunk;

extern int ChunkUpdates;

void ChunkCreate(Chunk * chunk, Level * level, int x, int y, int z, int chunkSize, int baseListID);
void ChunkUpdate(Chunk * chunk);
float ChunkDistanceSquared(Chunk * chunk, Player * player);
void ChunkSetAllDirty(Chunk * chunk);
void ChunkDispose(Chunk * chunk);
int ChunkAppendLists(Chunk * chunk, int dataCache[], int count, int pass);
void ChunkClip(Chunk * chunk, Frustum frustum);
void ChunkDestroy(Chunk * chunk);

int ChunkDistanceComparator(const void * a, const void * b);
int ChunkVisibleDistanceComparator(const void * a, const void * b);
