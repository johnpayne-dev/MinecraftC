#include "BlockMapSlot.h"
#include "BlockMap.h"

BlockMapSlot BlockMapSlotCreate(BlockMap blockMap)
{
	BlockMapSlot slot = MemoryAllocate(sizeof(struct BlockMapSlot));
	*slot = (struct BlockMapSlot){ .BlockMap = blockMap, };
	return slot;
}

BlockMapSlot BlockMapSlotInitialize(BlockMapSlot slot, float3 pos)
{
	slot->Position = int3f(pos / 16.0);
	slot->Position.x = slot->Position.x < 0 ? 0 : (slot->Position.x >= slot->BlockMap->Width ? slot->BlockMap->Width - 1 : slot->Position.x);
	slot->Position.y = slot->Position.y < 0 ? 0 : (slot->Position.y >= slot->BlockMap->Depth ? slot->BlockMap->Depth - 1 : slot->Position.y);
	slot->Position.z = slot->Position.z < 0 ? 0 : (slot->Position.z >= slot->BlockMap->Height ? slot->BlockMap->Height - 1 : slot->Position.z);
	return slot;
}

void BlockMapSlotAdd(BlockMapSlot slot, Entity entity)
{
	if (slot->Position.x >= 0 && slot->Position.y >= 0 && slot->Position.z >= 0)
	{
		int c = (slot->Position.z * slot->BlockMap->Depth + slot->Position.y) * slot->BlockMap->Width + slot->Position.x;
		slot->BlockMap->EntityGrid[c] = ListPush(slot->BlockMap->EntityGrid[c], &entity);
	}
}

void BlockMapSlotRemove(BlockMapSlot slot, Entity entity)
{
	if (slot->Position.x >= 0 && slot->Position.y >= 0 && slot->Position.z >= 0)
	{
		int c = (slot->Position.z * slot->BlockMap->Depth + slot->Position.y) * slot->BlockMap->Width + slot->Position.x;
		slot->BlockMap->EntityGrid[c] = ListRemoveAll(slot->BlockMap->EntityGrid[c], &entity);
	}
}

void BlockMapSlotDestroy(BlockMapSlot slot)
{
	MemoryFree(slot);
}
