#include "Model.h"
#include "AnimalModel.h"
#include "CreeperModel.h"
#include "HumanoidModel.h"
#include "SpiderModel.h"
#include "../Utilities/Memory.h"

Model ModelCreate()
{
	Model model = MemoryAllocate(sizeof(struct Model));
	*model = (struct Model){ 0 };
	return model;
}

void ModelRender(Model model, float anim, float t, float run, float2 rot, float offset)
{
	if (model->Type == ModelTypeAnimal) { return AnimalModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeCreeper) { return CreeperModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeHumanoid) { return HumanoidModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypePig) { return AnimalModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeSheepFur) { return AnimalModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeSheep) { return AnimalModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeSkeleton) { return HumanoidModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeSpider) { return SpiderModelRender(model, anim, t, run, rot, offset); }
	if (model->Type == ModelTypeZombie) { return HumanoidModelRender(model, anim, t, run, rot, offset); }
}

void ModelDestroy(Model model)
{
	if (model->Type == ModelTypeAnimal) { AnimalModelDestroy(model); }
	if (model->Type == ModelTypeCreeper) { CreeperModelDestroy(model); }
	if (model->Type == ModelTypeHumanoid) { HumanoidModelDestroy(model); }
	if (model->Type == ModelTypePig) { AnimalModelDestroy(model); }
	if (model->Type == ModelTypeSheepFur) { AnimalModelDestroy(model); }
	if (model->Type == ModelTypeSheep) { AnimalModelDestroy(model); }
	if (model->Type == ModelTypeSkeleton) { HumanoidModelDestroy(model); }
	if (model->Type == ModelTypeSpider) { SpiderModelDestroy(model); }
	if (model->Type == ModelTypeZombie) { HumanoidModelDestroy(model); }
	MemoryFree(model);
}
