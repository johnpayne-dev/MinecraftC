#pragma once
#include "Utilities/String.h"

typedef struct ChatLine
{
	String Message;
	int Time;
} * ChatLine;

ChatLine ChatLineCreate(char * message);
void ChatLineDestroy(ChatLine line);
