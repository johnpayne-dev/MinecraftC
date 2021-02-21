#include "Zombie.h"
#include "AI/BasicAttackAI.h"

Zombie ZombieCreate(Level level, float3 pos)
{
	Humanoid humanoid = HumanoidCreate(level, pos);
	humanoid->HeightOffset = 1.62;
	MobData mob = humanoid->TypeData;
	mob->Type = MobTypeZombie;
	mob->ModelName = "Zombie";
	mob->TextureName = "Mob/Zombie.png";
	AIDestroy(mob->AI);
	mob->AI = BasicAttackAICreate();
	mob->DeathScore = 80;
	mob->AI->DefaultLookAngle = 30;
	((BasicAIData)mob->AI->TypeData)->RunSpeed = 1.0;
	return humanoid;
}
