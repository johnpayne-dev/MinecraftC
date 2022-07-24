#include "ChatLine.h"
#include <stdlib.h>

void ChatLineCreate(ChatLine * line, char * message) {
	*line = (ChatLine) {
		.message = StringCreate(message),
		.time = 0,
	};
}

void ChatLineDestroy(ChatLine * line) {
	StringFree(line->message);
}
