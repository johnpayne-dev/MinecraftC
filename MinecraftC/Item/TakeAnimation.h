#pragma once
#include "../Entity.h"
#include "../Level/Level.h"

typedef Entity TakeAnimation;

typedef struct TakeAnimationData
{
	int Time;
	Entity Item;
	Entity Player;
	float3 Original;
} * TakeAnimationData;

TakeAnimation TakeAnimationCreate(Level level, Entity item, Entity player);
void TakeAnimationTick(TakeAnimation take);
void TakeAnimationRender(TakeAnimation take, TextureManager textures, float t);
void TakeAnimationDestroy(TakeAnimation take);
