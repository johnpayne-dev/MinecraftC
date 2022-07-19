#pragma once
#include <stdbool.h>
#include "../Render/TextureManager.h"
#include "../Utilities/String.h"

typedef struct FontRenderer {
	int widthMap[256];
	char * textureName;
	int texture;
	GameSettings * settings;
} * FontRenderer;

FontRenderer FontRendererCreate(GameSettings * settings, char * name, TextureManager * textures);
void FontRendererRender(FontRenderer font, char * str, int x, int y, uint32_t color);
void FontRendererRenderNoShadow(FontRenderer font, char * str, int x, int y, uint32_t color);
int FontRendererGetWidth(FontRenderer font, char * str);
String FontRendererStripColor(char * str);
void FontRendererDestroy(FontRenderer font);
