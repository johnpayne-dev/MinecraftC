#pragma once
#include <stdbool.h>
#include "../Render/TextureManager.h"
#include "../Utilities/LinearMath.h"
#include "../Utilities/String.h"

typedef struct FontRenderer
{
	int WidthMap[256];
	int Texture;
	GameSettings Settings;
} * FontRenderer;

FontRenderer FontRendererCreate(GameSettings settings, char * name, TextureManager textures);
void FontRendererRender(FontRenderer font, char * str, int x, int y, Color color);
void FontRendererRenderNoShadow(FontRenderer font, char * str, int x, int y, Color color);
int FontRendererGetWidth(FontRenderer font, char * str);
String FontRendererStripColor(char * str);
void FontRendererDestroy(FontRenderer font);
