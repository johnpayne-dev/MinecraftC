#pragma once
#include "../Level/Level.h"
#include "../Player/Player.h"
#include "../Render/Frustum.h"

typedef struct Chunk
{
	Level Level;
	int BaseListID;
	int3 Position;
	int Width, Depth, Height;
	bool Visible;
	bool Dirty[2];
	bool Loaded;
} * Chunk;

extern int ChunkUpdates;

Chunk ChunkCreate(Level level, int3 pos, int chunkSize, int baseListID);
void ChunkUpdate(Chunk chunk);
float ChunkDistanceSquared(Chunk chunk, Player player);
void ChunkSetAllDirty(Chunk chunk);
void ChunkDispose(Chunk chunk);
int ChunkAppendLists(Chunk chunk, int var1[], int var2, int var3);
void ChunkClip(Chunk chunk, Frustum frustum);
void ChunkDestroy(Chunk chunk);

int ChunkDistanceComparator(const void * a, const void * b);
int ChunkVisibleDistanceComparator(const void * a, const void * b);
