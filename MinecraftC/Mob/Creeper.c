#include "Creeper.h"
#include "CreeperAI.h"

Creeper CreeperCreate(Level level, float3 pos)
{
	Mob creeper = MobCreate(level);
	creeper->HeightOffset = 1.62;
	MobData mob = creeper->TypeData;
	mob->Type = MobTypeCreeper;
	mob->ModelName = "Creeper";
	mob->TextureName = "Mob/Creeper.png";
	AIDestroy(mob->AI);
	mob->AI = CreeperAICreate(creeper);
	mob->AI->DefaultLookAngle = 45;
	mob->DeathScore = 200;
	EntitySetPosition(creeper, pos);
	return creeper;
}

float CreeperGetBrightness(Creeper creeper, float t)
{
	MobData mob = creeper->TypeData;
	float h = (20 - mob->Health) / 20.0;
	float brightness = (sin(mob->TickCount + t) * 0.5 + 0.5) * h * 0.5 + 0.25 + h * 0.25;
	return brightness * LevelGetBrightness(creeper->Level, creeper->Position.x, creeper->Position.y, creeper->Position.z);
}
