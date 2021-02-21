#pragma once
#include "HumanoidModel.h"

typedef HumanoidModel ZombieModel;

ZombieModel ZombieModelCreate(void);
void ZombieModelSetRotationAngles(ZombieModel model, float anim, float t, float run, float2 rot, float offset);
