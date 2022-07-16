#include "Inventory.h"
#include "../SessionData.h"

Inventory InventoryCreate() {
	Inventory inventory = MemoryAllocate(sizeof(struct Inventory));
	*inventory = (struct Inventory) {
		.selected = 0,
	};
	for (int i = 0; i < 9; i++) {
		inventory->slots[i] = -1;
		inventory->count[i] = 0;
	}
	return inventory;
}

BlockType InventoryGetSelected(Inventory inventory) {
	return inventory->slots[inventory->selected];
}

static int GetSlot(Inventory inventory, BlockType tile) {
	for (int i = 0; i < 9; i++) {
		if (tile == inventory->slots[i]) { return i; }
	}
	return -1;
}

void InventoryGrabTexture(Inventory inventory, BlockType tile) {
	int slot = GetSlot(inventory, tile);
	if (slot >= 0) { inventory->selected = slot; }
	else {
		if (tile != BlockTypeNone && ListContains(SessionDataAllowedBlocks, Blocks.table[tile])) { InventoryReplaceSlotWithBlock(inventory, Blocks.table[tile]); }
	}
}

void InventorySwapPaint(Inventory inventory, int slot) {
	slot = slot > 0 ? 1 : (slot < 0 ? -1 : slot);
	
	for (inventory->selected -= slot; inventory->selected < 0; inventory->selected += 9) { }
	while (inventory->selected >= 9) { inventory->selected -= 9; }
}

void InventoryReplaceSlot(Inventory inventory, int sessionBlock) {
	if (sessionBlock >= 0) { InventoryReplaceSlotWithBlock(inventory, SessionDataAllowedBlocks[sessionBlock]); }
}

void InventoryReplaceSlotWithBlock(Inventory inventory, Block block) {
	if (block != NULL) {
		int slot = GetSlot(inventory, block->type);
		if (slot >= 0) { inventory->slots[slot] = inventory->slots[inventory->selected]; }
		inventory->slots[inventory->selected] = block->type;
	}
}

bool InventoryAddResource(Inventory inventory, BlockType tile) {
	int slot = GetSlot(inventory, tile);
	if (slot < 0) { slot = GetSlot(inventory, -1); }
	if (slot < 0) { return false; }
	else if (inventory->count[slot] >= 99) { return false; }
	else {
		inventory->slots[slot] = tile;
		inventory->count[slot]++;
		inventory->popTime[slot] = 5;
		return true;
	}
}

void InventoryTick(Inventory inventory) {
	for (int i = 0; i < 9; i++) {
		if (inventory->popTime[i] > 0) { inventory->popTime[i]--; }
	}
}

bool InventoryRemoveResource(Inventory inventory, BlockType blockType) {
	int slot = GetSlot(inventory, blockType);
	if (slot < 0) { return false; }
	else {
		inventory->count[slot]--;
		if (inventory->count[slot] <= 0) { inventory->slots[slot] = -1; }
		return true;
	}
}

void InventoryDestroy(Inventory inventory) {
	MemoryFree(inventory);
}
