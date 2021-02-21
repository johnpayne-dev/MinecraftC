#include "Pig.h"
#include "../Item/Item.h"

Pig PigCreate(Level level, float3 pos)
{
	Pig entity = MobCreate(level);
	entity->HeightOffset = 1.72;
	EntitySetSize(entity, 1.4, 1.2);
	EntitySetPosition(entity, pos);
	MobData mob = entity->TypeData;
	mob->Type = MobTypePig;
	mob->ModelName = "Pig";
	mob->TextureName = "Mob/Pig.png";
	return entity;
}

void PigDie(Pig pig, Entity entity)
{
	if (entity != NULL) { EntityAwardKillScore(entity, pig, 10); }
	int spawn = RandomUniform() + RandomUniform() + 1.0;
	for (int i = 0; i < spawn; i++) { LevelAddEntity(pig->Level, ItemCreate(pig->Level, pig->Position, BlockTypeBrownMushroom)); }
}
