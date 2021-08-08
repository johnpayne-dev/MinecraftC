#pragma once
#include "InputHandler.h"
#include "Inventory.h"
#include "PlayerAI.h"
#include "../Player/Player.h"

typedef Entity Player;

typedef struct PlayerData
{
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
	bool AllowAlpha;
	float RotationOffset;
	float BobbingStrength;
	float RenderOffset;
	float Tilt;
	float OldTilt;
	PlayerAI AI;
	InputHandler Input;
	Inventory Inventory;
	int UserType;
	float Bobbing;
	float OldBobbing;
} * PlayerData;

Player PlayerCreate(Level level);
void PlayerTick(Player player);
void PlayerTravel(Player player, float x, float y);
void PlayerResetPosition(Player player);
void PlayerStepAI(Player player);
void PlayerReleaseAllKeys(Player player);
void PlayerSetKey(Player player, int key, bool state);
bool PlayerAddResource(Player player, BlockType resource);
void PlayerDestroy(Player player);
