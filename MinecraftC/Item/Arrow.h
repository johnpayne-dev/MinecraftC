#pragma once
#include "../Entity.h"
#include "../Level/Level.h"

typedef Entity Arrow;

typedef struct ArrowData
{
	float3 Delta;
	float2 Rotation;
	float2 OldRotation;
	bool HasHit;
	int StickTime;
	Entity Owner;
	int Time;
	int Type;
	float Gravity;
	int Damage;
} * ArrowData;

Arrow ArrowCreate(Level level, Entity owner, float3 pos, float2 rot, float g);
void ArrowTick(Arrow arrow);
void ArrowRender(Arrow arrow, TextureManager textures, float t);
void ArrowAwardKillScore(Arrow arrow, Entity entity, int score);
Entity ArrowGetOwner(Arrow arrow);
void ArrowPlayerTouch(Arrow arrow, Entity player);
void ArrowDestroy(Arrow arrow);
