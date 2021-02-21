#include "Skeleton.h"
#include "SkeletonAI.h"
#include "../Item/Arrow.h"

Skeleton SkeletonCreate(Level level, float3 pos)
{
	Humanoid humanoid = HumanoidCreate(level, pos);
	humanoid->HeightOffset = 1.62;
	MobData mob = humanoid->TypeData;
	mob->Type = MobTypeSkeleton;
	mob->ModelName = "Skeleton";
	mob->TextureName = "Mob/Skeleton.png";
	AIDestroy(mob->AI);
	mob->AI = SkeletonAICreate(humanoid);
	mob->DeathScore = 120;
	mob->AI->DefaultLookAngle = 30;
	((BasicAIData)mob->AI->TypeData)->RunSpeed = 0.3;
	((BasicAttackAIData)((BasicAIData)mob->AI->TypeData)->TypeData)->Damage = 8;
	return humanoid;
}

void SkeletonShootArrow(Skeleton skeleton, Level level)
{
	LevelAddEntity(level, ArrowCreate(level, skeleton, skeleton->Position, skeleton->Rotation + (float2){ -RandomUniform() * 45.0 - 10.0, RandomUniform() * 45.0 - 22.5 + 180.0 }, 2.0));
}

void SkeletonShootRandomArrow(Skeleton skeleton)
{
	int count = (RandomUniform() + RandomUniform()) * 3.0 + 4.0;
	for (int i = 0; i < count; i++)
	{
		LevelAddEntity(skeleton->Level, ArrowCreate(skeleton->Level, LevelGetPlayer(skeleton->Level), skeleton->Position - up3f * 0.2, (float2){ RandomUniform() * 360.0, RandomUniform() * -60.0 }, 0.4));
	}
}
