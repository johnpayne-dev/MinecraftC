#include "Frustum.h"
#include "../Utilities/OpenGL.h"

static void Normalize(Frustum * frustum, int plane) {
	float len = length3f((float3){ frustum->planes[plane][0], frustum->planes[plane][1], frustum->planes[plane][2] });
	for (int i = 0; i < 4; i++) { frustum->planes[plane][i] /= len; }
}

Frustum FrustumUpdate() {
	Frustum frustum = { 0 };
	glGetFloatv(GL_PROJECTION_MATRIX, frustum.projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, frustum.modelView);
	frustum.clip[0x0] = frustum.modelView[0x0] * frustum.projection[0x0] + frustum.modelView[0x1] * frustum.projection[0x4] + frustum.modelView[0x2] * frustum.projection[0x8] + frustum.modelView[0x3] * frustum.projection[0xC];
	frustum.clip[0x1] = frustum.modelView[0x0] * frustum.projection[0x1] + frustum.modelView[0x1] * frustum.projection[0x5] + frustum.modelView[0x2] * frustum.projection[0x9] + frustum.modelView[0x3] * frustum.projection[0xD];
	frustum.clip[0x2] = frustum.modelView[0x0] * frustum.projection[0x2] + frustum.modelView[0x1] * frustum.projection[0x6] + frustum.modelView[0x2] * frustum.projection[0xA] + frustum.modelView[0x3] * frustum.projection[0xE];
	frustum.clip[0x3] = frustum.modelView[0x0] * frustum.projection[0x3] + frustum.modelView[0x1] * frustum.projection[0x7] + frustum.modelView[0x2] * frustum.projection[0xB] + frustum.modelView[0x3] * frustum.projection[0xF];
	frustum.clip[0x4] = frustum.modelView[0x4] * frustum.projection[0x0] + frustum.modelView[0x5] * frustum.projection[0x4] + frustum.modelView[0x6] * frustum.projection[0x8] + frustum.modelView[0x7] * frustum.projection[0xC];
	frustum.clip[0x5] = frustum.modelView[0x4] * frustum.projection[0x1] + frustum.modelView[0x5] * frustum.projection[0x5] + frustum.modelView[0x6] * frustum.projection[0x9] + frustum.modelView[0x7] * frustum.projection[0xD];
	frustum.clip[0x6] = frustum.modelView[0x4] * frustum.projection[0x2] + frustum.modelView[0x5] * frustum.projection[0x6] + frustum.modelView[0x6] * frustum.projection[0xA] + frustum.modelView[0x7] * frustum.projection[0xE];
	frustum.clip[0x7] = frustum.modelView[0x4] * frustum.projection[0x3] + frustum.modelView[0x5] * frustum.projection[0x7] + frustum.modelView[0x6] * frustum.projection[0xB] + frustum.modelView[0x7] * frustum.projection[0xF];
	frustum.clip[0x8] = frustum.modelView[0x8] * frustum.projection[0x0] + frustum.modelView[0x9] * frustum.projection[0x4] + frustum.modelView[0xA] * frustum.projection[0x8] + frustum.modelView[0xB] * frustum.projection[0xC];
	frustum.clip[0x9] = frustum.modelView[0x8] * frustum.projection[0x1] + frustum.modelView[0x9] * frustum.projection[0x5] + frustum.modelView[0xA] * frustum.projection[0x9] + frustum.modelView[0xB] * frustum.projection[0xD];
	frustum.clip[0xA] = frustum.modelView[0x8] * frustum.projection[0x2] + frustum.modelView[0x9] * frustum.projection[0x6] + frustum.modelView[0xA] * frustum.projection[0xA] + frustum.modelView[0xB] * frustum.projection[0xE];
	frustum.clip[0xB] = frustum.modelView[0x8] * frustum.projection[0x3] + frustum.modelView[0x9] * frustum.projection[0x7] + frustum.modelView[0xA] * frustum.projection[0xB] + frustum.modelView[0xB] * frustum.projection[0xF];
	frustum.clip[0xC] = frustum.modelView[0xC] * frustum.projection[0x0] + frustum.modelView[0xD] * frustum.projection[0x4] + frustum.modelView[0xE] * frustum.projection[0x8] + frustum.modelView[0xF] * frustum.projection[0xC];
	frustum.clip[0xD] = frustum.modelView[0xC] * frustum.projection[0x1] + frustum.modelView[0xD] * frustum.projection[0x5] + frustum.modelView[0xE] * frustum.projection[0x9] + frustum.modelView[0xF] * frustum.projection[0xD];
	frustum.clip[0xE] = frustum.modelView[0xC] * frustum.projection[0x2] + frustum.modelView[0xD] * frustum.projection[0x6] + frustum.modelView[0xE] * frustum.projection[0xA] + frustum.modelView[0xF] * frustum.projection[0xE];
	frustum.clip[0xF] = frustum.modelView[0xC] * frustum.projection[0x3] + frustum.modelView[0xD] * frustum.projection[0x7] + frustum.modelView[0xE] * frustum.projection[0xB] + frustum.modelView[0xF] * frustum.projection[0xF];
	frustum.planes[0][0] = frustum.clip[0x3] - frustum.clip[0x0];
	frustum.planes[0][1] = frustum.clip[0x7] - frustum.clip[0x4];
	frustum.planes[0][2] = frustum.clip[0xB] - frustum.clip[0x8];
	frustum.planes[0][3] = frustum.clip[0xF] - frustum.clip[0xC];
	Normalize(&frustum, 0);
	frustum.planes[1][0] = frustum.clip[0x3] + frustum.clip[0x0];
	frustum.planes[1][1] = frustum.clip[0x7] + frustum.clip[0x4];
	frustum.planes[1][2] = frustum.clip[0xB] + frustum.clip[0x8];
	frustum.planes[1][3] = frustum.clip[0xF] + frustum.clip[0xC];
	Normalize(&frustum, 1);
	frustum.planes[2][0] = frustum.clip[0x3] - frustum.clip[0x1];
	frustum.planes[2][1] = frustum.clip[0x7] - frustum.clip[0x5];
	frustum.planes[2][2] = frustum.clip[0xB] - frustum.clip[0x9];
	frustum.planes[2][3] = frustum.clip[0xF] - frustum.clip[0xD];
	Normalize(&frustum, 2);
	frustum.planes[3][0] = frustum.clip[0x3] + frustum.clip[0x1];
	frustum.planes[3][1] = frustum.clip[0x7] + frustum.clip[0x5];
	frustum.planes[3][2] = frustum.clip[0xB] + frustum.clip[0x9];
	frustum.planes[3][3] = frustum.clip[0xF] + frustum.clip[0xD];
	Normalize(&frustum, 3);
	frustum.planes[4][0] = frustum.clip[0x3] - frustum.clip[0x2];
	frustum.planes[4][1] = frustum.clip[0x7] - frustum.clip[0x6];
	frustum.planes[4][2] = frustum.clip[0xB] - frustum.clip[0xA];
	frustum.planes[4][3] = frustum.clip[0xF] - frustum.clip[0xE];
	Normalize(&frustum, 4);
	frustum.planes[5][0] = frustum.clip[0x3] + frustum.clip[0x2];
	frustum.planes[5][1] = frustum.clip[0x7] + frustum.clip[0x6];
	frustum.planes[5][2] = frustum.clip[0xB] + frustum.clip[0xA];
	frustum.planes[5][3] = frustum.clip[0xF] + frustum.clip[0xE];
	Normalize(&frustum, 5);
	return frustum;
}

bool FrustumContainsBox(Frustum frustum, float3 v0, float3 v1) {
	for (int i = 0; i < 6; i++) {
		bool b = true;
		b = b && frustum.planes[i][0] * v0.x + frustum.planes[i][1] * v0.y + frustum.planes[i][2] * v0.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v1.x + frustum.planes[i][1] * v0.y + frustum.planes[i][2] * v0.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v0.x + frustum.planes[i][1] * v1.y + frustum.planes[i][2] * v0.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v1.x + frustum.planes[i][1] * v1.y + frustum.planes[i][2] * v0.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v0.x + frustum.planes[i][1] * v0.y + frustum.planes[i][2] * v1.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v1.x + frustum.planes[i][1] * v0.y + frustum.planes[i][2] * v1.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v0.x + frustum.planes[i][1] * v1.y + frustum.planes[i][2] * v1.z + frustum.planes[i][3] <= 0.0;
		b = b && frustum.planes[i][0] * v1.x + frustum.planes[i][1] * v1.y + frustum.planes[i][2] * v1.z + frustum.planes[i][3] <= 0.0;
		if (b) { return false; }
	}
	return true;
}
