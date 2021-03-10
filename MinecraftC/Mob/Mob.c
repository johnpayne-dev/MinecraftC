#include <OpenGL.h>
#include "Mob.h"
#include "Humanoid.h"
#include "Pig.h"
#include "Sheep.h"
#include "AI/BasicAI.h"
#include "../Player/Player.h"

ModelManager MobModelCache = NULL;

Mob MobCreate(Level level)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypeMob;
	entity->FootSize = 0.5;
	EntitySetPosition(entity, entity->Position);
	MobData mob = MemoryAllocate(sizeof(struct MobData));
	*mob = (struct MobData)
	{
		.InvulnerableDuration = 20,
		.RotationA = (RandomUniform() + 1.0) * 0.01,
		.BodyRotation = 0.0,
		.OldBodyRotation = 0.0,
		.TickCount = 0,
		.HasHair = true,
		.TextureName = "Char.png",
		.AllowAlpha = true,
		.RotationOffset = 0.0,
		.ModelName = NULL,
		.BobbingStrength = 1.0,
		.DeathScore = 0,
		.RenderOffset = 0.0,
		.Health = 20,
		.InvulnerableTime = 0,
		.AirSupply = 300,
		.HurtDirection = 0.0,
		.DeathTime = 0,
		.AttackTime = 0,
		.Dead = false,
		.TimeOffset = RandomUniform() * 12398.0,
		.Rotation = 2.0 * pi * RandomUniform(),
		.Speed = 1.0,
		.AI = BasicAICreate(),
	};
	entity->TypeData = mob;
	return entity;
}

bool MobIsPickable(Mob mob)
{
	return !mob->Removed;
}

bool MobIsPushable(Mob mob)
{
	return !mob->Removed;
}

void MobTick(Mob mob)
{
	MobData this = mob->TypeData;
	this->OldTilt = this->Tilt;
	if (this->AttackTime > 0) { this->AttackTime--; }
	if (this->HurtTime > 0) { this->HurtTime--; }
	if (this->InvulnerableTime > 0) { this->InvulnerableTime--; }
	if (this->Health <= 0)
	{
		this->DeathTime++;
		if (this->DeathTime > 20)
		{
			if (this->AI != NULL) { AIBeforeRemove(this->AI); }
			EntityRemove(mob);
		}
	}
	if (EntityIsUnderWater(mob))
	{
		if (this->AirSupply > 0) { this->AirSupply--; }
		else { EntityHurt(mob, NULL, 2); }
	}
	else { this->AirSupply = 300; }
	if (EntityIsInWater(mob)) { mob->FallDistance = 0.0; }
	if (EntityIsInLava(mob)) { EntityHurt(mob, NULL, 10); }
	
	this->OldAnimationStep = this->AnimationStep;
	this->OldBodyRotation = this->BodyRotation;
	mob->OldRotation = mob->Rotation;
	this->TickCount++;
	MobStepAI(mob);
	this->OldRun = this->Run;
	float2 d = mob->Position.xz - mob->OldPosition.xz;
	float len = length2f(d);
	float rot = this->BodyRotation;
	float f1 = 0.0, f2 = 0.0;
	if (len > 0.05)
	{
		f2 = 1.0;
		f1 = len * 3.0;
		rot = atan2(d.y, d.x) * deg - 90.0;
	}
	if (!mob->OnGround) { f2 = 0.0; }
	this->Run += (f2 - this->Run) * 0.3;
	
	float a;
	for (a = rot - this->BodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	this->BodyRotation += a * 0.1;
	for (a = mob->Rotation.y - this->BodyRotation; a < -180.0; a += 360.0);
	while (a >= 180.0) { a -= 360.0; }
	bool b = a < -90.0 || a >= 90.0;
	if (a < -75.0) { a = -75.0; }
	if (a > 75.0) { a = 75.0; }
	this->BodyRotation = mob->Rotation.y - a;
	this->BodyRotation += a * 0.1;
	if (b) { f1 = -f1; }
	while (mob->Rotation.y - mob->OldRotation.y < -180.0) { mob->OldRotation.y -= 360.0; }
	while (mob->Rotation.y - mob->OldRotation.y >= 180.0) { mob->OldRotation.y += 360.0; }
	while (this->BodyRotation - this->OldBodyRotation < -180.0) { this->OldBodyRotation -= 360.0; }
	while (this->BodyRotation - this->OldBodyRotation >= 180.0) { this->OldBodyRotation += 360.0; }
	while (mob->Rotation.x - mob->OldRotation.x < -180.0) { mob->OldRotation.x -= 360.0; }
	while (mob->Rotation.x - mob->OldRotation.x >= 180.0) { mob->OldRotation.x += 360.0; }
	
	this->AnimationStep += f1;
}

void MobStepAI(Mob mob)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypePlayer) { return PlayerStepAI(mob); }
	if (this->AI != NULL) { AITick(this->AI, mob->Level, mob); }
	if (this->Type == MobTypeSheep) { SheepStepAI(mob); }
}

