#include <stdlib.h>
#include "ItemModel.h"

ItemModel ItemModelCreate(int texID)
{
	ItemModel model = malloc(sizeof(struct ItemModel));
	*model = (struct ItemModel){ .Part = ModelPartCreate((int2){ 0, 0 }), };
	ModelPart part = model->Part;
	part->Vertices[0] = (Vertex){ .Position = { -2.0, -2.0, -2.0 }, .UV = { 0.0, 0.0 } };
	part->Vertices[1] = (Vertex){ .Position = { 2.0, -2.0, -2.0 }, .UV = { 0.0, 8.0 } };
	part->Vertices[2] = (Vertex){ .Position = { 2.0, 2.0, -2.0 }, .UV = { 8.0, 8.0 } };
	part->Vertices[3] = (Vertex){ .Position = { -2.0, 2.0, -2.0 }, .UV = { 8.0, 0.0 } };
	part->Vertices[4] = (Vertex){ .Position = { -2.0, -2.0, 2.0 }, .UV = { 0.0, 0.0 } };
	part->Vertices[5] = (Vertex){ .Position = { 2.0, -2.0, 2.0 }, .UV = { 0.0, 8.0 } };
	part->Vertices[6] = (Vertex){ .Position = { 2.0, 2.0, 2.0 }, .UV = { 8.0, 8.0 } };
	part->Vertices[7] = (Vertex){ .Position = { -2.0, 2.0, 2.0 }, .UV = { 8.0, 0.0 } };
	float2 uv0 = ((float2){ texID % 16, texID / 16 } + (1.0 - 0.25)) / 16.0;
	float2 uv1 = ((float2){ texID % 16, texID / 16 } + 0.25) / 16.0;
	part->Quads[0] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[5], part->Vertices[1], part->Vertices[2], part->Vertices[6] }, uv0, uv1);
	part->Quads[1] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[0], part->Vertices[4], part->Vertices[7], part->Vertices[3] }, uv0, uv1);
	part->Quads[2] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[5], part->Vertices[4], part->Vertices[0], part->Vertices[1] }, uv0, uv1);
	part->Quads[3] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[2], part->Vertices[3], part->Vertices[7], part->Vertices[6] }, uv0, uv1);
	part->Quads[4] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[1], part->Vertices[0], part->Vertices[3], part->Vertices[2] }, uv0, uv1);
	part->Quads[5] = TexturedQuadCreateUV((Vertex[]){ part->Vertices[4], part->Vertices[5], part->Vertices[6], part->Vertices[7] }, uv0, uv1);
	return model;
}

void ItemModelGenerateList(ItemModel model)
{
	ModelPartRender(model->Part, 0.0625);
}

void ItemModelDestroy(ItemModel model)
{
	ModelPartDestroy(model->Part);
	free(model);
}
