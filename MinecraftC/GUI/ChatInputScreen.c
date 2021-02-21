#include <SDL2/SDL.h>
#include "ChatInputScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

ChatInputScreen ChatInputScreenCreate()
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeChatInput;
	screen->TypeData = MemoryAllocate(sizeof(struct ChatInputScreenData));
	ChatInputScreenData this = screen->TypeData;
	*this = (struct ChatInputScreenData)
	{
		.Message = StringCreate(""),
		.Counter = 0,
	};
	return screen;
}

void ChatInputScreenOnOpen(ChatInputScreen screen)
{
}

void ChatInputScreenOnClose(ChatInputScreen screen)
{
}

void ChatInputScreenTick(ChatInputScreen screen)
{
	ChatInputScreenData this = screen->TypeData;
	this->Counter++;
}

void ChatInputScreenRender(ChatInputScreen screen, int2 mousePos)
{
	ChatInputScreenData this = screen->TypeData;
	ScreenDrawBox((int2){ 2, screen->Height - 14 }, (int2){ screen->Width, screen->Height } - 2, ColorFromHex(0x00000080));
	String msg = StringConcat(StringConcatFront("> ", StringCreate(this->Message)), this->Counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->Font, msg, (int2){ 4, screen->Height - 12 }, ColorFromHex(0xE0E0E0FF));
	StringDestroy(msg);
}

void ChatInputScreenOnKeyPressed(ChatInputScreen screen, char eventChar, int eventKey)
{
	ChatInputScreenData this = screen->TypeData;
	if (eventKey == SDL_SCANCODE_ESCAPE) { MinecraftSetCurrentScreen(screen->Minecraft, NULL); }
	else if (eventKey == SDL_SCANCODE_RETURN)
	{
		/*NetworkManager var10000 = this.minecraft.networkManager;
		 String var4 = this.message.trim();
		 NetworkManager var3 = var10000;
		 if((var4 = var4.trim()).length() > 0) {
		    var3.netHandler.send(PacketType.CHAT_MESSAGE, new Object[]{Integer.valueOf(-1), var4});
		 }*/
		MinecraftSetCurrentScreen(screen->Minecraft, NULL);
	}
	else
	{
		if (eventKey == SDL_SCANCODE_BACKSPACE
		    && StringLength(this->Message) > 0) { this->Message = StringSub(this->Message, 0, StringLength(this->Message) - 1); }
		String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\\\"#%/()=+?[]{}<>@|$;");
		if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(this->Message) < 64 - (2))
		{
			this->Message = StringConcat(this->Message, (char[]){ eventChar, '\0' });
		}
		StringDestroy(allowedChars);
	}
}

void ChatInputScreenOnMouseClicked(ChatInputScreen screen, int x, int y, int button)
{
	ChatInputScreenData this = screen->TypeData;
	if (button == SDL_BUTTON_LEFT && screen->Minecraft->HUD->HoveredPlayer != NULL)
	{
		if (StringLength(this->Message) > 0 && this->Message[StringLength(this->Message) - 1] != ' ') { this->Message = StringConcat(this->Message, " "); }
		this->Message = StringConcat(this->Message, (char *)screen->Minecraft->HUD->HoveredPlayer);
		int len = 64 - (StringLength(screen->Minecraft->Session->UserName) + 2);
		if (StringLength(this->Message) > len) { this->Message = StringSub(this->Message, 0, len); }
	}
}

void ChatInputScreenDestroy(ChatInputScreen screen)
{
	ChatInputScreenData this = screen->TypeData;
	StringDestroy(this->Message);
	MemoryFree(this);
}
