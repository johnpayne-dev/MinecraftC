#include <SDL.h>
#include "ChatInputScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void ChatInputScreenCreate(ChatInputScreen * screen) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeChatInput;
	screen->chatInput = (ChatInputScreenData) {
		.message = StringCreate(""),
		.counter = 0,
	};
}

void ChatInputScreenOnOpen(ChatInputScreen * screen) {
}

void ChatInputScreenOnClose(ChatInputScreen * screen) {
}

void ChatInputScreenTick(ChatInputScreen * screen) {
	screen->chatInput.counter++;
}

void ChatInputScreenRender(ChatInputScreen * screen, int mx, int my) {
	ScreenDrawBox(2, screen->height - 14, screen->width - 2, screen->height - 2, 0x00000080);
	String msg = StringCreate(screen->chatInput.message);
	StringConcatFront("> ", &msg);
	StringConcat(&msg, screen->chatInput.counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->font, msg, 4, screen->height - 12, 0xE0E0E0FF);
	StringFree(msg);
}

void ChatInputScreenOnKeyPressed(ChatInputScreen * screen, char eventChar, int eventKey) {
	if (eventKey == SDL_SCANCODE_ESCAPE) { MinecraftSetCurrentScreen(screen->minecraft, NULL); }
	else if (eventKey == SDL_SCANCODE_RETURN) {
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	} else {
		if (eventKey == SDL_SCANCODE_BACKSPACE && StringLength(screen->chatInput.message) > 0) {
			screen->chatInput.message = StringSub(screen->chatInput.message, 0, StringLength(screen->chatInput.message) - 2);
		}
		String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\\\"#%/()=+?[]{}<>@|$;");
		if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(screen->chatInput.message) < 64 - (2)) {
			StringConcat(&screen->chatInput.message, (char[]){ eventChar, '\0' });
		}
		StringFree(allowedChars);
	}
}

void ChatInputScreenOnMouseClicked(ChatInputScreen * screen, int x, int y, int button) {
	if (button == SDL_BUTTON_LEFT && screen->minecraft->hud.hoveredPlayer != NULL) {
		if (StringLength(screen->chatInput.message) > 0 && screen->chatInput.message[StringLength(screen->chatInput.message) - 1] != ' ') { StringConcat(&screen->chatInput.message, " "); }
		StringConcat(&screen->chatInput.message, (char *)screen->minecraft->hud.hoveredPlayer);
		int len = 64;
		if (StringLength(screen->chatInput.message) > len) { screen->chatInput.message = StringSub(screen->chatInput.message, 0, len - 1); }
	}
}

void ChatInputScreenDestroy(ChatInputScreen * screen) {
	StringFree(screen->chatInput.message);
}
