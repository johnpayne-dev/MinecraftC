#pragma once
#include "Chunk.h"
#include "../Level/Level.h"

typedef struct LevelRenderer {
	Level level;
	TextureManager textures;
	int listID;
	list(Chunk) chunks;
	Chunk * loadQueue;
	Chunk * chunkCache;
	int chunkCacheCount;
	int3 chunkCount;
	int baseListID;
	struct Minecraft * minecraft;
	int * chunkDataCache;
	int ticks;
	float3 lastLoad;
	float cracks;
} * LevelRenderer;

LevelRenderer LevelRendererCreate(struct Minecraft * minecraft, TextureManager textures);
void LevelRendererRefresh(LevelRenderer renderer);
int LevelRendererSortChunks(LevelRenderer renderer, Player player, int var);
void LevelRendererQueueChunks(LevelRenderer renderer, int3 v0, int3 v1);
void LevelRendererDestroy(LevelRenderer renderer);
