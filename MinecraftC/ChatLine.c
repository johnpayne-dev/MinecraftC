#include "ChatLine.h"
#include <stdlib.h>

ChatLine ChatLineCreate(char * message) {
	ChatLine line = malloc(sizeof(struct ChatLine));
	*line = (struct ChatLine) {
		.message = StringCreate(message),
		.time = 0,
	};
	return line;
}

void ChatLineDestroy(ChatLine line) {
	StringFree(line->message);
	free(line);
}
