#include <SDL2/SDL.h>
#include "ChatInputScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

ChatInputScreen ChatInputScreenCreate() {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeChatInput;
	screen->typeData = MemoryAllocate(sizeof(struct ChatInputScreenData));
	ChatInputScreenData this = screen->typeData;
	*this = (struct ChatInputScreenData) {
		.message = StringCreate(""),
		.counter = 0,
	};
	return screen;
}

void ChatInputScreenOnOpen(ChatInputScreen screen) {
}

void ChatInputScreenOnClose(ChatInputScreen screen) {
}

void ChatInputScreenTick(ChatInputScreen screen)
{
	ChatInputScreenData this = screen->typeData;
	this->counter++;
}

void ChatInputScreenRender(ChatInputScreen screen, int2 mousePos) {
	ChatInputScreenData this = screen->typeData;
	ScreenDrawBox((int2){ 2, screen->height - 14 }, (int2){ screen->width, screen->height } - 2, ColorFromHex(0x00000080));
	String msg = StringConcat(StringConcatFront("> ", StringCreate(this->message)), this->counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->font, msg, (int2){ 4, screen->height - 12 }, ColorFromHex(0xE0E0E0FF));
	StringDestroy(msg);
}

void ChatInputScreenOnKeyPressed(ChatInputScreen screen, char eventChar, int eventKey) {
	ChatInputScreenData this = screen->typeData;
	if (eventKey == SDL_SCANCODE_ESCAPE) { MinecraftSetCurrentScreen(screen->minecraft, NULL); }
	else if (eventKey == SDL_SCANCODE_RETURN) {
		/*NetworkManager var10000 = this.minecraft.networkManager;
		 String var4 = this.message.trim();
		 NetworkManager var3 = var10000;
		 if((var4 = var4.trim()).length() > 0) {
		    var3.netHandler.send(PacketType.CHAT_MESSAGE, new Object[]{Integer.valueOf(-1), var4});
		 }*/
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	} else {
		if (eventKey == SDL_SCANCODE_BACKSPACE
		    && StringLength(this->message) > 0) { this->message = StringSub(this->message, 0, StringLength(this->message) - 1); }
		String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\\\"#%/()=+?[]{}<>@|$;");
		if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(this->message) < 64 - (2)) {
			this->message = StringConcat(this->message, (char[]){ eventChar, '\0' });
		}
		StringDestroy(allowedChars);
	}
}

void ChatInputScreenOnMouseClicked(ChatInputScreen screen, int x, int y, int button) {
	ChatInputScreenData this = screen->typeData;
	if (button == SDL_BUTTON_LEFT && screen->minecraft->hud->hoveredPlayer != NULL) {
		if (StringLength(this->message) > 0 && this->message[StringLength(this->message) - 1] != ' ') { this->message = StringConcat(this->message, " "); }
		this->message = StringConcat(this->message, (char *)screen->minecraft->hud->hoveredPlayer);
		int len = 64 - (StringLength(screen->minecraft->session->userName) + 2);
		if (StringLength(this->message) > len) { this->message = StringSub(this->message, 0, len); }
	}
}

void ChatInputScreenDestroy(ChatInputScreen screen) {
	ChatInputScreenData this = screen->typeData;
	StringDestroy(this->message);
	MemoryFree(this);
}
