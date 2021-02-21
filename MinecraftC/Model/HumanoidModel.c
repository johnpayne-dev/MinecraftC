#include "HumanoidModel.h"
#include "../Utilities/Memory.h"

HumanoidModel HumanoidModelCreate(float offset)
{
	Model model = ModelCreate();
	HumanoidModelData humanoid = MemoryAllocate(sizeof(struct HumanoidModelData));
	model->Type = ModelTypeHumanoid;
	model->TypeData = humanoid;
	humanoid->Head = ModelPartCreate((int2){ 0, 0 });
	ModelPartSetBounds(humanoid->Head, (float3){ -4.0, -8.0, -4.0 }, (int3){ 8, 8, 8 }, offset);
	humanoid->HeadWear = ModelPartCreate((int2){ 32, 0 });
	ModelPartSetBounds(humanoid->HeadWear, (float3){ -4.0, -8.0, -4.0 }, (int3){ 8, 8, 8 }, offset + 0.5);
	humanoid->Body = ModelPartCreate((int2){ 16, 16 });
	ModelPartSetBounds(humanoid->Body, (float3){ -4.0, 0.0, -2.0 }, (int3){ 8, 12, 4 }, offset);
	humanoid->RightArm = ModelPartCreate((int2){ 40, 16 });
	ModelPartSetBounds(humanoid->RightArm, (float3){ -3.0, -2.0, -2.0 }, (int3){ 4, 12, 4 }, offset);
	ModelPartSetPosition(humanoid->RightArm, (float3){ -5.0, 2.0, 0.0 });
	humanoid->LeftArm = ModelPartCreate((int2){ 40, 16 });
	humanoid->LeftArm->Mirror = true;
	ModelPartSetBounds(humanoid->LeftArm, (float3){ -1.0, -2.0, -2.0 }, (int3){ 4, 12, 4 }, offset);
	ModelPartSetPosition(humanoid->LeftArm, (float3){ 5.0, 2.0, 0.0 });
	humanoid->RightLeg = ModelPartCreate((int2){ 0, 16 });
	ModelPartSetBounds(humanoid->RightLeg, (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 12, 4 }, offset);
	ModelPartSetPosition(humanoid->RightLeg, (float3){ -2.0, 12.0, 0.0 });
	humanoid->LeftLeg = ModelPartCreate((int2){ 0, 16 });
	humanoid->LeftLeg->Mirror = true;
	ModelPartSetBounds(humanoid->LeftLeg, (float3){ -2.0, 0.0, -2.0 }, (int3){ 4, 12, 4 }, offset);
	ModelPartSetPosition(humanoid->LeftLeg, (float3){ 2.0, 12.0, 0.0 });
	return model;
}

void HumanoidModelRender(HumanoidModel model, float anim, float t, float run, float2 rot, float offset)
{
	HumanoidModelData this = model->TypeData;
	HumanoidModelSetRotationAngles(model, anim, t, run, rot, offset);
	ModelPartRender(this->Head, offset);
	ModelPartRender(this->Body, offset);
	ModelPartRender(this->RightArm, offset);
	ModelPartRender(this->RightLeg, offset);
	ModelPartRender(this->LeftArm, offset);
	ModelPartRender(this->LeftLeg, offset);
}

void HumanoidModelSetRotationAngles(HumanoidModel model, float anim, float t, float run, float2 rot, float offset)
{
	HumanoidModelData this = model->TypeData;
	this->Head->Rotation.yx = rot * rad;
	this->RightArm->Rotation.xz = (float2){ cos(anim * 0.6662 + pi) * 2.0, cos(anim * 0.2312) + 1.0 } * t;
	this->LeftArm->Rotation.xz = (float2){ cos(anim * 0.6662) * 2.0, cos(anim * 0.2812) - 1.0 } * t;
	this->RightLeg->Rotation.x = cos(anim * 0.6662) * 1.4 * t;
	this->LeftLeg->Rotation.x = cos(anim * 0.6662 + pi) * 1.4 * t;
	this->RightArm->Rotation.xz += (float2){ sin(run * 0.067) * 0.05, cos(run * 0.09) * 0.05 + 0.05 };
	this->LeftArm->Rotation.xz -= (float2){ sin(run * 0.067) * 0.05, cos(run * 0.09) * 0.05 + 0.05 };
}

void HumanoidModelDestroy(HumanoidModel model)
{
	HumanoidModelData this = model->TypeData;
	ModelPartDestroy(this->Head);
	ModelPartDestroy(this->HeadWear);
	ModelPartDestroy(this->Body);
	ModelPartDestroy(this->RightArm);
	ModelPartDestroy(this->RightLeg);
	ModelPartDestroy(this->LeftArm);
	ModelPartDestroy(this->LeftLeg);
	MemoryFree(this);
}
