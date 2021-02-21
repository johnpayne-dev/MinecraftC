#pragma once
#include "HumanoidModel.h"

typedef HumanoidModel SkeletonModel;

SkeletonModel SkeletonModelCreate(void);
void SkeletonModelSetRotationAngles(SkeletonModel model, float anim, float t, float run, float2 rot, float offset);
