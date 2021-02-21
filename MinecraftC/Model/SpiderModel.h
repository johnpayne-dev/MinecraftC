#pragma once
#include "Model.h"
#include "ModelPart.h"

typedef Model SpiderModel;

typedef struct SpiderModelData
{
	ModelPart Head;
	ModelPart Neck;
	ModelPart Body;
	ModelPart Legs[8];
} * SpiderModelData;

SpiderModel SpiderModelCreate(void);
void SpiderModelRender(SpiderModel model, float anim, float t, float run, float2 rot, float offset);
void SpiderModelDestroy(SpiderModel model);
