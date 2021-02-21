#pragma once
#include <stdbool.h>
#include "../Utilities/LinearMath.h"

extern struct ShapeRenderer
{
	float * Buffer;
	int Vertices;
	float2 UV;
	float3 RGB;
	bool HasColor;
	bool HasTexture;
	int Length;
	int Counter;
	bool NoColor;
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
