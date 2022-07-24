#include "Inventory.h"
#include "../SessionData.h"

void InventoryCreate(Inventory * inventory) {
	*inventory = (Inventory) {
		.selected = 0,
	};
	for (int i = 0; i < 9; i++) {
		inventory->slots[i] = -1;
	}
}

BlockType InventoryGetSelected(Inventory * inventory) {
	return inventory->slots[inventory->selected];
}

static int GetSlot(Inventory * inventory, BlockType tile) {
	for (int i = 0; i < 9; i++) {
		if (tile == inventory->slots[i]) { return i; }
	}
	return -1;
}

void InventoryGrabTexture(Inventory * inventory, BlockType tile) {
	int slot = GetSlot(inventory, tile);
	if (slot >= 0) { inventory->selected = slot; }
	else {
		if (tile != BlockTypeNone && ListContains(SessionDataAllowedBlocks, &(BlockType){ Blocks.table[tile].type })) {
			InventoryReplaceSlotWithBlock(inventory, &Blocks.table[tile]);
		}
	}
}

void InventorySwapPaint(Inventory * inventory, int slot) {
	slot = slot > 0 ? 1 : (slot < 0 ? -1 : slot);
	
	for (inventory->selected -= slot; inventory->selected < 0; inventory->selected += 9) { }
	while (inventory->selected >= 9) { inventory->selected -= 9; }
}

void InventoryReplaceSlot(Inventory * inventory, int sessionBlock) {
	if (sessionBlock >= 0) { InventoryReplaceSlotWithBlock(inventory, &Blocks.table[SessionDataAllowedBlocks[sessionBlock]]); }
}

void InventoryReplaceSlotWithBlock(Inventory * inventory, Block * block) {
	if (block != NULL) {
		int slot = GetSlot(inventory, block->type);
		if (slot >= 0) { inventory->slots[slot] = inventory->slots[inventory->selected]; }
		inventory->slots[inventory->selected] = block->type;
	}
}
