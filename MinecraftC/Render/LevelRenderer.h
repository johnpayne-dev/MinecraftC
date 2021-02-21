#pragma once
#include "Chunk.h"
#include "../Level/Level.h"

typedef struct LevelRenderer
{
	Level Level;
	TextureManager Textures;
	int ListID;
	list(Chunk) Chunks;
	Chunk * LoadQueue;
	Chunk * ChunkCache;
	int ChunkCacheCount;
	int3 ChunkCount;
	int BaseListID;
	struct Minecraft * Minecraft;
	int * ChunkDataCache;
	int Ticks;
	float3 LastLoad;
	float Cracks;
} * LevelRenderer;

LevelRenderer LevelRendererCreate(struct Minecraft * minecraft, TextureManager textures);
void LevelRendererRefresh(LevelRenderer renderer);
int LevelRendererSortChunks(LevelRenderer renderer, Player player, int var);
void LevelRendererQueueChunks(LevelRenderer renderer, int3 v0, int3 v1);
void LevelRendererDestroy(LevelRenderer renderer);