void MobBindTexture(Mob mob, TextureManager textures)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypePlayer) { return PlayerBindTexture(mob, textures); }
	mob->TextureID = TextureManagerLoad(textures, (char *)this->TextureName);
	glBindTexture(GL_TEXTURE_2D, mob->TextureID);
}

void MobRender(Mob mob, TextureManager textures, float t)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypePlayer) { return PlayerRender(mob, textures, t); }
	if (this->ModelName == NULL) { return; }
	
	float attack = this->AttackTime - t;
	if (attack < 0.0) { attack = 0.0; }
	while (this->BodyRotation - this->OldBodyRotation < -180.0) { this->OldBodyRotation -= 360.0; }
	while (this->BodyRotation - this->OldBodyRotation >= 180.0) { this->OldBodyRotation += 360.0; }
	while (mob->Rotation.x - mob->OldRotation.x < -180.0) { mob->OldRotation.x -= 360.0; }
	while (mob->Rotation.x - mob->OldRotation.x >= 180.0) { mob->OldRotation.x += 360.0; }
	while (mob->Rotation.y - mob->OldRotation.y < -180.0) { mob->OldRotation.y -= 360.0; }
	while (mob->Rotation.y - mob->OldRotation.y >= 180.0) { mob->OldRotation.y += 360.0; }
	float brot = this->OldBodyRotation + (this->BodyRotation - this->OldBodyRotation) * t;
	float run = this->OldRun + (this->Run - this->OldRun) * t;
	float2 rot = mob->OldRotation + (mob->Rotation - mob->OldRotation) * t;
	rot.y -= brot;
	glPushMatrix();
	float anim = this->OldAnimationStep + (this->AnimationStep - this->OldAnimationStep) * t;
	float brightness = EntityGetBrightness(mob, t);
	glColor3f(brightness, brightness, brightness);
	float offs = -fabs(cos(anim * 0.662)) * 5.0 * run * this->BobbingStrength - 23.0;
	float3 pos = mob->OldPosition + (mob->Position - mob->OldPosition) * t;
	glTranslatef(pos.x, pos.y - 1.62 + this->RenderOffset, pos.z);
	float hurt = this->HurtTime - t;
	if (hurt > 0.0 || this->Health <= 0)
	{
		hurt = hurt < 0.0 ? 0.0 : sin(pow(hurt / this->HurtDuration, 4.0) * pi) * 14.0;
		if (this->Health <= 0)
		{
			float h = (this->DeathTime + t) / 20.0;
			hurt += h * h * 800.0;
			if (hurt > 90.0) { hurt = 90.0; }
		}
		
		float hurtDir = this->HurtDirection;
		glRotatef(180.0 - brot + this->RotationOffset, 0.0, 1.0, 0.0);
		glRotatef(-hurtDir, 0.0, 1.0, 0.0);
		glRotatef(-hurt, 0.0, 0.0, 0.0);
		glRotatef(-(180.0 - brot + this->RotationOffset), 0.0, 1.0, 0.0);
	}
	
	glTranslatef(0.0, -offs * 0.0625, 0.0);
	glScalef(1.0, -1.0, 1.0);
	glRotatef(180.0 - brot + this->RotationOffset, 0.0, 1.0, 0.0);
	if (!this->AllowAlpha) { glDisable(GL_ALPHA_TEST); }
	else { glDisable(GL_CULL_FACE); }
	
	glScalef(-1.0, 1.0, 1.0);
	ModelManagerGetModel(MobModelCache, this->ModelName)->AttackOffset = attack / 3.0;
	MobBindTexture(mob, textures);
	MobRenderModel(mob, textures, anim, t, run, rot, 0.0625);
	if (this->InvulnerableTime > this->InvulnerableDuration - 10)
	{
		glColor4f(1.0, 1.0, 1.0, 0.75);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		MobBindTexture(mob, textures);
		MobRenderModel(mob, textures, anim, t, run, rot, 0.0625);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	glEnable(GL_ALPHA_TEST);
	if (this->AllowAlpha) { glEnable(GL_CULL_FACE); }
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPopMatrix();
}

void MobRenderModel(Mob mob, TextureManager textures, float anim, float t, float run, float2 rot, float offset)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypeSheep) { return SheepRenderModel(mob, textures, anim, t, run, rot, offset); }
	ModelRender(ModelManagerGetModel(MobModelCache, this->ModelName), anim, run, this->TickCount + t, rot, offset);
	if (this->Type == MobTypeHumanoid || this->Type == MobTypeSkeleton || this->Type == MobTypeZombie) { HumanoidRenderModel(mob, textures, anim, t, run, rot, offset); }
}

