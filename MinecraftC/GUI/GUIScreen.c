#include "GUIScreen.h"
#include "Screen.h"
#include "BlockSelectScreen.h"
#include "ChatInputScreen.h"
#include "ControlsScreen.h"
#include "ErrorScreen.h"
#include "GenerateLevelScreen.h"
#include "LevelNameScreen.h"
#include "LoadLevelScreen.h"
#include "OptionsScreen.h"
#include "PauseScreen.h"
#include "SaveLevelScreen.h"
#include "../Minecraft.h"
#include "../Utilities/OpenGL.h"

void GUIScreenCreate(GUIScreen * screen) {
	*screen = (GUIScreen) {
		.buttons = ListCreate(sizeof(Button)),
		.grabsMouse = false,
	};
}

void GUIScreenRender(GUIScreen * screen, int mx, int my) {
	if (screen->type == GUIScreenTypeBlockSelect) { BlockSelectScreenRender(screen, mx, my); return; }
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenRender(screen, mx, my); return; }
	if (screen->type == GUIScreenTypeControls) { ControlsScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypeError) { ErrorScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenRender(screen, mx, my); return; }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypePause) { PauseScreenRender(screen, mx, my); }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenRender(screen, mx, my); return; }
	
	for (int i = 0; i < ListLength(screen->buttons); i++) {
		Button * button = &screen->buttons[i];
		if (button->visible) {
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&screen->minecraft->textureManager, "GUI/GUI.png"));
			glColor4f(1.0, 1.0, 1.0, 1.0);
			bool hovered = mx >= button->x && my >= button->y && mx < button->x + button->width && my < button->y + button->height;
			int state = 1;
			if (!button->active) { state = 0; }
			else if (hovered) { state = 2; }
			
			ScreenDrawImage(button->x, button->y, 0, 46 + state * 20, button->width / 2, button->height, 0.0);
			ScreenDrawImage(button->x + button->width / 2, button->y, 200 - button->width / 2, 46 + state * 20, button->width / 2, button->height, 0.0);
			if (!button->active) {
				ScreenDrawCenteredString(screen->font, button->text, button->x + button->width / 2, button->y + (button->height - 8) / 2, 0xa0a0a0ff);
			} else if (hovered) {
				ScreenDrawCenteredString(screen->font, button->text, button->x + button->width / 2, button->y + (button->height - 8) / 2, 0xffffa0ff);
			} else {
				ScreenDrawCenteredString(screen->font, button->text, button->x + button->width / 2, button->y + (button->height - 8) / 2, 0xe0e0e0ff);
			}
		}
	}
}

void GUIScreenOnKeyPressed(GUIScreen * screen, char eventChar, int eventKey) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeControls) { ControlsScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeError) { ErrorScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (eventKey == SDL_SCANCODE_ESCAPE) {
		MinecraftGrabMouse(screen->minecraft);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	}
}

void GUIScreenOnMouseClicked(GUIScreen * screen, int x, int y, int button) {
	if (screen->type == GUIScreenTypeBlockSelect) { BlockSelectScreenOnMouseClicked(screen, x, y, button); return; }
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnMouseClicked(screen, x, y, button); return; }
	if (button == SDL_BUTTON_LEFT) {
		for (int i = 0; i < ListLength(screen->buttons); i++) {
			Button * button = &screen->buttons[i];
			if (button->active && x >= button->x && y >= button->y && x < button->x + button->width && y < button->y + button->height) {
				GUIScreenOnButtonClicked(screen, button);
			}
		}
	}
}

void GUIScreenOnButtonClicked(GUIScreen * screen, Button * button) {
	if (screen->type == GUIScreenTypeControls) { ControlsScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypePause) { PauseScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
}

void GUIScreenOpen(GUIScreen * screen, Minecraft * minecraft, int width, int height) {
	screen->minecraft = minecraft;
	screen->font = &minecraft->font;
	screen->width = width;
	screen->height = height;
	GUIScreenOnOpen(screen);
}

void GUIScreenOnOpen(GUIScreen * screen) {
	for (int i = 0; i < ListLength(screen->buttons); i++) { ButtonDestroy(&screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeControls) { ControlsScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeError) { ErrorScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypePause) { PauseScreenOnOpen(screen); return; }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnOpen(screen); return; }
}

void GUIScreenDoInput(GUIScreen * screen, List(SDL_Event) events) {
	for (int i = 0; i < ListLength(events); i++) {
		GUIScreenMouseEvent(screen, events[i]);
		GUIScreenKeyboardEvent(screen, events[i]);
	}
}

void GUIScreenMouseEvent(GUIScreen * screen, SDL_Event event) {
	if (event.type == SDL_MOUSEBUTTONUP) {
		int x = event.button.x * screen->width / screen->minecraft->width;
		int y = event.button.y * screen->height / screen->minecraft->height - 1;
		GUIScreenOnMouseClicked(screen, x, y, event.button.button);
	}
}

void GUIScreenKeyboardEvent(GUIScreen * screen, SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		GUIScreenOnKeyPressed(screen, '\0', event.key.keysym.scancode);
	} else if (event.type == SDL_TEXTINPUT) {
		GUIScreenOnKeyPressed(screen, event.text.text[0], '\0');
	}
}

void GUIScreenTick(GUIScreen * screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenTick(screen); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenTick(screen); return; }
}

void GUIScreenOnClose(GUIScreen * screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnClose(screen); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnClose(screen); return; }
}

void GUIScreenDestroy(GUIScreen * screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenDestroy(screen); }
	for (int i = 0; i < ListLength(screen->buttons); i++) { ButtonDestroy(&screen->buttons[i]); }
	ListFree(screen->buttons);
}
