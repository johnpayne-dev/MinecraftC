#pragma once
#include "../Utilities/LinearMath.h"
#include "../Utilities/String.h"

typedef struct Button {
	int2 position;
	int2 size;
	String text;
	int id;
	bool active;
	bool visible;
} * Button;

Button ButtonCreate(int buttonID, int x, int y, char * text);
Button ButtonCreateSize(int buttonID, int x, int y, int w, int h, char * text);
void ButtonDestroy(Button button);
