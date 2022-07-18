#pragma once
#include "Utilities/String.h"
#include "Level/Tile/Block.h"

typedef struct SessionData {
	String userName;
	char * sessionID;
	char * mpPass;
	bool hasPaid;
} * SessionData;

extern List(Block) SessionDataAllowedBlocks;
void SessionDataInitialize(void);
void SessionDataDeinitialize(void);

SessionData SessionDataCreate(char * userName, char * sessionID);
void SessionDataDestroy(SessionData data);
