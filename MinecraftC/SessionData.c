#include "SessionData.h"

list(Block) SessionDataAllowedBlocks = NULL;

void SessionDataInitialize()
{
	SessionDataAllowedBlocks = ListCreate(sizeof(Block));
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeCobbleStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeBrick]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeDirt]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeWood]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeLog]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeLeaves]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGlass]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeSlab]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeMossyCobbleStone]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeSapling]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeDandelion]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeRose]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeBrownMushroom]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeRedMushroom]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeSand]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGravel]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeSponge]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeRedWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeOrangeWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeYellowWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeLimeWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGreenWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeAquaGreenWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeCyanWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeBlueWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypePurpleWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeIndigoWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeVioletWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeMagentaWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypePinkWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeBlackWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGrayWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeWhiteWool]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeCoalOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeIronOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGoldOre]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeIron]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeGold]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeBookshelf]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeTNT]);
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &Blocks.Table[BlockTypeObsidian]);
}

void SessionDataDeinitialize()
{
	ListDestroy(SessionDataAllowedBlocks);
}

SessionData SessionDataCreate(char * userName, char * sessionID)
{
	SessionData session = MemoryAllocate(sizeof(struct SessionData));
	*session = (struct SessionData)
	{
		.UserName = StringCreate(userName),
		.SessionID = sessionID,
	};
	return session;
}

void SessionDataDestroy(SessionData session)
{
	StringDestroy(session->UserName);
	MemoryFree(session);
}
