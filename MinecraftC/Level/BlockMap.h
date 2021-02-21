#pragma once
#include "../Utilities/List.h"
#include "../Render/Frustum.h"
#include "../Render/TextureManager.h"
#include "BlockMapSlot.h"

typedef struct BlockMap
{
	int Width;
	int Height;
	int Depth;
	BlockMapSlot Slot1;
	BlockMapSlot Slot2;
	list(Entity) * EntityGrid;
	list(Entity) All;
} * BlockMap;

BlockMap BlockMapCreate(int width, int depth, int height);
void BlockMapInsert(BlockMap map, Entity entity);
void BlockMapRemove(BlockMap map, Entity entity);
void BlockMapMoved(BlockMap map, Entity entity);
list(Entity) BlockMapGetEntities(BlockMap map, Entity entity, float3 v0, float3 v1);
list(Entity) BlockMapGetEntitiesAABB(BlockMap map, Entity entity, AABB aabb);
void BlockMapRemoveAllNonCreativeModeEntities(BlockMap map);
void BlockMapClear(BlockMap map);
void BlockMapTickAll(BlockMap map);
void BlockMapRender(BlockMap map, float3 v, Frustum frustum, TextureManager textures, float t);
void BlockMapDestroy(BlockMap map);
