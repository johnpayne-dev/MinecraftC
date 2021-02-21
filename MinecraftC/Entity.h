#pragma once
#include "Physics/AABB.h"
#include "Render/TextureManager.h"

typedef enum EntityType
{
	EntityTypeNone,
	EntityTypeArrow,
	EntityTypeItem,
	EntityTypePrimedTNT,
	EntityTypeTakeAnimation,
	EntityTypeParticle,
	EntityTypeMob,
} EntityType;

typedef struct Entity
{
	struct Level * Level;
	float3 OldPosition;
	float3 Position;
	float3 Delta;
	float2 OldRotation;
	float2 Rotation;
	AABB AABB;
	bool OnGround;
	bool HorizontalCollision;
	bool Collision;
	bool Slide;
	bool Removed;
	float HeightOffset;
	float AABBWidth;
	float AABBHeight;
	float OldWalkDistance;
	float WalkDistance;
	bool MakeStepSound;
	float FallDistance;
	int NextStep;
	struct BlockMap * BlockMap;
	float3 XYZOld;
	int TextureID;
	float YSlideOffset;
	float FootSize;
	bool NoPhysics;
	float PushThrough;
	bool Hovered;
	EntityType Type;
	void * TypeData;
} * Entity;

Entity EntityCreate(struct Level * level);
void EntityResetPosition(Entity entity);
void EntityRemove(Entity entity);
void EntitySetSize(Entity entity, float w, float h);
void EntitySetPosition(Entity entity, float3 pos);
void EntityTurn(Entity entity, float2 angle);
void EntityInterpolateTurn(Entity entity, float2 angle);
void EntityTick(Entity entity);
bool EntityIsFree(Entity entity, float3 a);
bool EntityIsFreeScaled(Entity entity, float3 a, float s);
void EntityMove(Entity entity, float3 a);
void EntityCauseFallDamage(Entity entity, float height);
bool EntityIsInWater(Entity entity);
bool EntityIsUnderWater(Entity entity);
bool EntityIsInLava(Entity entity);
void EntityMoveRelative(Entity entity, float2 xz, float speed);
bool EntityIsLit(Entity entity);
float EntityGetBrightness(Entity entity, float t);
void EntityRender(Entity entity, TextureManager textures, float t);
void EntitySetLevel(Entity entity, struct Level * level);
void EntityPlaySound(Entity entity, const char * name, float volume, float pitch);
void EntityMoveTo(Entity entity, float3 pos, float2 rot);
float EntityDistanceTo(Entity entityA, Entity entityB);
float EntityDistanceToPoint(Entity entity, float3 point);
float EntitySquaredDistanceTo(Entity entityA, Entity entityB);
void EntityPlayerTouch(Entity entity, Entity player);
void EntityPush(Entity entityA, Entity entityB);
void EntityPushTowards(Entity entity, float3 point);
void EntityHurt(Entity entityA, Entity entityB, int damage);
bool EntityIntersects(Entity entity, float3 v0, float3 v1);
bool EntityIsPickable(Entity entity);
bool EntityIsPushable(Entity entity);
bool EntityIsShootable(Entity entity);
void EntityAwardKillScore(Entity entityA, Entity entityB, int score);
bool EntityShouldRender(Entity entity, float3 v);
bool EntityShouldRenderAtSquaredDistance(Entity entity, float v);
int EntityGetTexture(Entity entity);
bool EntityIsCreativeModeAllowed(Entity entity);
void EntityRenderHover(Entity entity, float dt);
void EntityDestroy(Entity entity);
