#pragma once
#include "Model.h"
#include "ModelPart.h"

typedef Model AnimalModel;

typedef struct AnimalModelData
{
	ModelPart Head;
	ModelPart Body;
	ModelPart Legs[4];
} * AnimalModelData;

AnimalModel AnimalModelCreate(int var1, float var2);
void AnimalModelRender(AnimalModel model, float anim, float t, float run, float2 rot, float offset);
void AnimalModelDestroy(AnimalModel model);
