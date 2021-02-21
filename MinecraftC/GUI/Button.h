#pragma once
#include "../Utilities/LinearMath.h"
#include "../Utilities/String.h"

typedef struct Button
{
	int2 Position;
	int2 Size;
	String Text;
	int ID;
	bool Active;
	bool Visible;
} * Button;

Button ButtonCreate(int buttonID, int x, int y, char * text);
Button ButtonCreateSize(int buttonID, int x, int y, int w, int h, char * text);
void ButtonDestroy(Button button);
