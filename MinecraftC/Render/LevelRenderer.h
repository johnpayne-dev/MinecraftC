#pragma once
#include "Chunk.h"
#include "../Level/Level.h"

typedef struct LevelRenderer {
	Level * level;
	TextureManager * textures;
	int listID;
	List(Chunk *) chunks;
	Chunk ** loadQueue;
	Chunk ** chunkCache;
	int chunkCacheCount;
	int xChunks, yChunks, zChunks;
	int baseListID;
	int * chunkDataCache;
	int ticks;
	float lastLoadX, lastLoadY, lastLoadZ;
} LevelRenderer;

void LevelRendererCreate(LevelRenderer * renderer, struct Minecraft * minecraft, Level * level, TextureManager * textures);
void LevelRendererRefresh(LevelRenderer * renderer);
int LevelRendererSortChunks(LevelRenderer * renderer, Player * player, int pass);
void LevelRendererQueueChunks(LevelRenderer * renderer, int x0, int y0, int z0, int x1, int y1, int z1);
void LevelRendererDestroy(LevelRenderer * renderer);
