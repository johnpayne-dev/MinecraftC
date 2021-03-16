#include <OpenGL.h>
#include "PrimedTNT.h"
#include "Item.h"
#include "TakeAnimation.h"
#include "../Mob/Mob.h"
#include "../Particle/SmokeParticle.h"
#include "../Particle/TerrainParticle.h"
#include "../Player/Player.h"

PrimedTNT PrimedTNTCreate(Level level, float3 pos)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypePrimedTNT;
	EntitySetSize(entity, 0.98, 0.98);
	entity->HeightOffset = entity->AABBHeight / 2.0;
	EntitySetPosition(entity, pos);
	entity->MakeStepSound = false;
	entity->OldPosition = pos;
	PrimedTNTData tnt = MemoryAllocate(sizeof(struct PrimedTNTData));
	*tnt = (struct PrimedTNTData)
	{
		.Life = 40,
	};
	entity->TypeData = tnt;
	float a = 2.0 * pi * RandomUniform();
	tnt->Delta.xz = (float2){ -sin(a * pi / 180.0), cos(a * pi / 180.0) } * 0.02;
	tnt->Delta.y = 0.2;
	return entity;
}

void PrimedTNTHurt(PrimedTNT tnt, Entity entity, int damage)
{
	if (!tnt->Removed)
	{
		tnt->Type = EntityTypeNone;
		EntityHurt(tnt, entity, damage);
		tnt->Type = EntityTypePrimedTNT;
		if (entity != NULL && entity->Type == EntityTypeMob && ((MobData)entity->TypeData)->Type == MobTypePlayer)
		{
			EntityRemove(tnt);
			LevelAddEntity(tnt->Level, ItemCreate(tnt->Level, tnt->Position, BlockTypeTNT));
		}
	}
}

bool PrimedTNTIsPickable(PrimedTNT tnt)
{
	return !tnt->Removed;
}

void PrimedTNTTick(PrimedTNT tnt)
{
	PrimedTNTData this = tnt->TypeData;
	tnt->OldPosition = tnt->Position;
	this->Delta.y -= 0.04;
	EntityMove(tnt, this->Delta);
	this->Delta *= 0.98;
	if (tnt->OnGround) { this->Delta *= (float3){ 0.7, -0.5, 0.7 }; }
	
	if (!this->Defused)
	{
		if (this->Life-- > 0) { ParticleManagerSpawnParticle(tnt->Level->ParticleEngine, SmokeParticleCreate(tnt->Level, tnt->Position + up3f * 0.6)); }
		else
		{
			EntityRemove(tnt);
			RandomGenerator random = RandomGeneratorCreate(time(NULL));
			float radius = 4.0;
			LevelExplode(tnt->Level, NULL, tnt->Position, radius);
			
			for (int i = 0; i < 100; i++)
			{
				float3 pos = (float3){ RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0) } * radius / 4.0;
				float3 vel = pos / dot3f(pos, pos);
				ParticleManagerSpawnParticle(tnt->Level->ParticleEngine, TerrainParticleCreate(tnt->Level, tnt->Position + pos, vel, Blocks.Table[BlockTypeTNT]));
			}
		}
	}
}

void PrimedTNTPlayerTouch(PrimedTNT tnt, Entity player)
{
	PrimedTNTData this = tnt->TypeData;
	if (this->Defused)
	{
		if (PlayerAddResource(player, BlockTypeTNT))
		{
			LevelAddEntity(tnt->Level, TakeAnimationCreate(tnt->Level, tnt, player));
			EntityRemove(tnt);
		}
	}
}

void PrimedTNTRender(PrimedTNT tnt, TextureManager textures, float t)
{
	PrimedTNTData this = tnt->TypeData;
	int texture = TextureManagerLoad(textures, "Terrain.png");
	glBindTexture(GL_TEXTURE_2D, texture);
	float brightness = LevelGetBrightness(tnt->Level, tnt->Position.x, tnt->Position.y, tnt->Position.z);
	glPushMatrix();
	glColor4f(brightness, brightness, brightness, 1.0);
	float3 v = tnt->OldPosition + (tnt->Position - tnt->OldPosition) * t - 0.5;
	glTranslatef(v.x, v.y, v.z);
	glPushMatrix();
	BlockRenderPreview(Blocks.Table[BlockTypeTNT]);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, ((this->Life / 4 + 1) % 2) * 0.4);
	if (this->Life <= 16) { glColor4f(1.0, 1.0, 1.0, ((this->Life + 1) % 2) * 0.6); }
	if (this->Life <= 2) { glColor4f(1.0, 1.0, 1.0, 0.9); }
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	BlockRenderPreview(Blocks.Table[BlockTypeTNT]);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPopMatrix();
}

void PrimedTNTDestroy(PrimedTNT tnt)
{
	MemoryFree(tnt->TypeData);
}