void MobHeal(Mob mob, int health)
{
	MobData this = mob->TypeData;
	if (this->Health > 0)
	{
		this->Health += health;
		if (this->Health > 20) { this->Health = 20; }
		this->InvulnerableTime = this->InvulnerableDuration / 2;
	}
}

void MobHurt(Mob mob, Entity entity, int damage)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypeSheep && ((SheepData)this->TypeData)->HasFur && entity != NULL && ((MobData)entity->TypeData)->Type == MobTypePlayer) { return SheepHurt(mob, entity, damage); }
	if (this->Type == MobTypePlayer) { return PlayerHurt(mob, entity, damage); }
	
	if (mob->Level->CreativeMode) { return; }
	if (this->Health <= 0) { return; }
	
	AIHurt(this->AI, entity, damage);
	if (this->InvulnerableTime > this->InvulnerableDuration / 2.0)
	{
		if (this->LastHealth - damage >= this->Health) { return; }
		this->Health = this->LastHealth - damage;
	}
	else
	{
		this->LastHealth = this->Health;
		this->InvulnerableTime = this->InvulnerableDuration;
		this->Health -= damage;
		this->HurtTime = this->HurtDuration = 10;
	}
	
	this->HurtDirection = 0.0;
	if (entity != NULL)
	{
		float2 d = entity->Position.xz - mob->Position.xz;
		this->HurtDirection = atan2(d.y, d.x) * deg - mob->Rotation.y;
		MobKnockback(mob, entity, damage, d.x, d.y);
	}
	else { this->HurtDirection = 180.0 * (int)(RandomUniform() * 2.0); }
	
	if (this->Health <= 0) { MobDie(mob, entity); }
}

void MobKnockback(Mob mob, Entity entity, int damage, int x, int z)
{
	float len = length2f((float2){ x, z });
	mob->Delta /= 2.0;
	mob->Delta.xz -= (float2){ x, z } / len * 0.4;
	mob->Delta.y += 0.4;
	if (mob->Delta.y > 0.4) { mob->Delta.y = 0.4; }
}

void MobDie(Mob mob, Entity entity)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypePig) { PigDie(mob, entity); }
	if (this->Type == MobTypeSheep) { SheepDie(mob, entity); }
	if (this->Type == MobTypePlayer) { return PlayerDie(mob, entity); }
	
	if (!mob->Level->CreativeMode)
	{
		if (this->DeathScore > 0 && entity != NULL) { EntityAwardKillScore(entity, mob, this->DeathScore); }
		this->Dead = true;
	}
}

void MobCauseFallDamage(Mob mob, float height)
{
	if (!mob->Level->CreativeMode)
	{
		int damage = ceil(height - 3.0);
		if (damage > 0) { EntityHurt(mob, NULL, damage); }
	}
}

void MobTravel(Mob mob, float x, float y)
{
	if (EntityIsInWater(mob))
	{
		float z = mob->Position.y;
		EntityMoveRelative(mob, (float2){ x, y }, 0.02);
		EntityMove(mob, mob->Delta);
		mob->Delta *= 0.8;
		mob->Delta.y -= 0.02;
		if (mob->HorizontalCollision && EntityIsFree(mob, mob->Delta + up3f * (0.6 - mob->Position.y + z))) { mob->Delta.y = 0.3; }
	}
	else if (EntityIsInLava(mob))
	{
		float z = mob->Position.y;
		EntityMoveRelative(mob, (float2){ x, y }, 0.02);
		EntityMove(mob, mob->Delta);
		mob->Delta *= 0.5;
		mob->Delta.y -= 0.02;
		if (mob->HorizontalCollision && EntityIsFree(mob, mob->Delta + up3f * (0.6 - mob->Position.y + z))) { mob->Delta.y = 0.3; }
	}
	else
	{
		EntityMoveRelative(mob, (float2){ x, y }, mob->OnGround ? 0.1 : 0.02);
		EntityMove(mob, mob->Delta);
		mob->Delta *= (float3){ 0.91, 0.98, 0.91 };
		//mob->Delta.x += 0.01;
		mob->Delta.y -= 0.08;
		if (mob->OnGround) { mob->Delta.xz *= 0.6; }
	}
}

bool MobIsShootable(Mob mob)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypePlayer) { return PlayerIsShootable(mob); }
	return true;
}

void MobDestroy(Mob mob)
{
	MobData this = mob->TypeData;
	if (this->Type == MobTypeHumanoid || this->Type == MobTypeSkeleton || this->Type == MobTypeZombie) { HumanoidDestroy(mob); }
	if (this->Type == MobTypeSheep) { SheepDestroy(mob); }
	if (this->Type == MobTypePlayer) { PlayerDestroy(mob); }
	if (this->AI != NULL) { AIDestroy(this->AI); }
	MemoryFree(this);
}
