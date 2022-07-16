#include "ShapeRenderer.h"
#include "../Utilities/Memory.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

struct ShapeRenderer ShapeRenderer = { 0 };
static int MaxFloats = 524288;

void ShapeRendererInitialize() {
	ShapeRenderer.buffer = MemoryAllocate(MaxFloats * sizeof(float));
	ShapeRenderer.vertices = 0;
	ShapeRenderer.hasColor = false;
	ShapeRenderer.hasTexture = false;
	ShapeRenderer.vertexLength = 3;
	ShapeRenderer.length = 0;
	ShapeRenderer.noColor = false;
}

void ShapeRendererBegin() {
	ShapeRendererClear();
	ShapeRenderer.hasTexture = false;
	ShapeRenderer.hasColor = false;
	ShapeRenderer.noColor = false;
}

void ShapeRendererEnd() {
	if (ShapeRenderer.vertices > 0) {
		if (ShapeRenderer.hasTexture && ShapeRenderer.hasColor) { glInterleavedArrays(GL_T2F_C3F_V3F, 0, ShapeRenderer.buffer); }
		else if (ShapeRenderer.hasTexture) { glInterleavedArrays(GL_T2F_V3F, 0, ShapeRenderer.buffer); }
		else if (ShapeRenderer.hasColor) { glInterleavedArrays(GL_C3F_V3F, 0, ShapeRenderer.buffer); }
		else { glInterleavedArrays(GL_V3F, 0, ShapeRenderer.buffer); }
		
		glEnableClientState(GL_VERTEX_ARRAY);
		if (ShapeRenderer.hasTexture) { glEnableClientState(GL_TEXTURE_COORD_ARRAY); }
		if (ShapeRenderer.hasColor) { glEnableClientState(GL_COLOR_ARRAY); }
		
		glDrawArrays(GL_QUADS, 0, ShapeRenderer.vertices);
		glDisableClientState(GL_VERTEX_ARRAY);
		if (ShapeRenderer.hasTexture) { glDisableClientState(GL_TEXTURE_COORD_ARRAY); }
		if (ShapeRenderer.hasColor) { glDisableClientState(GL_COLOR_ARRAY); }
	}
	
	ShapeRendererClear();
}

void ShapeRendererClear() {
	ShapeRenderer.vertices = 0;
	ShapeRenderer.length = 0;
}

void ShapeRendererColor(float3 color) {
	if (!ShapeRenderer.noColor) {
		if (!ShapeRenderer.hasColor) { ShapeRenderer.vertexLength += 3; }
		ShapeRenderer.hasColor = true;
		ShapeRenderer.rgb = color;
	}
}

void ShapeRendererVertexUV(float3 vertex, float2 uv) {
	if (!ShapeRenderer.hasTexture) { ShapeRenderer.vertexLength += 2; }
	ShapeRenderer.hasTexture = true;
	ShapeRenderer.uv = uv;
	ShapeRendererVertex(vertex);
}

void ShapeRendererVertex(float3 vertex) {
	if (ShapeRenderer.hasTexture) {
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.uv.x;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.uv.y;
	}
	
	if (ShapeRenderer.hasColor) {
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.rgb.r;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.rgb.g;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.rgb.b;
	}
	
	ShapeRenderer.buffer[ShapeRenderer.length++] = vertex.x;
	ShapeRenderer.buffer[ShapeRenderer.length++] = vertex.y;
	ShapeRenderer.buffer[ShapeRenderer.length++] = vertex.z;
	
	ShapeRenderer.vertices++;
	if (ShapeRenderer.vertices % 4 == 0 && ShapeRenderer.length >= MaxFloats - (ShapeRenderer.vertexLength * 4)) {
		ShapeRendererEnd();
		ShapeRenderer.vertexLength = 3;
	}
}

void ShapeRendererNoColor() {
	ShapeRenderer.noColor = true;
}

void ShapeRendererNormal(float3 normal) {
	glNormal3f(normal.x, normal.y, normal.z);
}

void ShapeRendererDeinitialize() {
	MemoryFree(ShapeRenderer.buffer);
}
