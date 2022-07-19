#pragma once
#include "Utilities/String.h"

typedef struct ChatLine {
	String message;
	int time;
} ChatLine;

void ChatLineCreate(ChatLine * line, char * message);
void ChatLineDestroy(ChatLine * line);
