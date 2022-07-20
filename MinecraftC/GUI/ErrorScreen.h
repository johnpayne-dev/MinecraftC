#pragma once
#include "GUIScreen.h"

typedef GUIScreen ErrorScreen;

typedef struct ErrorScreenData ErrorScreenData;

void ErrorScreenCreate(ErrorScreen * screen, char * title, char * text);
void ErrorScreenOnOpen(ErrorScreen * screen);
void ErrorScreenRender(ErrorScreen * screen, int mx, int my);
void ErrorScreenOnKeyPressed(ErrorScreen * screen, char eventChar, int eventKey);
