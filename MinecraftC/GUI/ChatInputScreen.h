#pragma once
#include "GUIScreen.h"
#include "../Utilities/String.h"

typedef GUIScreen ChatInputScreen;

typedef struct ChatInputScreenData
{
	String Message;
	int Counter;
} * ChatInputScreenData;

ChatInputScreen ChatInputScreenCreate(void);
void ChatInputScreenOnOpen(ChatInputScreen screen);
void ChatInputScreenOnClose(ChatInputScreen screen);
void ChatInputScreenTick(ChatInputScreen screen);
void ChatInputScreenRender(ChatInputScreen screen, int2 mousePos);
void ChatInputScreenOnKeyPressed(ChatInputScreen screen, char eventChar, int eventKey);
void ChatInputScreenOnMouseClicked(ChatInputScreen screen, int x, int y, int button);
void ChatInputScreenDestroy(ChatInputScreen screen);
