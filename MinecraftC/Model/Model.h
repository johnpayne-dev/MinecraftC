#pragma once
#include "../Utilities/LinearMath.h"

typedef enum ModelType
{
	ModelTypeNone,
	ModelTypeAnimal,
	ModelTypeCreeper,
	ModelTypeHumanoid,
	ModelTypePig,
	ModelTypeSheepFur,
	ModelTypeSheep,
	ModelTypeSkeleton,
	ModelTypeSpider,
	ModelTypeZombie,
} ModelType;

typedef struct Model
{
	float AttackOffset;
	ModelType Type;
	void * TypeData;
} * Model;

Model ModelCreate(void);
void ModelRender(Model model, float anim, float t, float run, float2 rot, float offset);
void ModelDestroy(Model model);
