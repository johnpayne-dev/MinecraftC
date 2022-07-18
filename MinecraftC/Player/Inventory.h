#pragma once
#include "../Level/Tile/Block.h"

typedef struct Inventory {
	BlockType slots[9];
	int selected;
} * Inventory;

Inventory InventoryCreate(void);
BlockType InventoryGetSelected(Inventory inventory);
void InventoryGrabTexture(Inventory inventory, BlockType tile);
void InventorySwapPaint(Inventory inventory, int slot);
void InventoryReplaceSlot(Inventory inventory, int sessionBlock);
void InventoryReplaceSlotWithBlock(Inventory inventory, Block block);
void InventoryDestroy(Inventory inventory);
