#include "ShapeRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"
#include <stdlib.h>

struct ShapeRenderer ShapeRenderer = { 0 };
static int MaxFloats = 524288;

void ShapeRendererInitialize() {
	ShapeRenderer.buffer = malloc(MaxFloats * sizeof(float));
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

void ShapeRendererColorf(float r, float g, float b) {
	if (!ShapeRenderer.noColor) {
		if (!ShapeRenderer.hasColor) { ShapeRenderer.vertexLength += 3; }
		ShapeRenderer.hasColor = true;
		ShapeRenderer.r = r;
		ShapeRenderer.g = g;
		ShapeRenderer.b = b;
	}
}

void ShapeRendererColor(uint32_t c) {
	ShapeRendererColorf((c >> 24) / 255.0, ((c >> 16) & 0xff) / 255.0, ((c >> 8) & 0xff) / 255.0);
}

void ShapeRendererVertexUV(float x, float y, float z, float u, float v) {
	if (!ShapeRenderer.hasTexture) { ShapeRenderer.vertexLength += 2; }
	ShapeRenderer.hasTexture = true;
	ShapeRenderer.u = u;
	ShapeRenderer.v = v;
	ShapeRendererVertex(x, y, z);
}

void ShapeRendererVertex(float x, float y, float z) {
	if (ShapeRenderer.hasTexture) {
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.u;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.v;
	}
	
	if (ShapeRenderer.hasColor) {
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.r;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.g;
		ShapeRenderer.buffer[ShapeRenderer.length++] = ShapeRenderer.b;
	}
	
	ShapeRenderer.buffer[ShapeRenderer.length++] = x;
	ShapeRenderer.buffer[ShapeRenderer.length++] = y;
	ShapeRenderer.buffer[ShapeRenderer.length++] = z;
	
	ShapeRenderer.vertices++;
	if (ShapeRenderer.vertices % 4 == 0 && ShapeRenderer.length >= MaxFloats - (ShapeRenderer.vertexLength * 4)) {
		ShapeRendererEnd();
		ShapeRenderer.vertexLength = 3;
	}
}

void ShapeRendererNoColor() {
	ShapeRenderer.noColor = true;
}

void ShapeRendererNormal(float nx, float ny, float nz) {
	glNormal3f(nx, ny, nz);
}

void ShapeRendererDeinitialize() {
	free(ShapeRenderer.buffer);
}
