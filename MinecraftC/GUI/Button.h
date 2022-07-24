#pragma once
#include <stdbool.h>
#include "../Utilities/String.h"

typedef struct Button {
	int x, y;
	int width, height;
	String text;
	int id;
	bool active;
	bool visible;
} Button;

void ButtonCreate(Button * button, int buttonID, int x, int y, char * text);
void ButtonCreateSize(Button * button, int buttonID, int x, int y, int w, int h, char * text);
void ButtonDestroy(Button * button);
