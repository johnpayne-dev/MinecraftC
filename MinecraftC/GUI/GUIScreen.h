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
	List(Button) buttons;
	bool grabsMouse;
	FontRenderer * font;
	GUIScreenType type;
	union {
		struct ChatInputScreenData {
			String message;
			int counter;
		} chatInput;
		struct ControlsScreenData {
			struct GUIScreen * parent;
			char * title;
			GameSettings * settings;
			int selected;
		} controls;
		struct ErrorScreenData {
			char * title;
			char * text;
		} error;
		struct GenerateLevelScreenData {
			struct GUIScreen * parent;
		} generateLevel;
		struct LevelNameScreenData {
			struct GUIScreen * parent;
			char * title;
			int id;
			String name;
			int counter;
		} levelName;
		struct LoadLevelScreenData {
			struct GUIScreen * parent;
			char * title;
			String names[5];
		} level;
		struct OptionsScreenData {
			struct GUIScreen * parent;
			char * title;
			GameSettings * settings;
		} options;
	};
} GUIScreen;

void GUIScreenCreate(GUIScreen * screen);
void GUIScreenRender(GUIScreen * screen, int mouseX, int mouseY);
void GUIScreenOnKeyPressed(GUIScreen * screen, char eventChar, int eventKey);
void GUIScreenOnMouseClicked(GUIScreen * screen, int x, int y, int button);
void GUIScreenOnButtonClicked(GUIScreen * screen, Button * button);
void GUIScreenOpen(GUIScreen * screen, struct Minecraft * minecraft, int width, int height);
void GUIScreenOnOpen(GUIScreen * screen);
void GUIScreenDoInput(GUIScreen * screen, List(SDL_Event) events);
void GUIScreenMouseEvent(GUIScreen * screen, SDL_Event event);
void GUIScreenKeyboardEvent(GUIScreen * screen, SDL_Event event);
void GUIScreenTick(GUIScreen * screen);
void GUIScreenOnClose(GUIScreen * screen);
void GUIScreenDestroy(GUIScreen * screen);
