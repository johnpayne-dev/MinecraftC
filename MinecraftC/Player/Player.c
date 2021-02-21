#include <OpenGL.h>
#include "Player.h"
#include "PlayerAI.h"

Player PlayerCreate(Level level)
{
	Entity player = MobCreate(level);
	player->HeightOffset = 1.62;
	MobData mob = player->TypeData;
	mob->Health = 20;
	mob->ModelName = "Humanoid";
	mob->RotationOffset = 180.0;
	AIDestroy(mob->AI);
	mob->AI = PlayerAICreate(player);
	mob->Type = MobTypePlayer;
	PlayerData this = MemoryAllocate(sizeof(struct PlayerData));
	*this = (struct PlayerData)
	{
		.Inventory = InventoryCreate(),
		.UserType = 0,
		.Score = 0,
		.Arrows = 20,
	};
	mob->TypeData = this;
	if (level != NULL)
	{
		level->Player = player;
		LevelRemoveEntity(level, player);
		LevelAddEntity(level, player);
	}
	return player;
}

void PlayerResetPosition(Player player)
{
	MobData mob = player->TypeData;
	player->HeightOffset = 1.62;
	EntitySetSize(player, 0.6, 1.8);
	mob->Type = MobTypeNone;
	EntityResetPosition(player);
	mob->Type = MobTypePlayer;
	if (player->Level != NULL) { player->Level->Player = player; }
	mob->Health = 20;
	mob->DeathTime = 0;
}

void PlayerStepAI(Player player)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	InventoryTick(this->Inventory);
	this->OldBobbing = this->Bobbing;
	InputHandlerUpdateMovement(this->Input);
	mob->Type = MobTypeNone;
	MobStepAI(player);
	mob->Type = MobTypePlayer;
	float bob = length2f(player->Delta.xz);
	float tilt = atan(-player->Delta.y * 0.2) * 15.0;
	if (bob > 0.1) { bob = 0.1; }
	if (!player->OnGround || mob->Health <= 0) { bob = 0.0; }
	if (player->OnGround || mob->Health <= 0) { tilt = 0.0; }
	this->Bobbing += (bob - this->Bobbing) * 0.4;
	mob->Tilt += (tilt - mob->Tilt) * 0.8;
	
	list(Entity) entities = LevelFindEntities(player->Level, AABBGrow(player->AABB, (float3){ 1.0, 0.0, 1.0 }));
	if (mob->Health > 0 && entities != NULL)
	{
		for (int i = 0; i < ListCount(entities); i++) { EntityPlayerTouch(entities[i], player); }
	}
	ListDestroy(entities);
}

void PlayerRender(Player player, TextureManager textures, float t)
{
	
}

void PlayerReleaseAllKeys(Player player)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	InputHandlerResetKeys(this->Input);
}

void PlayerSetKey(Player player, int key, bool state)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	InputHandlerSetKeyState(this->Input, key, state);
}

bool PlayerAddResource(Player player, BlockType resource)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	return InventoryAddResource(this->Inventory, resource);
}

int PlayerGetScore(Player player)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	return this->Score;
}

HumanoidModel PlayerGetModel(Player player)
{
	MobData mob = player->TypeData;
	return ModelManagerGetModel(MobModelCache, mob->ModelName);
}

void PlayerDie(Player player, Entity entity)
{
	MobData mob = player->TypeData;
	EntitySetSize(player, 0.2, 0.2);
	EntitySetPosition(player, player->Position);
	player->Delta.y = 0.1;
	if (entity != NULL)
	{
		player->Delta.xz = (float2){ cos((mob->HurtDirection + player->Rotation.y) * rad), sin((mob->HurtDirection + player->Rotation.y) * rad) } * 0.1;
	}
	else { player->Delta.xz = zero2f; }
	player->HeightOffset = 0.1;
}

void PlayerRemove(Player player)
{
	
}

void PlayerAwardKillScore(Player player, Entity entity, int score)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	this->Score += score;
}

bool PlayerIsShootable(Player player)
{
	return true;
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

void PlayerHurt(Player player, Entity entity, int damage)
{
	MobData mob = player->TypeData;
	if (!player->Level->CreativeMode)
	{
		mob->Type = MobTypeNone;
		MobHurt(player, entity, damage);
		mob->Type = MobTypePlayer;
	}
}

bool PlayerIsCreativeModeAllowed(Player player)
{
	return true;
}

void PlayerDestroy(Player player)
{
	MobData mob = player->TypeData;
	PlayerData this = mob->TypeData;
	InventoryDestroy(this->Inventory);
	MemoryFree(this);
}
