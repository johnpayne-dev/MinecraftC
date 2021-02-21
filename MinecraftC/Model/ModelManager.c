#include <string.h>
#include "ModelManager.h"
#include "../Utilities/Memory.h"

ModelManager ModelManagerCreate()
{
	ModelManager models = MemoryAllocate(sizeof(struct ModelManager));
	*models = (struct ModelManager)
	{
		.Human = HumanoidModelCreate(0.0),
		.ArmoredHuman = HumanoidModelCreate(1.0),
		.Creeper = CreeperModelCreate(),
		.Skeleton = SkeletonModelCreate(),
		.Zombie = ZombieModelCreate(),
		.Pig = PigModelCreate(),
		.Sheep = SheepModelCreate(),
		.Spider = SpiderModelCreate(),
		.SheepFur = SheepFurModelCreate(),
	};
	return models;
}

Model ModelManagerGetModel(ModelManager models, const char * modelName)
{
	if (strcmp(modelName, "Humanoid") == 0) { return models->Human; }
	if (strcmp(modelName, "Humanoid.Armor") == 0) { return models->ArmoredHuman; }
	if (strcmp(modelName, "Creeper") == 0) { return models->Creeper; }
	if (strcmp(modelName, "Skeleton") == 0) { return models->Skeleton; }
	if (strcmp(modelName, "Zombie") == 0) { return models->Zombie; }
	if (strcmp(modelName, "Pig") == 0) { return models->Pig; }
	if (strcmp(modelName, "Sheep") == 0) { return models->Sheep; }
	if (strcmp(modelName, "Spider") == 0) { return models->Spider; }
	if (strcmp(modelName, "Sheep.Fur") == 0) { return models->SheepFur; }
	return NULL;
}

void ModelManagerDestroy(ModelManager models)
{
	ModelDestroy(models->Human);
	ModelDestroy(models->ArmoredHuman);
	ModelDestroy(models->Creeper);
	ModelDestroy(models->Skeleton);
	ModelDestroy(models->Zombie);
	ModelDestroy(models->Pig);
	ModelDestroy(models->Sheep);
	ModelDestroy(models->Spider);
	ModelDestroy(models->SheepFur);
	MemoryFree(models);
}
