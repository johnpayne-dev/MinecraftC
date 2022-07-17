#pragma once
#include "../Utilities/List.h"
#include "Button.h"
#include "FontRenderer.h"

typedef enum GUIScreenType {
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

typedef struct GUIScreen {
	struct Minecraft * minecraft;
	int width, height;
	list(Button) buttons;
	bool grabsMouse;
	FontRenderer font;
	GUIScreenType type;
	void * typeData;
} * GUIScreen;

GUIScreen GUIScreenCreate(void);
void GUIScreenRender(GUIScreen screen, int mouseX, int mouseY);
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
