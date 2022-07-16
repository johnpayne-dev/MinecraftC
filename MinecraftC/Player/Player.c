#include "Player.h"
#include "PlayerAI.h"
#include "../Utilities/OpenGL.h"

Player PlayerCreate(Level level) {
	Entity entity = EntityCreate(level);
	entity->type = EntityTypePlayer;
	entity->footSize = 0.5;
	entity->heightOffset = 1.62;
	EntitySetPosition(entity, entity->position);
	PlayerData player = MemoryAllocate(sizeof(struct PlayerData));
	*player = (struct PlayerData) {
		.rotationA = (RandomUniform() + 1.0) * 0.01,
		.bodyRotation = 0.0,
		.oldBodyRotation = 0.0,
		.tickCount = 0,
		.allowAlpha = true,
		.bobbingStrength = 1.0,
		.renderOffset = 0.0,
		.timeOffset = RandomUniform() * 12398.0,
		.rotation = 2.0 * pi * RandomUniform(),
		.speed = 1.0,
		.ai = PlayerAICreate(entity),
		.rotationOffset = 180.0,
		.inventory = InventoryCreate(),
		.userType = 0,
	};
	entity->typeData = player;
	if (level != NULL) {
		level->player = entity;
		LevelAddEntity(level, entity);
	}
	return entity;
}

void PlayerTick(Player player) {
	PlayerData this = player->typeData;
	this->oldTilt = this->tilt;
	if (EntityIsInWater(player)) { player->fallDistance = 0.0; }
	
	this->oldAnimationStep = this->animationStep;
	this->oldBodyRotation = this->bodyRotation;
	player->oldRotation = player->rotation;
	this->tickCount++;
	PlayerStepAI(player);
	this->oldRun = this->run;
	float2 d = player->position.xz - player->oldPosition.xz;
	float len = length2f(d);
	float rot = this->bodyRotation;
	float f1 = 0.0, f2 = 0.0;
	if (len > 0.05) {
		f2 = 1.0;
		f1 = len * 3.0;
		rot = atan2(d.y, d.x) * deg - 90.0;
	}
	if (!player->onGround) { f2 = 0.0; }
	this->run += (f2 - this->run) * 0.3;
	
	float a;
	for (a = rot - this->bodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	this->bodyRotation += a * 0.1;
	for (a = player->rotation.y - this->bodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	bool b = a < -90.0 || a >= 90.0;
	if (a < -75.0) { a = -75.0; }
	if (a > 75.0) { a = 75.0; }
	this->bodyRotation = player->rotation.y - a;
	this->bodyRotation += a * 0.1;
	if (b) { f1 = -f1; }
	while (player->rotation.y - player->oldRotation.y < -180.0) { player->oldRotation.y -= 360.0; }
	while (player->rotation.y - player->oldRotation.y >= 180.0) { player->oldRotation.y += 360.0; }
	while (this->bodyRotation - this->oldBodyRotation < -180.0) { this->oldBodyRotation -= 360.0; }
	while (this->bodyRotation - this->oldBodyRotation >= 180.0) { this->oldBodyRotation += 360.0; }
	while (player->rotation.x - player->oldRotation.x < -180.0) { player->oldRotation.x -= 360.0; }
	while (player->rotation.x - player->oldRotation.x >= 180.0) { player->oldRotation.x += 360.0; }

	this->animationStep += f1;
}

void PlayerTravel(Player player, float x, float y) {
	if (EntityIsInWater(player)) {
		float z = player->position.y;
		EntityMoveRelative(player, (float2){ x, y }, 0.02);
		EntityMove(player, player->delta);
		player->delta *= 0.8;
		player->delta.y -= 0.02;
		if (player->horizontalCollision && EntityIsFree(player, player->delta + up3f * (0.6 - player->position.y + z))) { player->delta.y = 0.3; }
	} else if (EntityIsInLava(player)) {
		float z = player->position.y;
		EntityMoveRelative(player, (float2){ x, y }, 0.02);
		EntityMove(player, player->delta);
		player->delta *= 0.5;
		player->delta.y -= 0.02;
		if (player->horizontalCollision && EntityIsFree(player, player->delta + up3f * (0.6 - player->position.y + z))) { player->delta.y = 0.3; }
	} else {
		EntityMoveRelative(player, (float2){ x, y }, player->onGround ? 0.1 : 0.02);
		EntityMove(player, player->delta);
		player->delta *= (float3){ 0.91, 0.98, 0.91 };
		player->delta.y -= 0.08;
		if (player->onGround) { player->delta.xz *= 0.6; }
	}
}

void PlayerResetPosition(Player entity) {
	entity->heightOffset = 1.62;
	EntitySetSize(entity, 0.6, 1.8);
	entity->type = EntityTypeNone;
	EntityResetPosition(entity);
	entity->type = EntityTypePlayer;
	if (entity->level != NULL) { entity->level->player = entity; }
}

void PlayerStepAI(Player player) {
	PlayerData this = player->typeData;
	InventoryTick(this->inventory);
	this->oldBobbing = this->bobbing;
	InputHandlerUpdateMovement(this->input);
	PlayerAITick(this->ai, player->level, player);
	float bob = length2f(player->delta.xz);
	float tilt = atan(-player->delta.y * 0.2) * 15.0;
	if (bob > 0.1) { bob = 0.1; }
	if (!player->onGround) { bob = 0.0; }
	if (player->onGround) { tilt = 0.0; }
	this->bobbing += (bob - this->bobbing) * 0.4;
	this->tilt += (tilt - this->tilt) * 0.8;
}

void PlayerReleaseAllKeys(Player player) {
	PlayerData this = player->typeData;
	InputHandlerResetKeys(this->input);
}

void PlayerSetKey(Player player, int key, bool state) {
	PlayerData this = player->typeData;
	InputHandlerSetKeyState(this->input, key, state);
}

bool PlayerAddResource(Player player, BlockType resource) {
	PlayerData this = player->typeData;
	return InventoryAddResource(this->inventory, resource);
}

void PlayerDestroy(Player player) {
	PlayerData this = player->typeData;
	InventoryDestroy(this->inventory);
	MemoryFree(this);
}
