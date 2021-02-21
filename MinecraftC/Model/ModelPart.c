#include <OpenGL.h>
#include "ModelPart.h"
#include "../Utilities/Memory.h"

ModelPart ModelPartCreate(int2 uv)
{
	ModelPart part = MemoryAllocate(sizeof(struct ModelPart));
	*part = (struct ModelPart)
	{
		.UV = uv,
		.HasList = false,
		.List = 0,
		.Mirror = false,
		.Render = true,
	};
	return part;
}

void ModelPartSetBounds(ModelPart part, float3 v1, int3 v2, float v3)
{
	float3 a = v1 + float3i(v2);
	v1 -= v3;
	a += v3;
	if (part->Mirror)
	{
		int _ = a.x;
		a.x = v1.x;
		v1.x = _;
	}
	part->Vertices[0] = (Vertex){ .Position = { v1.x, v1.y, v1.z }, .UV = { 0.0, 0.0 } };
	part->Vertices[1] = (Vertex){ .Position = { a.x, v1.y, v1.z }, .UV = { 0.0, 8.0 } };
	part->Vertices[2] = (Vertex){ .Position = { a.x, a.y, v1.z }, .UV = { 8.0, 8.0 } };
	part->Vertices[3] = (Vertex){ .Position = { v1.x, a.y, v1.z }, .UV = { 8.0, 0.0 } };
	part->Vertices[4] = (Vertex){ .Position = { v1.x, v1.y, a.z }, .UV = { 0.0, 0.0 } };
	part->Vertices[5] = (Vertex){ .Position = { a.x, v1.y, a.z }, .UV = { 0.0, 8.0 } };
	part->Vertices[6] = (Vertex){ .Position = { a.x, a.y, a.z }, .UV = { 8.0, 8.0 } };
	part->Vertices[7] = (Vertex){ .Position = { v1.x, a.y, a.z }, .UV = { 8.0, 0.0 } };
	part->Quads[0] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[5], part->Vertices[1], part->Vertices[2], part->Vertices[6] }, float2i(part->UV) + (float2){ v2.x, 0.0 } + v3, float2i(part->UV) + (float2){ v2.x + v3, v2.y } + v3);
	part->Quads[1] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[0], part->Vertices[4], part->Vertices[7], part->Vertices[3] }, float2i(part->UV) + (float2){ 0.0, v3 }, float2i(part->UV) + (float2){ 0.0, v2.y } + v3);
	part->Quads[2] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[5], part->Vertices[4], part->Vertices[0], part->Vertices[1] }, float2i(part->UV) + (float2){ v3, 0.0 }, float2i(part->UV) + (float2){ v2.x, 0.0 } + v3);
	part->Quads[3] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[2], part->Vertices[3], part->Vertices[7], part->Vertices[6] }, float2i(part->UV) + (float2){ v2.x + v3, 0.0 }, float2i(part->UV) + (float2){ v2.x + v2.x, 0.0 } + v3);
	part->Quads[4] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[1], part->Vertices[0], part->Vertices[3], part->Vertices[2] }, float2i(part->UV) + (float2){ 0.0, 0.0 } + v3, float2i(part->UV) + (float2){ v2.x, v2.y } + v3);
	part->Quads[5] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[4], part->Vertices[5], part->Vertices[6], part->Vertices[7] }, float2i(part->UV) + (float2){ v2.x + v3, 0.0 } + v3, float2i(part->UV) + (float2){ v2.x + v2.x + v3, v2.y } + v3);
	if (part->Mirror)
	{
		for (int i = 0; i < 6; i++)
		{
			Vertex vertices[4];
			for (int j = 0; j < 4; j++) { vertices[j] = part->Quads[i].Vertices[4 - j - 1]; }
			for (int j = 0; j < 4; j++) { part->Quads[i].Vertices[j] = vertices[j]; }
		}
	}
}

void ModelPartSetPosition(ModelPart part, float3 pos)
{
	part->Position = pos;
}

void ModelPartRender(ModelPart part, float scale)
{
	if (!part->Render) { return; }
	
	if (!part->HasList) { ModelPartGenerateList(part, scale); }
	
	if (part->Rotation.x == 0.0 && part->Rotation.y == 0.0 && part->Rotation.z == 0.0)
	{
		if (part->Position.x == 0.0 && part->Position.y == 0.0 && part->Position.z == 0.0) { glCallList(part->List); }
		else
		{
			glTranslatef(part->Position.x * scale, part->Position.y * scale, part->Position.z * scale);
			glCallList(part->List);
			glTranslatef(-part->Position.x * scale, -part->Position.y * scale, -part->Position.z * scale);
		}
	}
	else
	{
		glPushMatrix();
		glTranslatef(part->Position.x * scale, part->Position.y * scale, part->Position.z * scale);
		if (part->Rotation.z != 0.0) { glRotatef(part->Rotation.z * deg, 0.0, 0.0, 1.0); }
		if (part->Rotation.y != 0.0) { glRotatef(part->Rotation.y * deg, 0.0, 1.0, 0.0); }
		if (part->Rotation.x != 0.0) { glRotatef(part->Rotation.x * deg, 1.0, 0.0, 0.0); }
		glCallList(part->List);
		glPopMatrix();
	}
}

void ModelPartGenerateList(ModelPart part, float scale)
{
	part->List = glGenLists(1);
	glNewList(part->List, GL_COMPILE);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		TexturedQuad quad = part->Quads[i];
		float3 c = cross3f(normalize3f(quad.Vertices[1].Position - quad.Vertices[0].Position), normalize3f(quad.Vertices[1].Position - quad.Vertices[2].Position));
		glNormal3f(c.x, c.y, c.z);
		
		for (int j = 0; j < 4; j++)
		{
			glTexCoord2f(quad.Vertices[j].UV.x, quad.Vertices[j].UV.y);
			glVertex3f(quad.Vertices[j].Position.x * scale, quad.Vertices[j].Position.y * scale, quad.Vertices[j].Position.z * scale);
		}
	}
	glEnd();
	glEndList();
	part->HasList = true;
}

void ModelPartDestroy(ModelPart part)
{
	MemoryFree(part);
}
