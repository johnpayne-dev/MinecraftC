#pragma once
#include "../Utilities/LinearMath.h"

typedef struct Vertex
{
	float3 Position;
	float2 UV;
} Vertex;

static inline Vertex VertexRemap(Vertex vertex, float2 uv) { return (Vertex){ .Position = vertex.Position, .UV = uv }; }
