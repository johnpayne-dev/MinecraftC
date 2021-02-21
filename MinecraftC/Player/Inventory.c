#include "Inventory.h"
#include "../SessionData.h"

Inventory InventoryCreate()
{
	Inventory inventory = MemoryAllocate(sizeof(struct Inventory));
	*inventory = (struct Inventory)
	{
		.Selected = 0,
	};
	for (int i = 0; i < 9; i++)
	{
		inventory->Slots[i] = -1;
		inventory->Count[i] = 0;
	}
	return inventory;
}

BlockType InventoryGetSelected(Inventory inventory)
{
	return inventory->Slots[inventory->Selected];
}

static int GetSlot(Inventory inventory, BlockType tile)
{
	for (int i = 0; i < 9; i++)
	{
		if (tile == inventory->Slots[i]) { return i; }
	}
	return -1;
}

void InventoryGrabTexture(Inventory inventory, BlockType tile, bool creative)
{
	int slot = GetSlot(inventory, tile);
	if (slot >= 0) { inventory->Selected = slot; }
	else
	{
		if (creative && tile != BlockTypeNone && ListContains(SessionDataAllowedBlocks, Blocks.Table[tile])) { InventoryReplaceSlotWithBlock(inventory, Blocks.Table[tile]); }
	}
}

void InventorySwapPaint(Inventory inventory, int slot)
{
	slot = slot > 0 ? 1 : (slot < 0 ? -1 : slot);
	
	for (inventory->Selected -= slot; inventory->Selected < 0; inventory->Selected += 9) { }
	while (inventory->Selected >= 9) { inventory->Selected -= 9; }
}

void InventoryReplaceSlot(Inventory inventory, int sessionBlock)
{
	if (sessionBlock >= 0) { InventoryReplaceSlotWithBlock(inventory, SessionDataAllowedBlocks[sessionBlock]); }
}

void InventoryReplaceSlotWithBlock(Inventory inventory, Block block)
{
	if (block != NULL)
	{
		int slot = GetSlot(inventory, block->Type);
		if (slot >= 0) { inventory->Slots[slot] = inventory->Slots[inventory->Selected]; }
		inventory->Slots[inventory->Selected] = block->Type;
	}
}

bool InventoryAddResource(Inventory inventory, BlockType tile)
{
	int slot = GetSlot(inventory, tile);
	if (slot < 0) { slot = GetSlot(inventory, -1); }
	if (slot < 0) { return false; }
	else if (inventory->Count[slot] >= 99) { return false; }
	else
	{
		inventory->Slots[slot] = tile;
		inventory->Count[slot]++;
		inventory->PopTime[slot] = 5;
		return true;
	}
}

void InventoryTick(Inventory inventory)
{
	for (int i = 0; i < 9; i++)
	{
		if (inventory->PopTime[i] > 0) { inventory->PopTime[i]--; }
	}
}

bool InventoryRemoveResource(Inventory inventory, BlockType blockType)
{
	int slot = GetSlot(inventory, blockType);
	if (slot < 0) { return false; }
	else
	{
		inventory->Count[slot]--;
		if (inventory->Count[slot] <= 0) { inventory->Slots[slot] = -1; }
		return true;
	}
}

void InventoryDestroy(Inventory inventory)
{
	MemoryFree(inventory);
}
