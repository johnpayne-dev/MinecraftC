#pragma once
#include "Model.h"
#include "ModelPart.h"

typedef Model HumanoidModel;

typedef struct HumanoidModelData
{
	ModelPart Head;
	ModelPart HeadWear;
	ModelPart Body;
	ModelPart LeftArm;
	ModelPart RightArm;
	ModelPart LeftLeg;
	ModelPart RightLeg;
} * HumanoidModelData;

HumanoidModel HumanoidModelCreate(float offset);
void HumanoidModelRender(HumanoidModel model, float anim, float t, float run, float2 rot, float offset);
void HumanoidModelSetRotationAngles(HumanoidModel model, float anim, float t, float run, float2 rot, float offset);
void HumanoidModelDestroy(HumanoidModel model);
