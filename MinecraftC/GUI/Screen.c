#include <OpenGL.h>
#include "Screen.h"
#include "../Render/ShapeRenderer.h"

void ScreenDrawBox(int2 v0, int2 v1, Color color)
{
	float4 col = ColorToFloat4(color);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(col.r, col.g, col.b, col.a);
	ShapeRendererBegin();
	ShapeRendererVertex((float3){ v0.x, v1.y, 0.0 });
	ShapeRendererVertex((float3){ v1.x, v1.y, 0.0 });
	ShapeRendererVertex((float3){ v1.x, v0.y, 0.0 });
	ShapeRendererVertex((float3){ v0.x, v0.y, 0.0 });
	ShapeRendererEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void ScreenDrawFadingBox(int2 v0, int2 v1, Color col0, Color col1)
{
	float4 c0 = ColorToFloat4(col0);
	float4 c1 = ColorToFloat4(col1);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(c0.r, c0.g, c0.b, c0.a);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v0.x, v0.y);
	glColor4f(c1.r, c1.g, c1.b, c1.a);
	glVertex2f(v0.x, v1.y);
	glVertex2f(v1.x, v1.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void ScreenDrawCenteredString(FontRenderer font, char * str, int2 pos, Color color)
{
	FontRendererRender(font, str, pos.x - FontRendererGetWidth(font, str) / 2, pos.y, color);
}

void ScreenDrawString(FontRenderer font, char * str, int2 pos, Color color)
{
	FontRendererRender(font, str, pos.x, pos.y, color);
}

void ScreenDrawImage(int2 v0, int2 v1, int2 uv, float imgZ)
{
	float2 s = one2f * 0.00390625;
	ShapeRendererBegin();
	ShapeRendererVertexUV((float3){ v0.x, v0.y + uv.y, imgZ }, (float2){ v1.x, v1.y + uv.y } * s);
	ShapeRendererVertexUV((float3){ v0.x + uv.x, v0.y + uv.y, imgZ }, (float2){ v1.x + uv.x, v1.y + uv.y } * s);
	ShapeRendererVertexUV((float3){ v0.x + uv.x, v0.y, imgZ }, (float2){ v1.x + uv.x, v1.y } * s);
	ShapeRendererVertexUV((float3){ v0.x, v0.y, imgZ }, (float2){ v1.x, v1.y } * s);
	ShapeRendererEnd();
}
