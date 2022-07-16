#include "ChatLine.h"

ChatLine ChatLineCreate(char * message) {
	ChatLine line = MemoryAllocate(sizeof(struct ChatLine));
	*line = (struct ChatLine) {
		.message = StringCreate(message),
		.time = 0,
	};
	return line;
}

void ChatLineDestroy(ChatLine line) {
	StringDestroy(line->message);
	MemoryFree(line);
}
