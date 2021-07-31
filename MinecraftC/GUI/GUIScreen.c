#include <OpenGL.h>
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

GUIScreen GUIScreenCreate()
{
	GUIScreen screen = MemoryAllocate(sizeof(struct GUIScreen));
	*screen = (struct GUIScreen)
	{
		.Buttons = ListCreate(sizeof(Button)),
		.GrabsMouse = false,
	};
	return screen;
}

void GUIScreenRender(GUIScreen screen, int2 m)
{
	if (screen->Type == GUIScreenTypeBlockSelect) { BlockSelectScreenRender(screen, m); return; }
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenRender(screen, m); return; }
	if (screen->Type == GUIScreenTypeControls) { ControlsScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeError) { ErrorScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenRender(screen, m); return; }
	if (screen->Type == GUIScreenTypeOptions) { OptionsScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypePause) { PauseScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenRender(screen, m); return; }
	
	for (int i = 0; i < ListCount(screen->Buttons); i++)
	{
		Button button = screen->Buttons[i];
		if (button->Visible)
		{
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(screen->Minecraft->TextureManager, "GUI/GUI.png"));
			glColor4f(1.0, 1.0, 1.0, 1.0);
			bool hovered = m.x >= button->Position.x && m.y >= button->Position.y && m.x < button->Position.x + button->Size.x && m.y < button->Position.y + button->Size.y;
			int state = 1;
			if (!button->Active) { state = 0; }
			else if (hovered) { state = 2; }
			
			ScreenDrawImage(button->Position, (int2){ 0, 46 + state * 20 }, button->Size / (int2){ 2, 1 }, 0.0);
			ScreenDrawImage(button->Position + (int2){ button->Size.x / 2, 0 }, (int2){ 200 - button->Size.x / 2, 46 + state * 20 }, button->Size / (int2){ 2, 1 }, 0.0);
			if (!button->Active) { ScreenDrawCenteredString(screen->Font, button->Text, button->Position + (int2){ button->Size.x / 2, (button->Size.y - 8) / 2 }, ColorFromHex(0xa0a0a0ff)); }
			else if (hovered) { ScreenDrawCenteredString(screen->Font, button->Text, button->Position + (int2){ button->Size.x / 2, (button->Size.y - 8) / 2 }, ColorFromHex(0xffffa0ff)); }
			else { ScreenDrawCenteredString(screen->Font, button->Text, button->Position + (int2){ button->Size.x / 2, (button->Size.y - 8) / 2 }, ColorFromHex(0xe0e0e0ff)); }
		}
	}
}

void GUIScreenOnKeyPressed(GUIScreen screen, char eventChar, int eventKey)
{
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->Type == GUIScreenTypeControls) { ControlsScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->Type == GUIScreenTypeError) { ErrorScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenOnKeyPressed(screen, eventChar, eventKey); return; }
	if (eventKey == SDL_SCANCODE_ESCAPE)
	{
		MinecraftGrabMouse(screen->Minecraft);
		MinecraftSetCurrentScreen(screen->Minecraft, NULL);
	}
}

void GUIScreenOnMouseClicked(GUIScreen screen, int x, int y, int button)
{
	if (screen->Type == GUIScreenTypeBlockSelect) { BlockSelectScreenOnMouseClicked(screen, x, y, button); return; }
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenOnMouseClicked(screen, x, y, button); return; }
	if (button == SDL_BUTTON_LEFT)
	{
		for (int i = 0; i < ListCount(screen->Buttons); i++)
		{
			Button button = screen->Buttons[i];
			if (button->Active && x >= button->Position.x && y >= button->Position.y && x < button->Position.x + button->Size.x && y < button->Position.y + button->Size.y) { GUIScreenOnButtonClicked(screen, button); }
		}
	}
}

void GUIScreenOnButtonClicked(GUIScreen screen, Button button)
{
	if (screen->Type == GUIScreenTypeControls) { ControlsScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypeOptions) { OptionsScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypePause) { PauseScreenOnButtonClicked(screen, button); return; }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnButtonClicked(screen, button); return; }
}

void GUIScreenOpen(GUIScreen screen, Minecraft minecraft, int width, int height)
{
	screen->Minecraft = minecraft;
	screen->Font = minecraft->Font;
	screen->Width = width;
	screen->Height = height;
	GUIScreenOnOpen(screen);
}

void GUIScreenOnOpen(GUIScreen screen)
{
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeControls) { ControlsScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeError) { ErrorScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeOptions) { OptionsScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypePause) { PauseScreenOnOpen(screen); return; }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnOpen(screen); return; }
}

void GUIScreenDoInput(GUIScreen screen, list(SDL_Event) events)
{
	for (int i = 0; i < ListCount(events); i++)
	{
		GUIScreenMouseEvent(screen, events[i]);
		GUIScreenKeyboardEvent(screen, events[i]);
	}
}

void GUIScreenMouseEvent(GUIScreen screen, SDL_Event event)
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		int x = event.button.x * screen->Width / screen->Minecraft->Width;
		int y = event.button.y * screen->Height / screen->Minecraft->Height - 1;
		GUIScreenOnMouseClicked(screen, x, y, event.button.button);
	}
}

void GUIScreenKeyboardEvent(GUIScreen screen, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		GUIScreenOnKeyPressed(screen, event.key.keysym.sym, event.key.keysym.scancode);
	}
}

void GUIScreenTick(GUIScreen screen)
{
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenTick(screen); return; }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenTick(screen); return; }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenTick(screen); return; }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenTick(screen); return; }
}

void GUIScreenOnClose(GUIScreen screen)
{
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenOnClose(screen); return; }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenOnClose(screen); return; }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenOnClose(screen); return; }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenOnClose(screen); return; }
}

void GUIScreenDestroy(GUIScreen screen)
{
	if (screen->Type == GUIScreenTypeChatInput) { ChatInputScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeError) { ErrorScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeLoadLevel) { LoadLevelScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeOptions) { OptionsScreenDestroy(screen); }
	if (screen->Type == GUIScreenTypeSaveLevel) { LoadLevelScreenDestroy(screen); }
	for (int i = 0; i < ListCount(screen->Buttons); i++) { ButtonDestroy(screen->Buttons[i]); }
	ListDestroy(screen->Buttons);
	MemoryFree(screen);
}
