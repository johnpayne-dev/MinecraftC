#include <OpenGL.h>
#include "ShapeRenderer.h"
#include "../Utilities/Memory.h"
#include "../Utilities/Log.h"

struct ShapeRenderer ShapeRenderer = { 0 };
static int MaxFloats = 524288;

void ShapeRendererInitialize()
{
	ShapeRenderer.Buffer = MemoryAllocate(MaxFloats * sizeof(float));
	ShapeRenderer.Vertices = 0;
	ShapeRenderer.HasColor = false;
	ShapeRenderer.HasTexture = false;
	ShapeRenderer.VertexLength = 3;
	ShapeRenderer.Length = 0;
	ShapeRenderer.NoColor = false;
}

void ShapeRendererBegin()
{
	ShapeRendererClear();
	ShapeRenderer.HasTexture = false;
	ShapeRenderer.HasColor = false;
	ShapeRenderer.NoColor = false;
}

void ShapeRendererEnd()
{
	if (ShapeRenderer.Vertices > 0)
	{
		if (ShapeRenderer.HasTexture && ShapeRenderer.HasColor) { glInterleavedArrays(GL_T2F_C3F_V3F, 0, ShapeRenderer.Buffer); }
		else if (ShapeRenderer.HasTexture) { glInterleavedArrays(GL_T2F_V3F, 0, ShapeRenderer.Buffer); }
		else if (ShapeRenderer.HasColor) { glInterleavedArrays(GL_C3F_V3F, 0, ShapeRenderer.Buffer); }
		else { glInterleavedArrays(GL_V3F, 0, ShapeRenderer.Buffer); }
		
		glEnableClientState(GL_VERTEX_ARRAY);
		if (ShapeRenderer.HasTexture) { glEnableClientState(GL_TEXTURE_COORD_ARRAY); }
		if (ShapeRenderer.HasColor) { glEnableClientState(GL_COLOR_ARRAY); }
		
		glDrawArrays(GL_QUADS, 0, ShapeRenderer.Vertices);
		glDisableClientState(GL_VERTEX_ARRAY);
		if (ShapeRenderer.HasTexture) { glDisableClientState(GL_TEXTURE_COORD_ARRAY); }
		if (ShapeRenderer.HasColor) { glDisableClientState(GL_COLOR_ARRAY); }
	}
	
	ShapeRendererClear();
}

void ShapeRendererClear()
{
	ShapeRenderer.Vertices = 0;
	ShapeRenderer.Length = 0;
}

void ShapeRendererColor(float3 color)
{
	if (!ShapeRenderer.NoColor)
	{
		if (!ShapeRenderer.HasColor) { ShapeRenderer.VertexLength += 3; }
		ShapeRenderer.HasColor = true;
		ShapeRenderer.RGB = color;
	}
}

void ShapeRendererVertexUV(float3 vertex, float2 uv)
{
	if (!ShapeRenderer.HasTexture) { ShapeRenderer.VertexLength += 2; }
	ShapeRenderer.HasTexture = true;
	ShapeRenderer.UV = uv;
	ShapeRendererVertex(vertex);
}

void ShapeRendererVertex(float3 vertex)
{
	if (ShapeRenderer.HasTexture)
	{
		ShapeRenderer.Buffer[ShapeRenderer.Length++] = ShapeRenderer.UV.x;
		ShapeRenderer.Buffer[ShapeRenderer.Length++] = ShapeRenderer.UV.y;
	}
	
	if (ShapeRenderer.HasColor)
	{
		ShapeRenderer.Buffer[ShapeRenderer.Length++] = ShapeRenderer.RGB.r;
		ShapeRenderer.Buffer[ShapeRenderer.Length++] = ShapeRenderer.RGB.g;
		ShapeRenderer.Buffer[ShapeRenderer.Length++] = ShapeRenderer.RGB.b;
	}
	
	ShapeRenderer.Buffer[ShapeRenderer.Length++] = vertex.x;
	ShapeRenderer.Buffer[ShapeRenderer.Length++] = vertex.y;
	ShapeRenderer.Buffer[ShapeRenderer.Length++] = vertex.z;
	
	ShapeRenderer.Vertices++;
	if (ShapeRenderer.Vertices % 4 == 0 && ShapeRenderer.Length >= MaxFloats - (ShapeRenderer.VertexLength * 4))
	{
		ShapeRendererEnd();
		ShapeRenderer.VertexLength = 3;
	}
}

void ShapeRendererNoColor()
{
	ShapeRenderer.NoColor = true;
}

void ShapeRendererNormal(float3 normal)
{
	glNormal3f(normal.x, normal.y, normal.z);
}

void ShapeRendererDeinitialize()
{
	MemoryFree(ShapeRenderer.Buffer);
}
