#pragma once
#include "../Level/Tile/Block.h"

typedef struct Inventory
{
	BlockType Slots[9];
	int Count[9];
	int PopTime[9];
	int Selected;
} * Inventory;

Inventory InventoryCreate(void);
BlockType InventoryGetSelected(Inventory inventory);
void InventoryGrabTexture(Inventory inventory, BlockType tile);
void InventorySwapPaint(Inventory inventory, int slot);
void InventoryReplaceSlot(Inventory inventory, int sessionBlock);
void InventoryReplaceSlotWithBlock(Inventory inventory, Block block);
bool InventoryAddResource(Inventory inventory, BlockType tile);
void InventoryTick(Inventory inventory);
bool InventoryRemoveResource(Inventory inventory, BlockType blockType);
void InventoryDestroy(Inventory inventory);
