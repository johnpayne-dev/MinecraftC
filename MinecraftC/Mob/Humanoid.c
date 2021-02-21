#include <OpenGL.h>
#include "Humanoid.h"

Humanoid HumanoidCreate(Level level, float3 pos)
{
	Humanoid entity = MobCreate(level);
	MobData mob = entity->TypeData;
	HumanoidData humanoid = MemoryAllocate(sizeof(struct HumanoidData));
	mob->Type = MobTypeHumanoid;
	mob->TypeData = humanoid;
	humanoid->Armor = RandomUniform() < 0.2;
	humanoid->Helmet = RandomUniform() < 0.2;
	mob->ModelName = "Humanoid";
	EntitySetPosition(entity, pos);
	return entity;
}

void HumanoidRenderModel(Humanoid humanoid, TextureManager textures, float anim, float t, float run, float2 rot, float offset)
{
	MobData mob = humanoid->TypeData;
	HumanoidData this = mob->TypeData;
	
	HumanoidModel model = ModelManagerGetModel(MobModelCache, mob->ModelName);
	HumanoidModelData modelData = model->TypeData;
	glEnable(GL_ALPHA_TEST);
	if (mob->AllowAlpha) { glEnable(GL_CULL_FACE); }
	
	if (mob->HasHair)
	{
		glDisable(GL_CULL_FACE);
		modelData->HeadWear->Rotation.xy = modelData->Head->Rotation.xy;
		ModelPartRender(modelData->HeadWear, offset);
		glEnable(GL_CULL_FACE);
	}
	
	if (this->Armor || this->Helmet)
	{
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(textures, "Armor/Plate.png"));
		glDisable(GL_CULL_FACE);
		HumanoidModel armor = ModelManagerGetModel(MobModelCache, "Humanoid.Armor");
		HumanoidModelData armorData = armor->TypeData;
		armorData->Head->Render = this->Helmet;
		armorData->Body->Render = this->Armor;
		armorData->RightArm->Render = this->Armor;
		armorData->LeftArm->Render = this->Armor;
		armorData->RightLeg->Render = false;
		armorData->LeftLeg->Render = false;
		armorData->Head->Rotation = modelData->Head->Rotation;
		armorData->RightArm->Rotation = modelData->RightArm->Rotation;
		armorData->RightLeg->Rotation = modelData->RightLeg->Rotation;
		armorData->LeftArm->Rotation = modelData->LeftArm->Rotation;
		armorData->LeftLeg->Rotation = modelData->LeftLeg->Rotation;
		ModelPartRender(armorData->Head, offset);
		ModelPartRender(armorData->Body, offset);
		ModelPartRender(armorData->RightArm, offset);
		ModelPartRender(armorData->RightLeg, offset);
		ModelPartRender(armorData->LeftArm, offset);
		ModelPartRender(armorData->LeftLeg, offset);
		glEnable(GL_CULL_FACE);
	}
	glDisable(GL_ALPHA_TEST);
}

void HumanoidDestroy(Humanoid humanoid)
{
	MobData mob = humanoid->TypeData;
	MemoryFree(mob->TypeData);
}
