#pragma once
#include "Utilities/String.h"

typedef struct ChatLine {
	String message;
	int time;
} * ChatLine;

ChatLine ChatLineCreate(char * message);
void ChatLineDestroy(ChatLine line);
