#pragma once
#include "Model.h"
#include "HumanoidModel.h"
#include "CreeperModel.h"
#include "SkeletonModel.h"
#include "ZombieModel.h"
#include "PigModel.h"
#include "SheepModel.h"
#include "SpiderModel.h"
#include "SheepFurModel.h"

typedef struct ModelManager
{
	HumanoidModel Human;
	HumanoidModel ArmoredHuman;
	CreeperModel Creeper;
	SkeletonModel Skeleton;
	ZombieModel Zombie;
	PigModel Pig;
	SheepModel Sheep;
	SheepFurModel SheepFur;
	SpiderModel Spider;
} * ModelManager;

ModelManager ModelManagerCreate(void);
Model ModelManagerGetModel(ModelManager models, const char * modelName);
void ModelManagerDestroy(ModelManager models);
