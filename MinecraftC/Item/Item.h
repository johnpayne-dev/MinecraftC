#pragma once
#include "../Entity.h"
#include "../Level/Level.h"

void ItemInitializeModels(void);
void ItemDeinitializeModels(void);

typedef Entity Item;

typedef struct ItemData
{
	float3 Delta;
	float Rotation;
	BlockType Resource;
	int TickCount;
	int Age;
} * ItemData;

Item ItemCreate(Level level, float3 pos, BlockType resource);
void ItemTick(Item item);
void ItemRender(Item item, TextureManager textures, float var);
void ItemPlayerTouch(Item item, Entity player);
void ItemDestroy(Item item);
