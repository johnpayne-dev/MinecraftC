#pragma once
#include "Mob.h"

typedef Mob Sheep;

typedef struct SheepData
{
	bool HasFur;
	bool Grazing;
	int GrazingTime;
	float Graze;
	float OldGraze;
} * SheepData;

Sheep SheepCreate(Level level, float3 pos);
void SheepStepAI(Sheep sheep);
void SheepDie(Sheep sheep, Entity entity);
void SheepHurt(Sheep sheep, Entity entity, int damage);
void SheepRenderModel(Sheep sheep, TextureManager textures, float anim, float t, float run, float2 rot, float offset);
void SheepDestroy(Sheep sheep);
