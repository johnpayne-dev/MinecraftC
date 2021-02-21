#pragma once
#include "AI/AI.h"
#include "../Entity.h"
#include "../Model/ModelManager.h"

typedef Entity Mob;

typedef enum MobType
{
	MobTypeNone,
	MobTypeCreeper,
	MobTypeHumanoid,
	MobTypePig,
	MobTypeSheep,
	MobTypeSkeleton,
	MobTypeSpider,
	MobTypeZombie,
	MobTypePlayer,
} MobType;

typedef struct MobData
{
	int InvulnerableDuration;
	float Rotation;
	float TimeOffset;
	float Speed;
	float RotationA;
	float BodyRotation;
	float OldBodyRotation;
	float Run;
	float OldRun;
	float AnimationStep;
	float OldAnimationStep;
	int TickCount;
	bool HasHair;
	const char * TextureName;
	bool AllowAlpha;
	float RotationOffset;
	const char * ModelName;
	float BobbingStrength;
	int DeathScore;
	float RenderOffset;
	int Health;
	int LastHealth;
	int InvulnerableTime;
	int AirSupply;
	int HurtTime;
	int HurtDuration;
	float HurtDirection;
	int DeathTime;
	int AttackTime;
	float Tilt;
	float OldTilt;
	bool Dead;
	AI AI;
	MobType Type;
	void * TypeData;
} * MobData;

Mob MobCreate(Level level);
bool MobIsPickable(Mob mob);
bool MobIsPushable(Mob mob);
void MobTick(Mob mob);
void MobStepAI(Mob mob);
void MobBindTexture(Mob mob, TextureManager textures);
void MobRender(Mob mob, TextureManager textures, float t);
void MobRenderModel(Mob mob, TextureManager textures, float anim, float t, float run, float2 rot, float offset);
void MobHeal(Mob mob, int health);
void MobHurt(Mob mob, Entity entity, int damage);
void MobKnockback(Mob mob, Entity entity, int damage, int x, int z);
void MobDie(Mob mob, Entity entity);
void MobCauseFallDamage(Mob mob, float height);
void MobTravel(Mob mob, float x, float y);
bool MobIsShootable(Mob mob);
void MobDestroy(Mob mob);

extern ModelManager MobModelCache;
