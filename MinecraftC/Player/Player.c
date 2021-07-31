#include <OpenGL.h>
#include "Player.h"
#include "PlayerAI.h"

Player PlayerCreate(Level level)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypePlayer;
	entity->FootSize = 0.5;
	entity->HeightOffset = 1.62;
	EntitySetPosition(entity, entity->Position);
	PlayerData player = MemoryAllocate(sizeof(struct PlayerData));
	*player = (struct PlayerData)
	{
		.RotationA = (RandomUniform() + 1.0) * 0.01,
		.BodyRotation = 0.0,
		.OldBodyRotation = 0.0,
		.TickCount = 0,
		.AllowAlpha = true,
		.BobbingStrength = 1.0,
		.RenderOffset = 0.0,
		.TimeOffset = RandomUniform() * 12398.0,
		.Rotation = 2.0 * pi * RandomUniform(),
		.Speed = 1.0,
		.AI = PlayerAICreate(entity),
		.RotationOffset = 180.0,
		.Inventory = InventoryCreate(),
		.UserType = 0,
	};
	entity->TypeData = player;
	if (level != NULL)
	{
		level->Player = entity;
		LevelAddEntity(level, entity);
	}
	return entity;
}

void PlayerTick(Player player)
{
	PlayerData this = player->TypeData;
	this->OldTilt = this->Tilt;
	if (EntityIsInWater(player)) { player->FallDistance = 0.0; }
	
	this->OldAnimationStep = this->AnimationStep;
	this->OldBodyRotation = this->BodyRotation;
	player->OldRotation = player->Rotation;
	this->TickCount++;
	PlayerStepAI(player);
	this->OldRun = this->Run;
	float2 d = player->Position.xz - player->OldPosition.xz;
	float len = length2f(d);
	float rot = this->BodyRotation;
	float f1 = 0.0, f2 = 0.0;
	if (len > 0.05)
	{
		f2 = 1.0;
		f1 = len * 3.0;
		rot = atan2(d.y, d.x) * deg - 90.0;
	}
	if (!player->OnGround) { f2 = 0.0; }
	this->Run += (f2 - this->Run) * 0.3;
	
	float a;
	for (a = rot - this->BodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	this->BodyRotation += a * 0.1;
	for (a = player->Rotation.y - this->BodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	bool b = a < -90.0 || a >= 90.0;
	if (a < -75.0) { a = -75.0; }
	if (a > 75.0) { a = 75.0; }
	this->BodyRotation = player->Rotation.y - a;
	this->BodyRotation += a * 0.1;
	if (b) { f1 = -f1; }
	while (player->Rotation.y - player->OldRotation.y < -180.0) { player->OldRotation.y -= 360.0; }
	while (player->Rotation.y - player->OldRotation.y >= 180.0) { player->OldRotation.y += 360.0; }
	while (this->BodyRotation - this->OldBodyRotation < -180.0) { this->OldBodyRotation -= 360.0; }
	while (this->BodyRotation - this->OldBodyRotation >= 180.0) { this->OldBodyRotation += 360.0; }
	while (player->Rotation.x - player->OldRotation.x < -180.0) { player->OldRotation.x -= 360.0; }
	while (player->Rotation.x - player->OldRotation.x >= 180.0) { player->OldRotation.x += 360.0; }

	this->AnimationStep += f1;
}

void PlayerTravel(Player player, float x, float y)
{
	if (EntityIsInWater(player))
	{
		float z = player->Position.y;
		EntityMoveRelative(player, (float2){ x, y }, 0.02);
		EntityMove(player, player->Delta);
		player->Delta *= 0.8;
		player->Delta.y -= 0.02;
		if (player->HorizontalCollision && EntityIsFree(player, player->Delta + up3f * (0.6 - player->Position.y + z))) { player->Delta.y = 0.3; }
	}
	else if (EntityIsInLava(player))
	{
		float z = player->Position.y;
		EntityMoveRelative(player, (float2){ x, y }, 0.02);
		EntityMove(player, player->Delta);
		player->Delta *= 0.5;
		player->Delta.y -= 0.02;
		if (player->HorizontalCollision && EntityIsFree(player, player->Delta + up3f * (0.6 - player->Position.y + z))) { player->Delta.y = 0.3; }
	}
	else
	{
		EntityMoveRelative(player, (float2){ x, y }, player->OnGround ? 0.1 : 0.02);
		EntityMove(player, player->Delta);
		player->Delta *= (float3){ 0.91, 0.98, 0.91 };
		player->Delta.y -= 0.08;
		if (player->OnGround) { player->Delta.xz *= 0.6; }
	}
}

void PlayerResetPosition(Player entity)
{
	entity->HeightOffset = 1.62;
	EntitySetSize(entity, 0.6, 1.8);
	entity->Type = EntityTypeNone;
	EntityResetPosition(entity);
	entity->Type = EntityTypePlayer;
	if (entity->Level != NULL) { entity->Level->Player = entity; }
}

void PlayerStepAI(Player player)
{
	PlayerData this = player->TypeData;
	InventoryTick(this->Inventory);
	this->OldBobbing = this->Bobbing;
	InputHandlerUpdateMovement(this->Input);
	PlayerAITick(this->AI, player->Level, player);
	float bob = length2f(player->Delta.xz);
	float tilt = atan(-player->Delta.y * 0.2) * 15.0;
	if (bob > 0.1) { bob = 0.1; }
	if (!player->OnGround) { bob = 0.0; }
	if (player->OnGround) { tilt = 0.0; }
	this->Bobbing += (bob - this->Bobbing) * 0.4;
	this->Tilt += (tilt - this->Tilt) * 0.8;
}

void PlayerReleaseAllKeys(Player player)
{
	PlayerData this = player->TypeData;
	InputHandlerResetKeys(this->Input);
}

void PlayerSetKey(Player player, int key, bool state)
{
	PlayerData this = player->TypeData;
	InputHandlerSetKeyState(this->Input, key, state);
}

bool PlayerAddResource(Player player, BlockType resource)
{
	PlayerData this = player->TypeData;
	return InventoryAddResource(this->Inventory, resource);
}

void PlayerBindTexture(Player player, TextureManager textures)
{
	//if (PlayerNewTexture != NULL)
	//{
	//	PlayerNewTextureID = TextureManagerLoad(textures, PlayerNewTexture);
	//	PlayerNewTexture = NULL;
	//}
	
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(textures, "Char.png"));
}

void PlayerDestroy(Player player)
{
	PlayerData this = player->TypeData;
	InventoryDestroy(this->Inventory);
	MemoryFree(this);
}
