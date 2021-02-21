#include "SheepModel.h"

SheepModel SheepModelCreate()
{
	Model model = AnimalModelCreate(12, 0.0);
	model->Type = ModelTypeSheep;
	AnimalModelData this = model->TypeData;
	this->Head->UV = (int2){ 0, 0 };
	ModelPartSetBounds(this->Head, (float3){ -3.0, -4.0, -4.0 }, (int3){ 6, 6, 8 }, 0.0);
	ModelPartSetPosition(this->Head, (float3){ 0.0, 6.0, -8.0 });
	this->Body->UV = (int2){ 28, 0 };
	ModelPartSetBounds(this->Body, (float3){ -4.0, -10.0, -7.0 }, (int3){ 8, 16, 6 }, 0.0);
	ModelPartSetPosition(this->Body, (float3){ 0.0, 5.0, 2.0 });
	return model;
}
