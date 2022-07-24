#pragma once
#include "Physics/AABB.h"
#include "Render/TextureManager.h"
#include "Player/Player.h"
#include "Particle/PrimedTNT.h"
#include "Particle/Particle.h"

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
	float ySlideOffset;
	float footSize;
	bool noPhysics;
	EntityType type;
	union {
		PlayerData player;
		PrimedTNTData tnt;
		ParticleData particle;
	};
} Entity;

void EntityCreate(Entity * entity, struct Level * level);
void EntityResetPosition(Entity * entity);
void EntityRemove(Entity * entity);
void EntitySetSize(Entity * entity, float w, float h);
void EntitySetPosition(Entity * entity, float x, float y, float z);
void EntityTurn(Entity * entity, float rx, float ry);
void EntityTick(Entity * entity);
bool EntityIsFree(Entity * entity, float x, float y, float z);
void EntityMove(Entity * entity, float x, float y, float z);
bool EntityIsInWater(Entity * entity);
bool EntityIsUnderWater(Entity * entity);
bool EntityIsInLava(Entity * entity);
void EntityMoveRelative(Entity * entity, float x, float z, float speed);
float EntityGetBrightness(Entity * entity, float t);
void EntitySetLevel(Entity * entity, struct Level * level);
void EntityPlaySound(Entity * entity, char * name, float volume, float pitch);
bool EntityIsPickable(Entity * entity);
void EntityRender(Entity * entity, TextureManager * textures, float dt);
