#pragma once
#include <stdbool.h>
#include <stdint.h>

extern struct ShapeRenderer {
	float * buffer;
	int vertices;
	float u, v;
	float r, g, b;
	bool hasColor;
	bool hasTexture;
	int vertexLength;
	int length;
	bool noColor;
} ShapeRenderer;

void ShapeRendererInitialize(void);
void ShapeRendererBegin(void);
void ShapeRendererEnd(void);
void ShapeRendererClear(void);
void ShapeRendererColor(uint32_t color);
void ShapeRendererColorf(float r, float g, float b);
void ShapeRendererVertexUV(float x, float y, float z, float u, float v);
void ShapeRendererVertex(float x, float y, float z);
void ShapeRendererNoColor(void);
void ShapeRendererNormal(float nx, float ny, float nz);
void ShapeRendererDeinitialize(void);
