#include <OpenGL.h>
#include "Frustum.h"

static void Normalize(Frustum * frustum, int plane)
{
	float len = length3f((float3){ frustum->Planes[plane][0], frustum->Planes[plane][1], frustum->Planes[plane][2] });
	for (int i = 0; i < 4; i++) { frustum->Planes[plane][i] /= len; }
}

Frustum FrustumUpdate()
{
	Frustum frustum = { 0 };
	glGetFloatv(GL_PROJECTION_MATRIX, frustum.Projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, frustum.ModelView);
	frustum.Clip[0x0] = frustum.ModelView[0x0] * frustum.Projection[0x0] + frustum.ModelView[0x1] * frustum.Projection[0x4] + frustum.ModelView[0x2] * frustum.Projection[0x8] + frustum.ModelView[0x3] * frustum.Projection[0xC];
	frustum.Clip[0x1] = frustum.ModelView[0x0] * frustum.Projection[0x1] + frustum.ModelView[0x1] * frustum.Projection[0x5] + frustum.ModelView[0x2] * frustum.Projection[0x9] + frustum.ModelView[0x3] * frustum.Projection[0xD];
	frustum.Clip[0x2] = frustum.ModelView[0x0] * frustum.Projection[0x2] + frustum.ModelView[0x1] * frustum.Projection[0x6] + frustum.ModelView[0x2] * frustum.Projection[0xA] + frustum.ModelView[0x3] * frustum.Projection[0xE];
	frustum.Clip[0x3] = frustum.ModelView[0x0] * frustum.Projection[0x3] + frustum.ModelView[0x1] * frustum.Projection[0x7] + frustum.ModelView[0x2] * frustum.Projection[0xB] + frustum.ModelView[0x3] * frustum.Projection[0xF];
	frustum.Clip[0x4] = frustum.ModelView[0x4] * frustum.Projection[0x0] + frustum.ModelView[0x5] * frustum.Projection[0x4] + frustum.ModelView[0x6] * frustum.Projection[0x8] + frustum.ModelView[0x7] * frustum.Projection[0xC];
	frustum.Clip[0x5] = frustum.ModelView[0x4] * frustum.Projection[0x1] + frustum.ModelView[0x5] * frustum.Projection[0x5] + frustum.ModelView[0x6] * frustum.Projection[0x9] + frustum.ModelView[0x7] * frustum.Projection[0xD];
	frustum.Clip[0x6] = frustum.ModelView[0x4] * frustum.Projection[0x2] + frustum.ModelView[0x5] * frustum.Projection[0x6] + frustum.ModelView[0x6] * frustum.Projection[0xA] + frustum.ModelView[0x7] * frustum.Projection[0xE];
	frustum.Clip[0x7] = frustum.ModelView[0x4] * frustum.Projection[0x3] + frustum.ModelView[0x5] * frustum.Projection[0x7] + frustum.ModelView[0x6] * frustum.Projection[0xB] + frustum.ModelView[0x7] * frustum.Projection[0xF];
	frustum.Clip[0x8] = frustum.ModelView[0x8] * frustum.Projection[0x0] + frustum.ModelView[0x9] * frustum.Projection[0x4] + frustum.ModelView[0xA] * frustum.Projection[0x8] + frustum.ModelView[0xB] * frustum.Projection[0xC];
	frustum.Clip[0x9] = frustum.ModelView[0x8] * frustum.Projection[0x1] + frustum.ModelView[0x9] * frustum.Projection[0x5] + frustum.ModelView[0xA] * frustum.Projection[0x9] + frustum.ModelView[0xB] * frustum.Projection[0xD];
	frustum.Clip[0xA] = frustum.ModelView[0x8] * frustum.Projection[0x2] + frustum.ModelView[0x9] * frustum.Projection[0x6] + frustum.ModelView[0xA] * frustum.Projection[0xA] + frustum.ModelView[0xB] * frustum.Projection[0xE];
	frustum.Clip[0xB] = frustum.ModelView[0x8] * frustum.Projection[0x3] + frustum.ModelView[0x9] * frustum.Projection[0x7] + frustum.ModelView[0xA] * frustum.Projection[0xB] + frustum.ModelView[0xB] * frustum.Projection[0xF];
	frustum.Clip[0xC] = frustum.ModelView[0xC] * frustum.Projection[0x0] + frustum.ModelView[0xD] * frustum.Projection[0x4] + frustum.ModelView[0xE] * frustum.Projection[0x8] + frustum.ModelView[0xF] * frustum.Projection[0xC];
	frustum.Clip[0xD] = frustum.ModelView[0xC] * frustum.Projection[0x1] + frustum.ModelView[0xD] * frustum.Projection[0x5] + frustum.ModelView[0xE] * frustum.Projection[0x9] + frustum.ModelView[0xF] * frustum.Projection[0xD];
	frustum.Clip[0xE] = frustum.ModelView[0xC] * frustum.Projection[0x2] + frustum.ModelView[0xD] * frustum.Projection[0x6] + frustum.ModelView[0xE] * frustum.Projection[0xA] + frustum.ModelView[0xF] * frustum.Projection[0xE];
	frustum.Clip[0xF] = frustum.ModelView[0xC] * frustum.Projection[0x3] + frustum.ModelView[0xD] * frustum.Projection[0x7] + frustum.ModelView[0xE] * frustum.Projection[0xB] + frustum.ModelView[0xF] * frustum.Projection[0xF];
	frustum.Planes[0][0] = frustum.Clip[0x3] - frustum.Clip[0x0];
	frustum.Planes[0][1] = frustum.Clip[0x7] - frustum.Clip[0x4];
	frustum.Planes[0][2] = frustum.Clip[0xB] - frustum.Clip[0x8];
	frustum.Planes[0][3] = frustum.Clip[0xF] - frustum.Clip[0xC];
	Normalize(&frustum, 0);
	frustum.Planes[1][0] = frustum.Clip[0x3] + frustum.Clip[0x0];
	frustum.Planes[1][1] = frustum.Clip[0x7] + frustum.Clip[0x4];
	frustum.Planes[1][2] = frustum.Clip[0xB] + frustum.Clip[0x8];
	frustum.Planes[1][3] = frustum.Clip[0xF] + frustum.Clip[0xC];
	Normalize(&frustum, 1);
	frustum.Planes[2][0] = frustum.Clip[0x3] - frustum.Clip[0x1];
	frustum.Planes[2][1] = frustum.Clip[0x7] - frustum.Clip[0x5];
	frustum.Planes[2][2] = frustum.Clip[0xB] - frustum.Clip[0x9];
	frustum.Planes[2][3] = frustum.Clip[0xF] - frustum.Clip[0xD];
	Normalize(&frustum, 2);
	frustum.Planes[3][0] = frustum.Clip[0x3] + frustum.Clip[0x1];
	frustum.Planes[3][1] = frustum.Clip[0x7] + frustum.Clip[0x5];
	frustum.Planes[3][2] = frustum.Clip[0xB] + frustum.Clip[0x9];
	frustum.Planes[3][3] = frustum.Clip[0xF] + frustum.Clip[0xD];
	Normalize(&frustum, 3);
	frustum.Planes[4][0] = frustum.Clip[0x3] - frustum.Clip[0x2];
	frustum.Planes[4][1] = frustum.Clip[0x7] - frustum.Clip[0x6];
	frustum.Planes[4][2] = frustum.Clip[0xB] - frustum.Clip[0xA];
	frustum.Planes[4][3] = frustum.Clip[0xF] - frustum.Clip[0xE];
	Normalize(&frustum, 4);
	frustum.Planes[5][0] = frustum.Clip[0x3] + frustum.Clip[0x2];
	frustum.Planes[5][1] = frustum.Clip[0x7] + frustum.Clip[0x6];
	frustum.Planes[5][2] = frustum.Clip[0xB] + frustum.Clip[0xA];
	frustum.Planes[5][3] = frustum.Clip[0xF] + frustum.Clip[0xE];
	Normalize(&frustum, 5);
	return frustum;
}

bool FrustumContainsBox(Frustum frustum, float3 v0, float3 v1)
{
	for (int i = 0; i < 7; i++)
	{
		bool b = true;
		b = b && frustum.Planes[i][0] * v0.x + frustum.Planes[i][1] * v0.y + frustum.Planes[i][2] * v0.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v1.x + frustum.Planes[i][1] * v0.y + frustum.Planes[i][2] * v0.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v0.x + frustum.Planes[i][1] * v1.y + frustum.Planes[i][2] * v0.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v1.x + frustum.Planes[i][1] * v1.y + frustum.Planes[i][2] * v0.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v0.x + frustum.Planes[i][1] * v0.y + frustum.Planes[i][2] * v1.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v1.x + frustum.Planes[i][1] * v0.y + frustum.Planes[i][2] * v1.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v0.x + frustum.Planes[i][1] * v1.y + frustum.Planes[i][2] * v1.z + frustum.Planes[i][3] <= 0.0;
		b = b && frustum.Planes[i][0] * v1.x + frustum.Planes[i][1] * v1.y + frustum.Planes[i][2] * v1.z + frustum.Planes[i][3] <= 0.0;
		if (b) { return false; }
	}
	return true;
}
