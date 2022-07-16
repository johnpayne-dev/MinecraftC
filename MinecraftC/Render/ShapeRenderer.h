#pragma once
#include <stdbool.h>
#include "../Utilities/LinearMath.h"

extern struct ShapeRenderer {
	float * buffer;
	int vertices;
	float2 uv;
	float3 rgb;
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
void ShapeRendererColor(float3 color);
void ShapeRendererVertexUV(float3 vertex, float2 uv);
void ShapeRendererVertex(float3 vertex);
void ShapeRendererNoColor(void);
void ShapeRendererNormal(float3 normal);
void ShapeRendererDeinitialize(void);
