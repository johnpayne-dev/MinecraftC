#include <OpenGL.h>
#include "Sheep.h"
#include "SheepAI.h"
#include "../Item/Item.h"

Sheep SheepCreate(Level level, float3 pos)
{
	Sheep entity = MobCreate(level);
	EntitySetSize(entity, 1.4, 1.72);
	EntitySetPosition(entity, pos);
	entity->HeightOffset = 1.72f;
	MobData mob = entity->TypeData;
	mob->Type = MobTypeSheep;
	mob->ModelName = "Sheep";
	mob->TextureName = "Mob/Sheep.png";
	AIDestroy(mob->AI);
	mob->AI = SheepAICreate(entity);
	SheepData sheep = MemoryAllocate(sizeof(struct SheepData));
	*sheep = (struct SheepData)
	{
		.HasFur = true,
		.Grazing = false,
		.GrazingTime = 0,
	};
	mob->TypeData = sheep;
	return entity;
}

void SheepStepAI(Sheep sheep)
{
	MobData mob = sheep->TypeData;
	SheepData this = mob->TypeData;
	this->OldGraze = this->Graze;
	if (this->Grazing) { this->Graze += 0.2; }
	else { this->Graze -= 0.2; }
	if (this->Graze < 0.0) { this->Graze = 0.0; }
	if (this->Graze > 1.0) { this->Graze = 1.0; }
}

void SheepDie(Sheep sheep, Entity entity)
{
	if (entity != NULL) { EntityAwardKillScore(entity, sheep, 10); }
	int drops = RandomUniform() + RandomUniform() + 1.0;
	for (int i = 0; i < drops; i++) { LevelAddEntity(sheep->Level, ItemCreate(sheep->Level, sheep->Position, BlockTypeBrownMushroom)); }
}

void SheepHurt(Sheep sheep, Entity entity, int damage)
{
	MobData mob = sheep->TypeData;
	SheepData this = mob->TypeData;
	
	if (this->HasFur && entity->Type == EntityTypeMob && ((MobData)entity->TypeData)->Type == MobTypePlayer)
	{
		this->HasFur = false;
		int drops = RandomUniform() * 3.0 + 1.0;
		for (int i = 0; i < drops; i++) { LevelAddEntity(sheep->Level, ItemCreate(sheep->Level, sheep->Position, BlockTypeWhiteWool)); }
	}
}

void SheepRenderModel(Sheep sheep, TextureManager textures, float anim, float t, float run, float2 rot, float offset)
{
	MobData mob = sheep->TypeData;
	SheepData this = mob->TypeData;
	Model model = ModelManagerGetModel(MobModelCache, mob->ModelName);
	AnimalModelData modelData = model->TypeData;
	
	float3 p = modelData->Head->Position;
	modelData->Head->Position.y += (this->OldGraze + (this->Graze - this->OldGraze) * t) * 8.0;
	modelData->Head->Position.z -= this->OldGraze + (this->Graze - this->OldGraze) * t;
	mob->Type = MobTypeNone;
	MobRenderModel(sheep, textures, anim, t, run, rot, offset);
	mob->Type = MobTypeSheep;
	if (this->HasFur)
	{
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(textures, "Mob/SheepFur.png"));
		glDisable(GL_CULL_FACE);
		Model fur = ModelManagerGetModel(MobModelCache, "Sheep.Fur");
		AnimalModelData furData = fur->TypeData;
		furData->Head->Rotation = modelData->Head->Rotation;
		furData->Head->Position = modelData->Head->Position;
		furData->Body->Rotation = modelData->Body->Rotation;
		for (int i = 0; i < 4; i++) { furData->Legs[i]->Rotation = modelData->Legs[i]->Rotation; }
		ModelPartRender(furData->Head, offset);
		ModelPartRender(furData->Body, offset);
		for (int i = 0; i < 4; i++) { ModelPartRender(furData->Legs[i], offset); }
	}
	modelData->Head->Position = p;
}

void SheepDestroy(Sheep sheep)
{
	MobData mob = sheep->TypeData;
	SheepData this = mob->TypeData;
	MemoryFree(this);
}
