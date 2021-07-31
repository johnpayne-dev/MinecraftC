#pragma once
#include <stdbool.h>
#include "Tile/Block.h"
#include "NextTickListEntry.h"
#include "../MovingObjectPosition.h"
#include "../Utilities/List.h"
#include "../Utilities/Random.h"
#include "../Particle/ParticleManager.h"
#include "../Physics/AABB.h"
#include "../GUI/FontRenderer.h"

typedef struct Level
{
	int Width, Height, Depth;
	unsigned char * Blocks;
	const char * Name;
	const char * Creator;
	long CreateTime;
	int3 Spawn;
	float SpawnRotation;
	list(struct LevelRenderer *) Renderers;
	int * LightBlockers;
	RandomGenerator Random;
	int RandomValue;
	list(NextTickListEntry) TickList;
	bool NetworkMode;
	struct Minecraft * Minecraft;
	bool CreativeMode;
	int WaterLevel;
	Color SkyColor;
	Color FogColor;
	Color CloudColor;
	int Unprocessed;
	int TickCount;
	Entity Player;
	ParticleManager ParticleEngine;
	FontRenderer Font;
	bool GrowTrees;
} * Level;

Level LevelCreate(void);
void LevelInitializeTransient(Level level);
void LevelSetData(Level level, int w, int d, int h, unsigned char * blocks);
void LevelFindSpawn(Level level);
void LevelCalculateLightDepths(Level level, int x0, int y0, int x1, int y1);
void LevelAddRenderer(Level level, struct LevelRenderer * listener);
void LevelFinalize(Level level);
void LevelRemoveRenderer(Level level, struct LevelRenderer * listener);
bool LevelIsLightBlocker(Level level, int x, int y, int z);
list(AABB) LevelGetCubes(Level level, AABB box);
void LevelSwap(Level level, int3 xyz0, int3 xyz1);
bool LevelSetTile(Level level, int x, int y, int z, BlockType tile);
bool LevelNetSetTile(Level level, int x, int y, int z, BlockType tile);
bool LevelSetTileNoNeighborChange(Level level, int x, int y, int z, BlockType tile);
bool LevelNetSetTileNoNeighborChange(Level level, int x, int y, int z, BlockType tile);
void LevelUpdateNeighborsAt(Level Level, int x, int y, int z, BlockType tile);
bool LevelSetTileNoUpdate(Level level, int x, int y, int z, BlockType tile);
bool LevelIsLit(Level level, int x, int y, int z);
BlockType LevelGetTile(Level level, int x, int y, int z);
bool LevelIsSolidTile(Level level, int x, int y, int z);
void LevelTickEntities(Level level);
void LevelTick(Level level);
bool LevelIsInBounds(Level level, int x, int y, int z);
float LevelGetGroundLevel(Level level);
float LevelGetWaterLevel(Level level);
bool LevelContainsAnyLiquid(Level level, AABB box);
bool LevelContainsLiquid(Level level, AABB box, LiquidType liquidID);
void LevelAddToNextTick(Level level, int x, int y, int z, BlockType tile);
bool LevelIsSolidSearch(Level level, float3 pos, float search);
int LevelGetHighestTile(Level level, int x, int z);
void LevelSetSpawnPosition(Level level, int x, int y, int z, float rotation);
float LevelGetBrightness(Level level, int x, int y, int z);
float LevelGetCaveness(Level level, float3 xyz, float degrees);
float LevelGetCavenessEntity(Level level, Entity entity);
unsigned char * LevelCopyBlocks(Level level);
LiquidType LevelGetLiquidType(Level level, int x, int y, int z);
bool LevelIsWater(Level level, int x, int y, int z);
void LevelSetNetworkMode(Level level, bool network);
MovingObjectPosition LevelClip(Level level, float3 v0, float3 v1);
void LevelPlaySound(Level level, const char * sound, Entity entity, float volume, float pitch);
void LevelPlaySoundAt(Level level, const char * sound, float3 position, float volume, float pitch);
bool LevelMaybeGrowTree(Level level, int x, int y, int z);
Entity LevelGetPlayer(Level level);
void LevelAddEntity(Level level, Entity entity);
void LevelExplode(Level level, Entity entity, float3 pos, float radius);
Entity LevelFindPlayer(Level level);
void LevelDestroy(Level level);
