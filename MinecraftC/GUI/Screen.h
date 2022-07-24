#pragma once
#include "FontRenderer.h"

void ScreenDrawBox(int x0, int y0, int x1, int y1, uint32_t color);
void ScreenDrawFadingBox(int x0, int y0, int x1, int y1, uint32_t col0, uint32_t col1);
void ScreenDrawCenteredString(FontRenderer * font, char * str, int x, int y, uint32_t color);
void ScreenDrawString(FontRenderer * font, char * str, int x, int y, uint32_t color);
void ScreenDrawImage(int x0, int y0, int x1, int y1, int u, int v, float imgZ);
