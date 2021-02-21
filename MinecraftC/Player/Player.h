#pragma once
#include "InputHandler.h"
#include "Inventory.h"
#include "../Model/HumanoidModel.h"
#include "../Mob/Mob.h"

typedef Mob Player;

typedef struct PlayerData
{
	InputHandler Input;
	Inventory Inventory;
	int UserType;
	float Bobbing;
	float OldBobbing;
	int Score;
	int Arrows;
} * PlayerData;

Player PlayerCreate(Level level);
void PlayerResetPosition(Player player);
void PlayerStepAI(Player player);
void PlayerRender(Player player, TextureManager textures, float t);
void PlayerReleaseAllKeys(Player player);
void PlayerSetKey(Player player, int key, bool state);
bool PlayerAddResource(Player player, BlockType resource);
int PlayerGetScore(Player player);
HumanoidModel PlayerGetModel(Player player);
void PlayerDie(Player player, Entity entity);
void PlayerRemove(Player player);
void PlayerAwardKillScore(Player player, Entity entity, int score);
bool PlayerIsShootable(Player player);
void PlayerBindTexture(Player player, TextureManager textures);
void PlayerHurt(Player player, Entity entity, int damage);
bool PlayerIsCreativeModeAllowed(Player player);
void PlayerDestroy(Player player);
