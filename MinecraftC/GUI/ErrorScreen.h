#pragma once
#include "GUIScreen.h"

typedef GUIScreen ErrorScreen;

typedef struct ErrorScreenData {
	char * title;
	char * text;
} * ErrorScreenData;

ErrorScreen ErrorScreenCreate(char * title, char * text);
void ErrorScreenOnOpen(ErrorScreen screen);
void ErrorScreenRender(ErrorScreen screen, int mx, int my);
void ErrorScreenOnKeyPressed(ErrorScreen screen, char eventChar, int eventKey);
void ErrorScreenDestroy(ErrorScreen screen);
