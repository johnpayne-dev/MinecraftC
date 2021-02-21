#pragma once
#include "../Utilities/LinearMath.h"
#include "../Utilities/List.h"
#include "Button.h"
#include "FontRenderer.h"

typedef enum GUIScreenType
{
	GUIScreenTypeNone,
	GUIScreenTypeBlockSelect,
	GUIScreenTypeChatInput,
	GUIScreenTypeControls,
	GUIScreenTypeError,
	GUIScreenTypeGameOver,
	GUIScreenTypeGenerateLevel,
	GUIScreenTypeLevelName,
	GUIScreenTypeLoadLevel,
	GUIScreenTypeOptions,
	GUIScreenTypePause,
	GUIScreenTypeSaveLevel,
} GUIScreenType;

typedef struct GUIScreen
{
	struct Minecraft * Minecraft;
	int Width, Height;
	list(Button) Buttons;
	bool GrabsMouse;
	FontRenderer Font;
	GUIScreenType Type;
	void * TypeData;
} * GUIScreen;

GUIScreen GUIScreenCreate(void);
void GUIScreenRender(GUIScreen screen, int2 mousePos);
void GUIScreenOnKeyPressed(GUIScreen screen, char eventChar, int eventKey);
void GUIScreenOnMouseClicked(GUIScreen screen, int x, int y, int button);
void GUIScreenOnButtonClicked(GUIScreen screen, Button button);
void GUIScreenOpen(GUIScreen screen, struct Minecraft * minecraft, int width, int height);
void GUIScreenOnOpen(GUIScreen screen);
void GUIScreenDoInput(GUIScreen screen, list(SDL_Event) events);
void GUIScreenMouseEvent(GUIScreen screen, SDL_Event event);
void GUIScreenKeyboardEvent(GUIScreen screen, SDL_Event event);
void GUIScreenTick(GUIScreen screen);
void GUIScreenOnClose(GUIScreen screen);
void GUIScreenDestroy(GUIScreen screen);
