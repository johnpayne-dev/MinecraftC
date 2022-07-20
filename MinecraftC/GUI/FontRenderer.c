#include <string.h>
#include "FontRenderer.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/String.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

#include "../../Resources/Default.h"

void FontRendererCreate(FontRenderer * font, GameSettings * settings, char * name, TextureManager * textures) {
	*font = (FontRenderer){ 0 };
	font->settings = settings;
	font->textureName = name;
	
	int width = 0;
	int * pixels = NULL;
	if (strcmp(name, "Default.png") == 0) { pixels = (int *)Resource_Default_RGBA; width = Resource_Default_Width; }
	if (pixels == NULL) { LogFatal("Failed to load image %s.\n", name); }
	
	for (int i = 0; i < 128; i++) {
		int xt = i % 16;
		int yt = i / 16;
		int x = 0;
		for (bool emptyColumn = false; x < 8 && !emptyColumn; x++) {
			int xPixel = xt * 8 + x;
			emptyColumn = true;
			for (int y = 0; y < 8 && emptyColumn; y++) {
				int yPixel = (yt * 8 + y) * width;
				int pixel = pixels[xPixel + yPixel] & 0xff;
				if (pixel > 128) { emptyColumn = false; }
			}
		}
		
		if (i == 32) { x = 4; }
		font->widthMap[i] = x;
	}
	font->texture = TextureManagerLoad(textures, name);
}

static void Render(FontRenderer * font, char * str, int x, int y, uint32_t color, bool darken) {
	if (str != NULL) {
		if (darken) { color = (color & 0xfcfcfc) >> 2; }
		glBindTexture(GL_TEXTURE_2D, font->texture);
		ShapeRendererBegin();
		ShapeRendererColor(color);
		for (int i = 0, w = 0; i < strlen(str); i++) {
			if (str[i] == 38 && strlen(str) > i + 1) {
				String hex = StringCreate("0123456789abcdef");
				int index = StringIndexOf(hex, str[i + 1]);
				StringFree(hex);
				if (index < 0) { index = 15; }
				uint8_t v = (index & 0x08) << 3;
				uint8_t r = ((index & 0x04) >> 2) * 191 + v;
				uint8_t g = ((index & 0x02) >> 1) * 191 + v;
				uint8_t b = (index & 0x01) * 191 + v;
				if (font->settings->anaglyph) {
					uint8_t ar = (r * 30 + g * 59 + b * 11) / 100;
					uint8_t ag = (r * 30 + g * 70) / 100;
					uint8_t ab = (r * 30 + b * 70) / 100;
					r = ar;
					g = ag;
					b = ab;
				}
				color = r << 24 | g << 16 | b << 8;
				i += 2;
				if (darken) { color = (color & 0xfcfcfc) >> 2; }
				ShapeRendererColor(color);
			}
			
			int u = str[i] % 16 << 3;
			int v = str[i] / 16 << 3;
			float s = 7.99;
			ShapeRendererVertexUV(x + w, y + s, 0.0, u / 128.0, (v + s) / 128.0);
			ShapeRendererVertexUV(x + w + s, y + s, 0.0, (u + s) / 128.0, (v + s) / 128.0);
			ShapeRendererVertexUV(x + w + s, y, 0.0, (u + s) / 128.0, v / 128.0);
			ShapeRendererVertexUV(x + w, y, 0.0, u / 128.0 , v / 128.0);
			w += font->widthMap[(int)str[i]];
		}
		ShapeRendererEnd();
	}
}

void FontRendererRender(FontRenderer * font, char * str, int x, int y, uint32_t color) {
	Render(font, str, x + 1, y + 1, color, true);
	Render(font, str, x, y, color, false);
}

void FontRendererRenderNoShadow(FontRenderer * font, char * str, int x, int y, uint32_t color) {
	Render(font, str, x, y, color, false);
}

int FontRendererGetWidth(FontRenderer * font, char * str) {
	if (str == NULL) {
		return 0;
	} else {
		int width = 0;
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] == 38) { i++; }
			else { width += font->widthMap[(int)str[i]]; }
		}
		return width;
	}
}

String FontRendererStripColor(char * str) {
	String string = StringCreate(str);
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == 38) { i++; }
		else { StringConcat(&string, (char[]){ str[i], '\0' }); }
	}
	return string;
}
