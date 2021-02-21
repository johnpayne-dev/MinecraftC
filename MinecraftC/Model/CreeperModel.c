#include "CreeperModel.h"
#include "../Utilities/Memory.h"

CreeperModel CreeperModelCreate(void)
{
	Model model = ModelCreate();
	CreeperModelData creeper = MemoryAllocate(sizeof(struct CreeperModelData));
	model->Type = ModelTypeCreeper;
	model->TypeData = creeper;
	creeper->Head = ModelPartCreate((int2){ 0, 0 });
	ModelPartSetBounds(creeper->Head, (float3){ -4.0, -8.0, -4.0 }, (int3){ 8, 8, 8 }, 0.0);
	creeper->Unused = ModelPartCreate((int2){ 32, 0 });
	ModelPartSetBounds(creeper->Unused, (float3){ -4.0, -8.0, -4.0 }, (int3){ 8, 8, 8 }, 0.5);
	creeper->Body = ModelPartCreate((int2){ 16, 16 });
	ModelPartSetBounds(creeper->Body, (float3){ -4.0, 0.0, -2.0 }, (int3){ 8, 12, 4 }, 0.0);
	creeper->Legs[0] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(creeper->Legs[0], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.0);
	ModelPartSetPosition(creeper->Legs[0], (float3){ -2.0, 12.0, 4.0 });
	creeper->Legs[1] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(creeper->Legs[1], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.0);
	ModelPartSetPosition(creeper->Legs[1], (float3){ 2.0, 12.0, 4.0 });
	creeper->Legs[2] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(creeper->Legs[2], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.0);
	ModelPartSetPosition(creeper->Legs[2], (float3){ -2.0, 12.0, -4.0 });
	creeper->Legs[3] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(creeper->Legs[3], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.0);
	ModelPartSetPosition(creeper->Legs[3], (float3){ 2.0, 12.0, -4.0 });
	return model;
}

void CreeperModelRender(CreeperModel model, float anim, float t, float run, float2 rot, float offset)
{
	CreeperModelData this = model->TypeData;
	this->Head->Rotation.yx = rot * rad;
	this->Legs[0]->Rotation.x = cos(anim * 0.6662) * 1.4 * t;
	this->Legs[1]->Rotation.x = cos(anim * 0.6662 + pi) * 1.4 * t;
	this->Legs[2]->Rotation.x = cos(anim * 0.6662 + pi) * 1.4 * t;
	this->Legs[3]->Rotation.x = cos(anim * 0.6662) * 1.4 * t;
	ModelPartRender(this->Head, offset);
	ModelPartRender(this->Body, offset);
	for (int i = 0; i < 4; i++) { ModelPartRender(this->Legs[i], offset); }
}

void CreeperModelDestroy(CreeperModel model)
{
	CreeperModelData this = model->TypeData;
	ModelPartDestroy(this->Body);
	ModelPartDestroy(this->Unused);
	ModelPartDestroy(this->Head);
	for (int i = 0; i < 4; i++) { ModelPartDestroy(this->Legs[i]); }
	MemoryFree(this);
}
