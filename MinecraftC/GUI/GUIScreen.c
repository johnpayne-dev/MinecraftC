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

GUIScreen GUIScreenCreate() {
	GUIScreen screen = MemoryAllocate(sizeof(struct GUIScreen));
	*screen = (struct GUIScreen) {
		.buttons = ListCreate(sizeof(Button)),
		.grabsMouse = false,
	};
	return screen;
}

void GUIScreenRender(GUIScreen screen, int2 m) {
	if (screen->type == GUIScreenTypeBlockSelect) { BlockSelectScreenRender(screen, m); return; }
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenRender(screen, m); return; }
	if (screen->type == GUIScreenTypeControls) { ControlsScreenRender(screen, m); }
	if (screen->type == GUIScreenTypeError) { ErrorScreenRender(screen, m); }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenRender(screen, m); }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenRender(screen, m); }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenRender(screen, m); return; }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenRender(screen, m); }
	if (screen->type == GUIScreenTypePause) { PauseScreenRender(screen, m); }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenRender(screen, m); return; }
	
	for (int i = 0; i < ListCount(screen->buttons); i++) {
		Button button = screen->buttons[i];
		if (button->visible) {
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(screen->minecraft->textureManager, "GUI/GUI.png"));
			glColor4f(1.0, 1.0, 1.0, 1.0);
			bool hovered = m.x >= button->position.x && m.y >= button->position.y && m.x < button->position.x + button->size.x && m.y < button->position.y + button->size.y;
			int state = 1;
			if (!button->active) { state = 0; }
			else if (hovered) { state = 2; }
			
			ScreenDrawImage(button->position, (int2){ 0, 46 + state * 20 }, button->size / (int2){ 2, 1 }, 0.0);
			ScreenDrawImage(button->position + (int2){ button->size.x / 2, 0 }, (int2){ 200 - button->size.x / 2, 46 + state * 20 }, button->size / (int2){ 2, 1 }, 0.0);
			if (!button->active) { ScreenDrawCenteredString(screen->font, button->text, button->position + (int2){ button->size.x / 2, (button->size.y - 8) / 2 }, ColorFromHex(0xa0a0a0ff)); }
			else if (hovered) { ScreenDrawCenteredString(screen->font, button->text, button->position + (int2){ button->size.x / 2, (button->size.y - 8) / 2 }, ColorFromHex(0xffffa0ff)); }
			else { ScreenDrawCenteredString(screen->font, button->text, button->position + (int2){ button->size.x / 2, (button->size.y - 8) / 2 }, ColorFromHex(0xe0e0e0ff)); }
		}
	}
}

void GUIScreenOnKeyPressed(GUIScreen screen, char eventChar, int eventKey) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeControls) { ControlsScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeError) { ErrorScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (eventKey == SDL_SCANCODE_ESCAPE) {
		MinecraftGrabMouse(screen->minecraft);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	}
}

void GUIScreenOnMouseClicked(GUIScreen screen, int x, int y, int button) {
	if (screen->type == GUIScreenTypeBlockSelect) { BlockSelectScreenOnMouseClicked(screen, x, y, button); return; }
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnMouseClicked(screen, x, y, button); return; }
	if (button == SDL_BUTTON_LEFT) {
		for (int i = 0; i < ListCount(screen->buttons); i++) {
			Button button = screen->buttons[i];
			if (button->active && x >= button->position.x && y >= button->position.y && x < button->position.x + button->size.x && y < button->position.y + button->size.y) { GUIScreenOnButtonClicked(screen, button); }
		}
	}
}

void GUIScreenOnButtonClicked(GUIScreen screen, Button button) {
	if (screen->type == GUIScreenTypeControls) { ControlsScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypePause) { PauseScreenOnButtonClicked(screen, button); return; }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
}

void GUIScreenOpen(GUIScreen screen, Minecraft minecraft, int width, int height) {
	screen->minecraft = minecraft;
	screen->font = minecraft->font;
	screen->width = width;
	screen->height = height;
	GUIScreenOnOpen(screen);
}

void GUIScreenOnOpen(GUIScreen screen) {
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

void GUIScreenDoInput(GUIScreen screen, list(SDL_Event) events) {
	for (int i = 0; i < ListCount(events); i++)
	{
		GUIScreenMouseEvent(screen, events[i]);
		GUIScreenKeyboardEvent(screen, events[i]);
	}
}

void GUIScreenMouseEvent(GUIScreen screen, SDL_Event event) {
	if (event.type == SDL_MOUSEBUTTONUP) {
		int x = event.button.x * screen->width / screen->minecraft->width;
		int y = event.button.y * screen->height / screen->minecraft->height - 1;
		GUIScreenOnMouseClicked(screen, x, y, event.button.button);
	}
}

void GUIScreenKeyboardEvent(GUIScreen screen, SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		GUIScreenOnKeyPressed(screen, event.key.keysym.sym, event.key.keysym.scancode);
	}
}

void GUIScreenTick(GUIScreen screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenTick(screen); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenTick(screen); return; }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenTick(screen); return; }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenTick(screen); return; }
}

void GUIScreenOnClose(GUIScreen screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenOnClose(screen); return; }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenOnClose(screen); return; }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnClose(screen); return; }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnClose(screen); return; }
}

void GUIScreenDestroy(GUIScreen screen) {
	if (screen->type == GUIScreenTypeChatInput) { ChatInputScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeError) { ErrorScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeLevelName) { LevelNameScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeLoadLevel) { LoadLevelScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeOptions) { OptionsScreenDestroy(screen); }
	if (screen->type == GUIScreenTypeSaveLevel) { LoadLevelScreenDestroy(screen); }
	for (int i = 0; i < ListCount(screen->buttons); i++) { ButtonDestroy(screen->buttons[i]); }
	ListDestroy(screen->buttons);
	MemoryFree(screen);
}
