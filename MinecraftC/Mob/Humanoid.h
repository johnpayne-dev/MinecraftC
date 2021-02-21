#pragma once
#include "Mob.h"

typedef Mob Humanoid;

typedef struct HumanoidData
{
	bool Helmet;
	bool Armor;
} * HumanoidData;

Humanoid HumanoidCreate(Level level, float3 pos);
void HumanoidRenderModel(Humanoid humanoid, TextureManager textures, float anim, float t, float run, float2 rot, float offset);
void HumanoidDestroy(Humanoid humanoid);
