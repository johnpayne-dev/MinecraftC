#pragma once
#include "Physics/AABB.h"
#include "Render/TextureManager.h"

typedef enum EntityType {
	EntityTypeNone,
	EntityTypePrimedTNT,
	EntityTypeParticle,
	EntityTypePlayer,
} EntityType;

typedef struct Entity {
	struct Level * level;
	float3 oldPosition;
	float3 position;
	float3 delta;
	float2 oldRotation;
	float2 rotation;
	AABB aabb;
	bool onGround;
	bool horizontalCollision;
	bool collision;
	bool slide;
	bool removed;
	float heightOffset;
	float aabbWidth;
	float aabbHeight;
	float oldWalkDistance;
	float walkDistance;
	bool makeStepSound;
	float fallDistance;
	int nextStep;
	struct BlockMap * blockMap;
	float3 xyzOld;
	int textureID;
	float ySlideOffset;
	float footSize;
	bool noPhysics;
	float pushThrough;
	bool hovered;
	EntityType type;
	void * typeData;
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
bool EntityIsInWater(Entity entity);
bool EntityIsUnderWater(Entity entity);
bool EntityIsInLava(Entity entity);
void EntityMoveRelative(Entity entity, float2 xz, float speed);
bool EntityIsLit(Entity entity);
float EntityGetBrightness(Entity entity, float t);
void EntitySetLevel(Entity entity, struct Level * level);
void EntityPlaySound(Entity entity, const char * name, float volume, float pitch);
void EntityMoveTo(Entity entity, float3 pos, float2 rot);
float EntityDistanceTo(Entity entityA, Entity entityB);
float EntityDistanceToPoint(Entity entity, float3 point);
float EntitySquaredDistanceTo(Entity entityA, Entity entityB);
bool EntityIntersects(Entity entity, float3 v0, float3 v1);
bool EntityIsPickable(Entity entity);
void EntityRender(Entity entity, TextureManager textures, float t);
bool EntityShouldRender(Entity entity, float3 v);
bool EntityShouldRenderAtSquaredDistance(Entity entity, float v);
int EntityGetTexture(Entity entity);
void EntityDestroy(Entity entity);
