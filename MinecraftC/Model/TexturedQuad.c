#include "TexturedQuad.h"

TexturedQuad TexturedQuadCreate(Vertex vertices[4], int2 uv0, int2 uv1)
{
	float2 a = { 0.0015625, 0.003125 };
	return TexturedQuadCreateUV(vertices, float2i(uv0) / (float2){ 64.0, 32.0 } + a, float2i(uv1) / (float2){ 64.0, 32.0 } - a);
}

TexturedQuad TexturedQuadCreateUV(Vertex vertices[4], float2 uv0, float2 uv1)
{
	TexturedQuad quad = { 0 };
	quad.Vertices[0] = VertexRemap(vertices[0], (float2){ uv1.x, uv0.y });
	quad.Vertices[1] = VertexRemap(vertices[1], (float2){ uv0.x, uv0.y });
	quad.Vertices[2] = VertexRemap(vertices[2], (float2){ uv0.x, uv1.y });
	quad.Vertices[3] = VertexRemap(vertices[3], (float2){ uv1.x, uv1.y });
	return quad;
}
