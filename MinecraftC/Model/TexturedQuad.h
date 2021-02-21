#pragma once
#include "Vertex.h"

typedef struct TexturedQuad
{
	Vertex Vertices[4];
} TexturedQuad;

TexturedQuad TexturedQuadCreate(Vertex vertices[4], int2 uv0, int2 uv1);
TexturedQuad TexturedQuadCreateUV(Vertex vertices[4], float2 uv0, float2 uv1);
