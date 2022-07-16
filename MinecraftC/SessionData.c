#include "SessionData.h"

list(Block) SessionDataAllowedBlocks = NULL;

void SessionDataInitialize() {
	SessionDataAllowedBlocks = ListCreate(sizeof(Block));
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeCobbleStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeBrick]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeDirt]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeWood]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeLog]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeLeaves]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGlass]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeSlab]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeMossyCobbleStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeSapling]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeDandelion]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeRose]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeBrownMushroom]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeRedMushroom]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeSand]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGravel]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeSponge]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeRedWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeOrangeWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeYellowWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeLimeWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGreenWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeAquaGreenWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeCyanWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeBlueWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypePurpleWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeIndigoWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeVioletWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeMagentaWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypePinkWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeBlackWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGrayWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeWhiteWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeCoalOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeIronOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGoldOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeIron]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeGold]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeBookshelf]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeTNT]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.table[BlockTypeObsidian]);
}

void SessionDataDeinitialize() {
	ListDestroy(SessionDataAllowedBlocks);
}

SessionData SessionDataCreate(char * userName, char * sessionID) {
	SessionData session = MemoryAllocate(sizeof(struct SessionData));
	*session = (struct SessionData) {
		.userName = StringCreate(userName),
		.sessionID = sessionID,
	};
	return session;
}

void SessionDataDestroy(SessionData session) {
	StringDestroy(session->userName);
	MemoryFree(session);
}
