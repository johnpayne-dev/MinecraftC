#include "TakeAnimation.h"
#include "../Utilities/Memory.h"

TakeAnimation TakeAnimationCreate(Level level, Entity item, Entity player)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypeTakeAnimation;
	EntitySetSize(entity, 1.0, 1.0);
	TakeAnimationData take = MemoryAllocate(sizeof(struct TakeAnimationData));
	*take = (struct TakeAnimationData)
	{
		.Time = 0,
		.Item = item,
		.Player = player,
		.Original = item->Position,
	};
	entity->TypeData = take;
	return entity;
}

void TakeAnimationTick(TakeAnimation take)
{
	TakeAnimationData this = take->TypeData;
	this->Time++;
	if (this->Time >= 3) { EntityRemove(take); }
	
	float t = pow(this->Time / 3.0, 2.0);
	take->OldPosition = this->Item->OldPosition = this->Item->Position;
	take->Position = this->Item->Position = this->Original + (this->Player->Position - this->Original - up3f) * t;
	EntitySetPosition(take, take->Position);
}

void TakeAnimationRender(TakeAnimation take, TextureManager textures, float t)
{
	EntityRender(((TakeAnimationData)take->TypeData)->Item, textures, t);
}

void TakeAnimationDestroy(TakeAnimation take)
{
	MemoryFree(take->TypeData);
}
