#include "Screen.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/OpenGL.h"

void ScreenDrawBox(int x0, int y0, int x1, int y1, uint32_t color) {
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f((color >> 24) / 255.0, ((color >> 16) & 0xff) / 255.0, ((color >> 8) & 0xff) / 255.0, (color & 0xff) / 255.0);
	ShapeRendererBegin();
	ShapeRendererVertex(x0, y1, 0.0);
	ShapeRendererVertex(x1, y1, 0.0);
	ShapeRendererVertex(x1, y0, 0.0);
	ShapeRendererVertex(x0, y0, 0.0);
	ShapeRendererEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void ScreenDrawFadingBox(int x0, int y0, int x1, int y1, uint32_t col0, uint32_t col1) {
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f((col0 >> 24) / 255.0, ((col0 >> 16) & 0xff) / 255.0, ((col0 >> 8) & 0xff) / 255.0, (col0 & 0xff) / 255.0);
	glVertex2f(x1, y0);
	glVertex2f(x0, y0);
	glColor4f((col1 >> 24) / 255.0, ((col1 >> 16) & 0xff) / 255.0, ((col1 >> 8) & 0xff) / 255.0, (col1 & 0xff) / 255.0);
	glVertex2f(x0, y1);
	glVertex2f(x1, y1);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void ScreenDrawCenteredString(FontRenderer * font, char * str, int x, int y, uint32_t color) {
	FontRendererRender(font, str, x - FontRendererGetWidth(font, str) / 2, y, color);
}

void ScreenDrawString(FontRenderer * font, char * str, int x, int y, uint32_t color) {
	FontRendererRender(font, str, x, y, color);
}

void ScreenDrawImage(int x0, int y0, int x1, int y1, int u, int v, float imgZ) {
	float s = 0.00390625;
	ShapeRendererBegin();
	ShapeRendererVertexUV(x0, y0 + v, imgZ, x1 * s, (y1 + v) * s);
	ShapeRendererVertexUV(x0 + u, y0 + v, imgZ, (x1 + u) * s, (y1 + v) * s);
	ShapeRendererVertexUV(x0 + u, y0, imgZ, (x1 + u) * s, y1 * s);
	ShapeRendererVertexUV(x0, y0, imgZ, x1 * s, y1 * s);
	ShapeRendererEnd();
}
