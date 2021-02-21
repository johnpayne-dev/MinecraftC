#include "ChatLine.h"

ChatLine ChatLineCreate(char * message)
{
	ChatLine line = MemoryAllocate(sizeof(struct ChatLine));
	*line = (struct ChatLine)
	{
		.Message = StringCreate(message),
		.Time = 0,
	};
	return line;
}

void ChatLineDestroy(ChatLine line)
{
	StringDestroy(line->Message);
	MemoryFree(line);
}
