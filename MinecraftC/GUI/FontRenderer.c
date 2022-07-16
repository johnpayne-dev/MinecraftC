#include <string.h>
#include "FontRenderer.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/String.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

#include "../../Resources/Default.h"

FontRenderer FontRendererCreate(GameSettings settings, char * name, TextureManager textures) {
	FontRenderer font = MemoryAllocate(sizeof(struct FontRenderer));
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
	return font;
}

static void Render(FontRenderer font, char * str, int x, int y, Color color, bool darken) {
	if (str != NULL) {
		if (darken) { color.rgb >>= 2; }
		glBindTexture(GL_TEXTURE_2D, font->texture);
		ShapeRendererBegin();
		ShapeRendererColor(ColorToFloat4(color).rgb);
		for (int i = 0, w = 0; i < strlen(str); i++) {
			if (str[i] == 38 && strlen(str) > i + 1) {
				String hex = StringCreate("0123456789abcdef");
				int index = StringIndexOf(hex, str[i + 1]);
				StringDestroy(hex);
				if (index < 0) { index = 15; }
				int3 col = { 0 };
				col.r = (index & 8) << 3;
				col.b = (index & 1) * 191 + col.r;
				col.g = ((index & 2) >> 1) * 191 + col.r;
				col.r = ((index & 4) >> 2) * 191 + col.r;
				if (font->settings->anaglyph) { col.rgb = (int3){ col.r * 30 + col.g * 59 + col.b * 11, col.r * 30 + col.g * 70, col.r * 30 + col.b * 70 } / 100; }
				Color color = ColorFromHex(col.r << 24 | col.g << 16 | col.b << 8);
				i += 2;
				if (darken) { color.rgb >>= 2; }
				ShapeRendererColor(ColorToFloat4(color).rgb);
			}
			
			int u = str[i] % 16 << 3;
			int v = str[i] / 16 << 3;
			float s = 7.99;
			ShapeRendererVertexUV((float3){ x + w, y + s, 0.0 }, (float2){ u, v + s } / 128.0);
			ShapeRendererVertexUV((float3){ x + w + s, y + s, 0.0 }, (float2){ u + s, v + s } / 128.0);
			ShapeRendererVertexUV((float3){ x + w + s, y, 0.0 }, (float2){ u + s, v } / 128.0);
			ShapeRendererVertexUV((float3){ x + w, y, 0.0 }, (float2){ u, v } / 128.0);
			w += font->widthMap[str[i]];
		}
		ShapeRendererEnd();
	}
}

void FontRendererRender(FontRenderer font, char * str, int x, int y, Color color) {
	Render(font, str, x + 1, y + 1, color, true);
	Render(font, str, x, y, color, false);
}

void FontRendererRenderNoShadow(FontRenderer font, char * str, int x, int y, Color color) {
	Render(font, str, x, y, color, false);
}

int FontRendererGetWidth(FontRenderer font, char * str) {
	if (str == NULL) {
		return 0;
	} else {
		int width = 0;
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] == 38) { i++; }
			else { width += font->widthMap[str[i]]; }
		}
		return width;
	}
}

String FontRendererStripColor(char * str) {
	String string = StringCreate(str);
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == 38) { i++; }
		else { string = StringConcat(string, (char[]){ str[i], '\0' }); }
	}
	return string;
}

void FontRendererDestroy(FontRenderer font) {
	MemoryFree(font);
}
