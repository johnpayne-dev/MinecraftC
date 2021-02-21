#include <OpenGL.h>
#include "GUIScreen.h"
#include "Screen.h"
#include "BlockSelectScreen.h"
#include "ChatInputScreen.h"
#include "ControlsScreen.h"
#include "ErrorScreen.h"
#include "GameOverScreen.h"
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
	if (screen->Type == GUIScreenTypeBlockSelect) { return BlockSelectScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeControls) { ControlsScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeError) { ErrorScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeGameOver) { GameOverScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeGenerateLevel) { GenerateLevelScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeLevelName) { LevelNameScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeLoadLevel) { return LoadLevelScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeOptions) { OptionsScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypePause) { PauseScreenRender(screen, m); }
	if (screen->Type == GUIScreenTypeSaveLevel) { return LoadLevelScreenRender(screen, m); }
	
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
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenOnKeyPressed(screen, eventChar, eventKey); }
	if (screen->Type == GUIScreenTypeControls) { return ControlsScreenOnKeyPressed(screen, eventChar, eventKey); }
	if (screen->Type == GUIScreenTypeError) { return ErrorScreenOnKeyPressed(screen, eventChar, eventKey); }
	if (screen->Type == GUIScreenTypeLevelName) { return LevelNameScreenOnKeyPressed(screen, eventChar, eventKey); }
	if (eventKey == SDL_SCANCODE_ESCAPE)
	{
		MinecraftGrabMouse(screen->Minecraft);
		MinecraftSetCurrentScreen(screen->Minecraft, NULL);
	}
}

void GUIScreenOnMouseClicked(GUIScreen screen, int x, int y, int button)
{
	if (screen->Type == GUIScreenTypeBlockSelect) { return BlockSelectScreenOnMouseClicked(screen, x, y, button); }
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenOnMouseClicked(screen, x, y, button); }
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
	if (screen->Type == GUIScreenTypeControls) { return ControlsScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeGameOver) { return GameOverScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeGenerateLevel) { return GenerateLevelScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeLevelName) { return LevelNameScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeLoadLevel) { return LoadLevelScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeOptions) { return OptionsScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypePause) { return PauseScreenOnButtonClicked(screen, button); }
	if (screen->Type == GUIScreenTypeSaveLevel) { return LoadLevelScreenOnButtonClicked(screen, button); }
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
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeControls) { return ControlsScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeError) { return ErrorScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeGameOver) { return GameOverScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeGenerateLevel) { return GenerateLevelScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeLevelName) { return LevelNameScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeLoadLevel) { return LoadLevelScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeOptions) { return OptionsScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypePause) { return PauseScreenOnOpen(screen); }
	if (screen->Type == GUIScreenTypeSaveLevel) { return LoadLevelScreenOnOpen(screen); }
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
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenTick(screen); }
	if (screen->Type == GUIScreenTypeLevelName) { return LevelNameScreenTick(screen); }
	if (screen->Type == GUIScreenTypeLoadLevel) { return LoadLevelScreenTick(screen); }
	if (screen->Type == GUIScreenTypeSaveLevel) { return LoadLevelScreenTick(screen); }
}

void GUIScreenOnClose(GUIScreen screen)
{
	if (screen->Type == GUIScreenTypeChatInput) { return ChatInputScreenOnClose(screen); }
	if (screen->Type == GUIScreenTypeLevelName) { return LevelNameScreenOnClose(screen); }
	if (screen->Type == GUIScreenTypeLoadLevel) { return LoadLevelScreenOnClose(screen); }
	if (screen->Type == GUIScreenTypeSaveLevel) { return LoadLevelScreenOnClose(screen); }
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
