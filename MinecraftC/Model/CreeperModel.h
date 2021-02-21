#pragma once
#include "Model.h"
#include "ModelPart.h"

typedef Model CreeperModel;

typedef struct CreeperModelData
{
	ModelPart Head;
	ModelPart Unused;
	ModelPart Body;
	ModelPart Legs[4];
} * CreeperModelData;

CreeperModel CreeperModelCreate(void);
void CreeperModelRender(CreeperModel model, float anim, float t, float run, float2 rot, float offset);
void CreeperModelDestroy(CreeperModel model);
