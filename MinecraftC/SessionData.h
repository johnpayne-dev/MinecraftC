#pragma once
#include "Utilities/String.h"
#include "Level/Tile/Block.h"

typedef struct SessionData
{
	String UserName;
	char * SessionID;
	char * MPPass;
	bool HasPaid;
} * SessionData;

extern list(Block) SessionDataAllowedBlocks;
void SessionDataInitialize(void);
void SessionDataDeinitialize(void);

SessionData SessionDataCreate(char * userName, char * sessionID);
void SessionDataDestroy(SessionData data);
