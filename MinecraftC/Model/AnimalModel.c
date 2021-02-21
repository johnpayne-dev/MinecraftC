#include "AnimalModel.h"
#include "../Utilities/Memory.h"

AnimalModel AnimalModelCreate(int v1, float v2)
{
	Model model = ModelCreate();
	AnimalModelData animal = MemoryAllocate(sizeof(struct AnimalModelData));
	model->Type = ModelTypeAnimal;
	model->TypeData = animal;
	
	animal->Head = ModelPartCreate((int2){ 0, 0 });
	ModelPartSetBounds(animal->Head, (float3){ -4.0, -4.0, -8.0 }, (int3){ 8, 8, 8 }, 0.0);
	ModelPartSetPosition(animal->Head, (float3){ 0.0, 18 - v1, -6.0 });
	animal->Body = ModelPartCreate((int2){ 28, 8 });
	ModelPartSetBounds(animal->Body, (float3){ -5.0, -10.0, -7.0 }, (int3){ 10, 16, 8 }, 0.0);
	ModelPartSetPosition(animal->Body, (float3){ 0.0, 17 - v1, 2.0 });
	animal->Legs[0] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(animal->Legs[0], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, v1, 4 }, 0.0);
	ModelPartSetPosition(animal->Legs[0], (float3){ -3.0, 24 - v1, 7.0 });
	animal->Legs[1] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(animal->Legs[1], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, v1, 4 }, 0.0);
	ModelPartSetPosition(animal->Legs[1], (float3){ 3.0, 24 - v1, 7.0 });
	animal->Legs[2] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(animal->Legs[2], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, v1, 4 }, 0.0);
	ModelPartSetPosition(animal->Legs[2], (float3){ -3.0, 24 - v1, -5.0 });
	animal->Legs[3] = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(animal->Legs[3], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, v1, 4 }, 0.0);
	ModelPartSetPosition(animal->Legs[3], (float3){ 3.0, 24 - v1, -5.0 });
	return model;
}

void AnimalModelRender(AnimalModel model, float anim, float t, float run, float2 rot, float offset)
{
	AnimalModelData this = model->TypeData;
	this->Head->Rotation.yx = rot * rad;
	this->Body->Rotation.x = pi / 2.0;
	this->Legs[0]->Rotation.x = cos(anim * 0.6662) * 1.4 * t;
	this->Legs[1]->Rotation.x = cos(anim * 0.6662 + pi) * 1.4 * t;
	this->Legs[2]->Rotation.x = cos(anim * 0.6662 + pi) * 1.4 * t;
	this->Legs[3]->Rotation.x = cos(anim * 0.6662) * 1.4 * t;
	ModelPartRender(this->Head, offset);
	ModelPartRender(this->Body, offset);
	for (int i = 0; i < 4; i++) { ModelPartRender(this->Legs[i], offset); }
}

void AnimalModelDestroy(AnimalModel model)
{
	AnimalModelData this = model->TypeData;
	ModelPartDestroy(this->Body);
	ModelPartDestroy(this->Head);
	for (int i = 0; i < 4; i++) { ModelPartDestroy(this->Legs[i]); }
	MemoryFree(this);
}
