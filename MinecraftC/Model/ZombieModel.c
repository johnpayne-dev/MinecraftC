#include "ZombieModel.h"

ZombieModel ZombieModelCreate()
{
	HumanoidModel model = HumanoidModelCreate(0.0);
	model->Type = ModelTypeZombie;
	return model;
}

void ZombieModelSetRotationAngles(ZombieModel model, float anim, float t, float run, float2 rot, float offset)
{
	HumanoidModelData this = model->TypeData;
	HumanoidModelSetRotationAngles(model, anim, t, run, rot, offset);
	float a = sin(model->AttackOffset * pi);
	float b = sin((1.0 - (1.0 - model->AttackOffset) * (1.0 - model->AttackOffset)) * pi);
	this->RightArm->Rotation = (float3){ -pi / 2.0, -(0.1 - a * 0.6), 0.0 };
	this->LeftArm->Rotation = (float3){ -pi / 2.0, 0.1 - a * 0.6, 0.0 };
	this->RightArm->Rotation += (float3){ sin(run * 0.067) * 0.05 + b * 0.4 - a * 1.2, 0.0, cos(run * 0.09) * 0.05 + 0.05 };
	this->LeftArm->Rotation -= (float3){ a * 1.2 - b * 0.4 + sin(run * 0.067) * 0.05, 0.0, cos(run * 0.09) * 0.05 + 0.05 };
}
