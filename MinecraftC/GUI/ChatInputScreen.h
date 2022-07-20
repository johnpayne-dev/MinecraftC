#pragma once
#include "GUIScreen.h"
#include "../Utilities/String.h"

typedef GUIScreen ChatInputScreen;

typedef struct ChatInputScreenData ChatInputScreenData;

void ChatInputScreenCreate(ChatInputScreen * screen);
void ChatInputScreenOnOpen(ChatInputScreen * screen);
void ChatInputScreenOnClose(ChatInputScreen * screen);
void ChatInputScreenTick(ChatInputScreen * screen);
void ChatInputScreenRender(ChatInputScreen * screen, int mx, int my);
void ChatInputScreenOnKeyPressed(ChatInputScreen * screen, char eventChar, int eventKey);
void ChatInputScreenOnMouseClicked(ChatInputScreen * screen, int x, int y, int button);
void ChatInputScreenDestroy(ChatInputScreen * screen);
