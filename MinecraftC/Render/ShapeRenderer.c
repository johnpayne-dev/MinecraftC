#include <OpenGL.h>
#include "ShapeRenderer.h"
#include "../Utilities/Memory.h"

struct ShapeRenderer ShapeRenderer = { 0 };
static int MaxFloats = 524288;

void ShapeRendererInitialize()
{
	ShapeRenderer.Buffer = MemoryAllocate(MaxFloats * sizeof(float));
	ShapeRenderer.Length = 3;
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
	ShapeRenderer.Counter = 0;
}

void ShapeRendererColor(float3 color)
{
	if (!ShapeRenderer.NoColor)
	{
		if (!ShapeRenderer.HasColor) { ShapeRenderer.Length += 3; }
		ShapeRenderer.HasColor = true;
		ShapeRenderer.RGB = color;
	}
}

void ShapeRendererVertexUV(float3 vertex, float2 uv)
{
	if (!ShapeRenderer.HasTexture) { ShapeRenderer.Length += 2; }
	ShapeRenderer.HasTexture = true;
	ShapeRenderer.UV = uv;
	ShapeRendererVertex(vertex);
}

void ShapeRendererVertex(float3 vertex)
{
	if (ShapeRenderer.HasTexture)
	{
		ShapeRenderer.Buffer[ShapeRenderer.Counter++] = ShapeRenderer.UV.x;
		ShapeRenderer.Buffer[ShapeRenderer.Counter++] = ShapeRenderer.UV.y;
	}
	
	if (ShapeRenderer.HasColor)
	{
		ShapeRenderer.Buffer[ShapeRenderer.Counter++] = ShapeRenderer.RGB.r;
		ShapeRenderer.Buffer[ShapeRenderer.Counter++] = ShapeRenderer.RGB.g;
		ShapeRenderer.Buffer[ShapeRenderer.Counter++] = ShapeRenderer.RGB.b;
	}
	
	ShapeRenderer.Buffer[ShapeRenderer.Counter++] = vertex.x;
	ShapeRenderer.Buffer[ShapeRenderer.Counter++] = vertex.y;
	ShapeRenderer.Buffer[ShapeRenderer.Counter++] = vertex.z;
	
	ShapeRenderer.Vertices++;
	if (ShapeRenderer.Vertices % 4 == 0 && ShapeRenderer.Counter >= MaxFloats - ShapeRenderer.Length * 4)
	{
		ShapeRendererEnd();
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
