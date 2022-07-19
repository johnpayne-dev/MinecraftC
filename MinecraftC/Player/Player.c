#include "Player.h"
#include "PlayerAI.h"
#include "../Utilities/OpenGL.h"
#include "../Level/Level.h"

void PlayerCreate(Player * entity, Level * level) {
	EntityCreate(entity, level);
	entity->type = EntityTypePlayer;
	entity->footSize = 0.5;
	entity->heightOffset = 1.62;
	EntitySetPosition(entity, entity->x, entity->y, entity->z);
	PlayerData * player = &entity->player;
	*player = (PlayerData) {
		.bodyRotation = 0.0,
		.oldBodyRotation = 0.0,
		.userType = 0,
	};
	InventoryCreate(&player->inventory);
	PlayerAICreate(&player->ai, entity);
	if (level != NULL) {
		level->player = entity;
		LevelAddEntity(level, entity);
	}
}

void PlayerTick(Player * player) {
	PlayerData * this = &player->player;
	this->oldTilt = this->tilt;
	if (EntityIsInWater(player)) { player->fallDistance = 0.0; }
	
	this->oldBodyRotation = this->bodyRotation;
	player->xRotO = player->xRot;
	player->yRotO = player->yRot;
	PlayerStepAI(player);
	float dx = player->x - player->xo;
	float dz = player->z - player->zo;
	float len = sqrtf(dx * dx + dz * dz);
	float rot = this->bodyRotation;
	float f1 = 0.0;
	if (len > 0.05) {
		f1 = len * 3.0;
		rot = atan2(dz, dx) * (180.0 / M_PI) - 90.0;
	}
	
	float a;
	for (a = rot - this->bodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	this->bodyRotation += a * 0.1;
	for (a = player->yRot - this->bodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	bool b = a < -90.0 || a >= 90.0;
	if (a < -75.0) { a = -75.0; }
	if (a > 75.0) { a = 75.0; }
	this->bodyRotation = player->yRot - a;
	this->bodyRotation += a * 0.1;
	if (b) { f1 = -f1; }
	while (player->yRot - player->yRotO < -180.0) { player->yRotO -= 360.0; }
	while (player->yRot - player->yRotO >= 180.0) { player->yRotO += 360.0; }
	while (this->bodyRotation - this->oldBodyRotation < -180.0) { this->oldBodyRotation -= 360.0; }
	while (this->bodyRotation - this->oldBodyRotation >= 180.0) { this->oldBodyRotation += 360.0; }
	while (player->xRot - player->xRotO < -180.0) { player->xRotO -= 360.0; }
	while (player->xRot - player->xRotO >= 180.0) { player->xRotO += 360.0; }
}

void PlayerTravel(Player * player, float x, float y) {
	if (EntityIsInWater(player)) {
		float z = player->y;
		EntityMoveRelative(player, x, y, 0.02);
		EntityMove(player, player->xd, player->yd, player->zd);
		player->xd *= 0.8;
		player->yd *= 0.8;
		player->zd *= 0.8;
		player->yd -= 0.02;
		if (player->horizontalCollision && EntityIsFree(player, player->xd, player->yd + 0.6 - player->y + z, player->zd)) { player->yd = 0.3; }
	} else if (EntityIsInLava(player)) {
		float z = player->y;
		EntityMoveRelative(player, x, y, 0.02);
		EntityMove(player, player->xd, player->yd, player->zd);
		player->xd *= 0.5;
		player->yd *= 0.5;
		player->zd *= 0.5;
		player->yd -= 0.02;
		if (player->horizontalCollision && EntityIsFree(player, player->xd, player->yd + 0.6 - player->y + z, player->zd)) { player->yd = 0.3; }
	} else {
		EntityMoveRelative(player, x, y, player->onGround ? 0.1 : 0.02);
		EntityMove(player, player->xd, player->yd, player->zd);
		player->xd *= 0.91;
		player->yd *= 0.98;
		player->zd *= 0.91;
		player->yd -= 0.08;
		if (player->onGround) {
			player->xd *= 0.6;
			player->zd *= 0.6;
		}
	}
}

void PlayerResetPosition(Player * entity) {
	entity->heightOffset = 1.62;
	EntitySetSize(entity, 0.6, 1.8);
	entity->type = EntityTypeNone;
	EntityResetPosition(entity);
	entity->type = EntityTypePlayer;
	if (entity->level != NULL) { entity->level->player = entity; }
}

void PlayerStepAI(Player * player) {
	PlayerData * this = &player->player;
	this->oldBobbing = this->bobbing;
	InputHandlerUpdateMovement(&this->input);
	PlayerAITick(&this->ai, player);
	float bob = sqrtf(player->xd * player->xd + player->zd * player->zd);
	float tilt = atan(-player->yd * 0.2) * 15.0;
	if (bob > 0.1) { bob = 0.1; }
	if (!player->onGround) { bob = 0.0; }
	if (player->onGround) { tilt = 0.0; }
	this->bobbing += (bob - this->bobbing) * 0.4;
	this->tilt += (tilt - this->tilt) * 0.8;
}

void PlayerReleaseAllKeys(Player * player) {
	PlayerData * this = &player->player;
	InputHandlerResetKeys(&this->input);
}

void PlayerSetKey(Player * player, int key, bool state) {
	PlayerData * this = &player->player;
	InputHandlerSetKeyState(&this->input, key, state);
}
