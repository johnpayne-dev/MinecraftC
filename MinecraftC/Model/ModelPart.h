#pragma once
#include "TexturedQuad.h"

typedef struct ModelPart
{
	Vertex Vertices[8];
	TexturedQuad Quads[6];
	int2 UV;
	float3 Position;
	float3 Rotation;
	bool HasList;
	int List;
	bool Mirror;
	bool Render;
} * ModelPart;

ModelPart ModelPartCreate(int2 uv);
void ModelPartSetBounds(ModelPart part, float3 var1, int3 var2, float var3);
void ModelPartSetPosition(ModelPart part, float3 pos);
void ModelPartRender(ModelPart part, float scale);
void ModelPartGenerateList(ModelPart part, float scale);
void ModelPartDestroy(ModelPart part);
