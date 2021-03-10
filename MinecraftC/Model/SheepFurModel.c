#include "SheepFurModel.h"

SheepFurModel SheepFurModelCreate()
{
	Model model = AnimalModelCreate(12, 0.0);
	model->Type = ModelTypeSheepFur;
	AnimalModelData this = model->TypeData;
	this->Head->UV = (int2){ 0, 0 };
	ModelPartSetBounds(this->Head, (float3){ -3.0, -4.0, -4.0 }, (int3){ 6, 6, 6 }, 0.6);
	ModelPartSetPosition(this->Head, (float3){ 0.0, 6.0, -8.0 });
	this->Body->UV = (int2){ 28, 8 };
	ModelPartSetBounds(this->Body, (float3){ -4.0, -10.0, -7.0 }, (int3){ 8, 16, 6 }, 1.75);
	ModelPartSetPosition(this->Body, (float3){ 0.0, 5.0, 2.0 });
	this->Legs[0]->UV = (int2){ 0, 16 };
	ModelPartSetBounds(this->Legs[0], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.5);
	ModelPartSetPosition(this->Legs[0], (float3){ -3.0, 12.0, 7.0 });
	this->Legs[1]->UV = (int2){ 0, 16 };
	ModelPartSetBounds(this->Legs[1], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.5);
	ModelPartSetPosition(this->Legs[1], (float3){ 3.0, 12.0, 7.0 });
	this->Legs[2]->UV = (int2){ 0, 16 };
	ModelPartSetBounds(this->Legs[2], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.5);
	ModelPartSetPosition(this->Legs[2], (float3){ -3.0, 12.0, -5.0 });
	this->Legs[3]->UV = (int2){ 0, 16 };
	ModelPartSetBounds(this->Legs[3], (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 6, 4 }, 0.5);
	ModelPartSetPosition(this->Legs[3], (float3){ 3.0, 12.0, -5.0 });
	return model;
}
