#include "SessionData.h"

List(BlockType) SessionDataAllowedBlocks = NULL;

void SessionDataInitialize() {
	SessionDataAllowedBlocks = ListCreate(sizeof(BlockType));
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeStone });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeCobbleStone });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeBrick });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeDirt });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeWood });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeLog });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeLeaves });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGlass });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeSlab });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeMossyCobbleStone });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeSapling });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeDandelion });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeRose });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeBrownMushroom });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeRedMushroom });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeSand });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGravel });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeSponge });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeRedWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeOrangeWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeYellowWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeLimeWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGreenWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeAquaGreenWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeCyanWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeBlueWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypePurpleWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeIndigoWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeVioletWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeMagentaWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypePinkWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeBlackWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGrayWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeWhiteWool });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeCoalOre });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeIronOre });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGoldOre });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeIron });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeGold });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeBookshelf });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeTNT });
	SessionDataAllowedBlocks = ListPush(SessionDataAllowedBlocks, &(BlockType){ BlockTypeObsidian });
}

void SessionDataFree() {
	ListFree(SessionDataAllowedBlocks);
}
