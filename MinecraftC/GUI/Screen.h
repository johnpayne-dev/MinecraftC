#pragma once
#include "../Utilities/LinearMath.h"
#include "FontRenderer.h"

void ScreenDrawBox(int2 v0, int2 v1, Color color);
void ScreenDrawFadingBox(int2 v0, int2 v1, Color col0, Color col1);
void ScreenDrawCenteredString(FontRenderer font, char * str, int2 pos, Color color);
void ScreenDrawString(FontRenderer font, char * str, int2 pos, Color color);
void ScreenDrawImage(int2 v0, int2 v1, int2 uv, float imgZ);
