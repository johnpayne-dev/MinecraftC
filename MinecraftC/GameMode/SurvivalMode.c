#include "SurvivalMode.h"
#include "../Minecraft.h"

SurvivalMode SurvivalModeCreate(Minecraft minecraft)
{
	GameMode mode = GameModeCreate(minecraft);
	mode->TypeData = malloc(sizeof(struct SurvivalModeData));
	mode->Type = GameModeTypeSurvival;
	*(SurvivalModeData)mode->TypeData = (struct SurvivalModeData){ 0 };
	return mode;
}

void SurvivalModePreparePlayer(SurvivalMode mode, Player player)
{
	PlayerData playerData = ((MobData)player->TypeData)->TypeData;
	playerData->Inventory->Slots[8] = BlockTypeTNT;
	playerData->Inventory->Count[8] = 10;
}

void SurvivalModeBreakBlock(SurvivalMode mode, int x, int y, int z)
{
	BlockType blockType = LevelGetTile(mode->Minecraft->Level, x, y, z);
	BlockOnBreak(Blocks.Table[blockType], mode->Minecraft->Level, x, y, z);
}

bool SurvivalModeCanPlace(SurvivalMode mode, BlockType blockType)
{
	return InventoryRemoveResource(((PlayerData)((MobData)mode->Minecraft->Level->Player->TypeData)->TypeData)->Inventory, blockType);
}

void SurvivalModeHitBlock(SurvivalMode mode, int x, int y, int z)
{
	BlockType block = LevelGetTile(mode->Minecraft->Level, x, y, z);
	if (block > 0 && BlockGetHardness(Blocks.Table[block]) == 0) { GameModeBreakBlock(mode, x, y, z); }
}

void SurvivalModeResetHits(SurvivalMode mode)
{
	((SurvivalModeData)mode->TypeData)->Hits = 0;
	((SurvivalModeData)mode->TypeData)->HitDelay = 0;
}

void SurvivalModeHitBlockSide(SurvivalMode mode, int x, int y, int z, int side)
{
	SurvivalModeData this = mode->TypeData;
	if (this->HitDelay > 0) { this->HitDelay--; }
	else if (x == this->Hit.x && y == this->Hit.y && z == this->Hit.z)
	{
		BlockType blockType = LevelGetTile(mode->Minecraft->Level, x, y, z);
		if (blockType != 0)
		{
			Block block = Blocks.Table[blockType];
			this->Hardness = BlockGetHardness(block);
			BlockSpawnBlockParticles(block, mode->Minecraft->Level, x, y, z, side, mode->Minecraft->ParticleManager);
			this->Hits++;
			if (this->Hits == this->Hardness + 1)
			{
				GameModeBreakBlock(mode, x, y, z);
				this->Hits = 0;
				this->HitDelay = 5;
			}
		}
	}
	else
	{
		this->Hits = 0;
		this->Hit = (int3){ x, y, z };
	}
}

void SurvivalModeApplyCracks(SurvivalMode mode, float cracks)
{
	SurvivalModeData this = mode->TypeData;
	if (this->Hits <= 0) { mode->Minecraft->LevelRenderer->Cracks = 0.0; }
	else { mode->Minecraft->LevelRenderer->Cracks = (this->Hits + cracks - 1.0) / this->Hardness; }
}

float SurvivalModeGetReachDistance(SurvivalMode mode)
{
	return 4.0;
}

bool SurvivalModeUseItem(SurvivalMode mode, Player player, BlockType blockType)
{
	PlayerData playerData = mode->Minecraft->Player->TypeData;
	if (blockType == BlockTypeRedMushroom && InventoryRemoveResource(playerData->Inventory, blockType))
	{
		EntityHurt(player, NULL, 3);
		return true;
	}
	else if (blockType == BlockTypeBrownMushroom && InventoryRemoveResource(playerData->Inventory, blockType))
	{
		MobHeal(player, 5);
		return true;
	}
	else { return false; }
}

void SurvivalModeApply(SurvivalMode mode, Level level)
{
	((SurvivalModeData)mode->TypeData)->MobSpawner = MobSpawnerCreate(level);
}

void SurvivalModeSpawnMob(SurvivalMode mode)
{
	MobSpawner spawner = ((SurvivalModeData)mode->TypeData)->MobSpawner;
	int c = spawner->Level->Width * spawner->Level->Height * spawner->Level->Depth / 64 / 64 / 64;
	if (RandomGeneratorIntegerRange(spawner->Level->Random, 0, 99) < c && LevelCountInstancesOf(spawner->Level, EntityTypeMob) < c * 20)
	{
		MobSpawnerSpawn(spawner, c, spawner->Level->Player, NULL);
	}
}

void SurvivalModePrepareLevel(SurvivalMode mode, Level level)
{
	SurvivalModeData this = mode->TypeData;
	this->MobSpawner = MobSpawnerCreate(level);
	ProgressBarDisplaySetText(mode->Minecraft->ProgressBar, "Spawning..");
	int count = level->Width * level->Height * level->Depth / 800;
	MobSpawnerSpawn(this->MobSpawner, count, NULL, mode->Minecraft->ProgressBar);
}

void SurvivalModeDestroy(SurvivalMode mode)
{
	SurvivalModeData this = mode->TypeData;
	if (this->MobSpawner != NULL) { MobSpawnerDestroy(this->MobSpawner); }
	free(this);
}
