#pragma once
#include "../Utilities/LinearMath.h"
#include "../Entity.h"

typedef struct BlockMapSlot
{
	int3 Position;
	struct BlockMap * BlockMap;
} * BlockMapSlot;

BlockMapSlot BlockMapSlotCreate(struct BlockMap * blockMap);
BlockMapSlot BlockMapSlotInitialize(BlockMapSlot slot, float3 pos);
void BlockMapSlotAdd(BlockMapSlot slot, Entity entity);
void BlockMapSlotRemove(BlockMapSlot slot, Entity entity);
void BlockMapSlotDestroy(BlockMapSlot slot);
