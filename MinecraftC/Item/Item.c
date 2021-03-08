#include <OpenGL.h>
#include "Item.h"
#include "ItemModel.h"
#include "TakeAnimation.h"
#include "../Player/Player.h"

static ItemModel Models[256];

void ItemInitializeModels()
{
	for (int i = 0; i < 256; i++)
	{
		if (Blocks.Table[i] != NULL) { Models[i] = ItemModelCreate(Blocks.Table[i]->TextureID); }
		else { Models[i] = NULL; }
	}
}

void ItemDeinitializeModels()
{
	for (int i = 0; i < 256; i++)
	{
		if (Models[i] != NULL) { ItemModelDestroy(Models[i]); }
	}
}

Item ItemCreate(Level level, float3 pos, BlockType resource)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypeItem;
	
	EntitySetSize(entity, 0.25, 0.25);
	entity->HeightOffset = entity->AABBHeight / 2.0;
	entity->MakeStepSound = false;
	EntitySetPosition(entity, pos);
	ItemData item = MemoryAllocate(sizeof(struct ItemData));
	*item = (struct ItemData)
	{
		.Age = 0,
		.Resource = resource,
		.Rotation = RandomUniform() * 360.0,
		.Delta = (float3){ RandomUniform() * 0.2 - 0.1, 0.2, RandomUniform() * 0.2 - 0.1 },
	};
	entity->TypeData = item;
	return entity;
}

void ItemTick(Item item)
{
	ItemData this = item->TypeData;
	item->OldPosition = item->Position;
	this->Delta.y -= 0.04;
	EntityMove(item, this->Delta);
	this->Delta *= 0.98;
	if (item->OnGround) { this->Delta *= (float3){ 0.7, -0.5, 0.7 }; }
	
	this->TickCount++;
	this->Age++;
	if (this->Age >= 6000) { EntityRemove(item); }
}

void ItemRender(Item item, TextureManager textures, float t)
{
	ItemData this = item->TypeData;
	item->TextureID = TextureManagerLoad(textures, "Terrain.png");
	glBindTexture(GL_TEXTURE_2D, item->TextureID);
	float brightness = LevelGetBrightness(item->Level, item->Position.x, item->Position.y, item->Position.z);
	float rotation = this->Rotation + (this->TickCount + t) * 3.0;
	glPushMatrix();
	glColor4f(brightness, brightness, brightness, 1.0);
	float offset = sin(rotation / 10.0) * 0.1 + 0.1;
	float3 v = item->OldPosition + (item->Position - item->OldPosition) * t;
	glTranslatef(v.x, v.y + offset, v.z);
	glRotatef(rotation, 0.0, 1.0, 0.0);
	ItemModelGenerateList(Models[this->Resource]);
	float s = pow(sin(rotation / 10.0) * 0.5 + 0.5, 4.0);
	glColor4f(s * 0.4, s * 0.4, s * 0.4, s * 0.4);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_ALPHA_TEST);
	ItemModelGenerateList(Models[this->Resource]);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void ItemPlayerTouch(Item item, Entity player)
{
	if (PlayerAddResource(player, ((ItemData)item->TypeData)->Resource))
	{
		LevelAddEntity(item->Level, TakeAnimationCreate(item->Level, item, player));
		EntityRemove(item);
	}
}

void ItemDestroy(Item item)
{
	MemoryFree(item->TypeData);
}
