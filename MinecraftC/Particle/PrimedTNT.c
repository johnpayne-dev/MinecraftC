#include "PrimedTNT.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "../Utilities/SinTable.h"
#include "../Utilities/OpenGL.h"

PrimedTNT PrimedTNTCreate(Level level, float3 pos)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypePrimedTNT;
	entity->TypeData = MemoryAllocate(sizeof(struct PrimedTNTData));
	EntitySetSize(entity, 0.98, 0.98);
	entity->HeightOffset = entity->AABBHeight / 2.0;
	EntitySetPosition(entity, pos);
	entity->MakeStepSound = false;
	entity->OldPosition = pos;
	float r = RandomUniform() * 2.0 * pi;
	PrimedTNTData this = entity->TypeData;
	this->Delta = (float3){ -tsin(r * pi / 180.0) * 0.02, 0.2, -cos(r * pi / 180.0) * 0.02 };
	this->Life = 40;
	this->Defused = false;
	return entity;
}

void PrimedTNTHurt(PrimedTNT entity)
{
	if (!entity->Removed) { EntityRemove(entity); }
}

bool PrimedTNTIsPickable(PrimedTNT entity)
{
	return !entity->Removed;
}

void PrimedTNTTick(PrimedTNT entity)
{
	PrimedTNTData this = entity->TypeData;
	entity->OldPosition = entity->Position;
	this->Delta.y -= 0.04;
	EntityMove(entity, this->Delta);
	this->Delta *= 0.98;
	if (entity->OnGround) { this->Delta *= (float3){ 0.7, -0.5, 0.7 }; }
	
	if (!this->Defused)
	{
		this->Life--;
		if (this->Life > 0)
		{
			ParticleManagerSpawnParticle(entity->Level->ParticleEngine, SmokeParticleCreate(entity->Level, entity->Position + up3f * 0.6));
		}
		else
		{
			EntityRemove(entity);
			RandomGenerator random = RandomGeneratorCreate(time(NULL));
			float radius = 4.0;
			LevelExplode(entity->Level, NULL, entity->Position, radius);
			for (int i = 0; i < 100; i++)
			{
				float3 offset = (float3){ RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0) } * radius / 4.0;
				float3 vel = offset / dot3f(offset, offset);
				ParticleManagerSpawnParticle(entity->Level->ParticleEngine, TerrainParticleCreate(entity->Level, entity->Position + offset, vel, Blocks.Table[BlockTypeTNT]));
			}
			RandomGeneratorDestroy(random);
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
