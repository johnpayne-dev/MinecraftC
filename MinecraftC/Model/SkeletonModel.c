#include "SkeletonModel.h"
#include "ZombieModel.h"

SkeletonModel SkeletonModelCreate()
{
	HumanoidModel model = HumanoidModelCreate(0.0);
	model->Type = ModelTypeSkeleton;
	HumanoidModelData this = model->TypeData;
	this->RightArm->UV = (int2){ 40, 16 };
	ModelPartSetBounds(this->RightArm, (float3){ -1.0, -2.0, -1.0 }, (int3){ 2, 12, 2 }, 0.0);
	ModelPartSetPosition(this->RightArm, (float3){ -5.0, 2.0, 0.0 });
	this->LeftArm->UV = (int2){ 40, 16 };
	this->LeftArm->Mirror = true;
	ModelPartSetBounds(this->LeftArm, (float3){ -1.0, -2.0, -1.0 }, (int3){ 2, 12, 2 }, 0.0);
	ModelPartSetPosition(this->LeftArm, (float3){ 5.0, 2.0, 0.0 });
	this->RightLeg->UV = (int2){ 0, 16 };
	ModelPartSetBounds(this->RightLeg, (float3){ -1.0, 0.0, -1.0 }, (int3){ 2, 12, 2 }, 0.0);
	ModelPartSetPosition(this->RightLeg, (float3){ -2.0, 12.0, 0.0 });
	this->LeftLeg->UV = (int2){ 0, 16 };
	this->LeftLeg->Mirror = true;
	ModelPartSetBounds(this->LeftLeg, (float3){ -1.0, 0.0, -1.0 }, (int3){ 2, 12, 2 }, 0.0);
	ModelPartSetPosition(this->LeftLeg, (float3){ 2.0, 12.0, 0.0 });
	return model;
}

void SkeletonModelSetRotationAngles(SkeletonModel model, float anim, float t, float run, float2 rot, float offset)
{
	ZombieModelSetRotationAngles(model, anim, t, run, rot, offset);
}
