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
	float xo, yo, zo;
	float x, y, z;
	float xd, yd, zd;
	float yRot, xRot;
	float yRotO, xRotO;
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
	float xOld, yOld, zOld;
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
void EntitySetPosition(Entity entity, float x, float y, float z);
void EntityTurn(Entity entity, float rx, float ry);
void EntityInterpolateTurn(Entity entity, float rx, float ry);
void EntityTick(Entity entity);
bool EntityIsFree(Entity entity, float x, float y, float z);
bool EntityIsFreeScaled(Entity entity, float x, float y, float z, float s);
void EntityMove(Entity entity, float x, float y, float z);
bool EntityIsInWater(Entity entity);
bool EntityIsUnderWater(Entity entity);
bool EntityIsInLava(Entity entity);
void EntityMoveRelative(Entity entity, float x, float z, float speed);
bool EntityIsLit(Entity entity);
float EntityGetBrightness(Entity entity, float t);
void EntitySetLevel(Entity entity, struct Level * level);
void EntityPlaySound(Entity entity, const char * name, float volume, float pitch);
void EntityMoveTo(Entity entity, float x, float y, float z, float rx, float ry);
bool EntityIntersects(Entity entity, float x0, float y0, float z0, float x1, float y1, float z1);
bool EntityIsPickable(Entity entity);
void EntityRender(Entity entity, TextureManager textures, float t);
int EntityGetTexture(Entity entity);
void EntityDestroy(Entity entity);
