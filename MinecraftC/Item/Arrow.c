#include <OpenGL.h>
#include "Arrow.h"
#include "TakeAnimation.h"
#include "../Mob/Mob.h"
#include "../Render/ShapeRenderer.h"
#include "../Player/Player.h"

Arrow ArrowCreate(Level level, Entity owner, float3 pos, float2 rot, float g)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypeArrow;
	entity->HeightOffset = 0.25;
	entity->Slide = false;
	entity->MakeStepSound = false;
	float2 a1 = { cos(-rot.x * rad - pi), sin(-rot.x * rad - pi) };
	float2 a2 = { cos(-rot.y * rad), sin(-rot.y * rad) };
	entity->OldPosition.xz += (float2){ -a1.x, a1.y } * 0.2;
	pos.xz += (float2){ -a1.x, a1.y } * 0.2;
	EntitySetPosition(entity, pos);
	EntitySetSize(entity, 0.3, 0.5);
	ArrowData arrow = MemoryAllocate(sizeof(struct ArrowData));
	*arrow = (struct ArrowData)
	{
		.HasHit = false,
		.StickTime = 0,
		.Time = 0,
		.Type = 0,
		.Gravity = 1.0 / g,
		.Owner = owner,
		.Damage = 3,
		.Delta = (float3){ a1.y * a2.x, a2.y, a1.x * a2.x } * g,
	};
	entity->TypeData = arrow;
	if (((MobData)owner->TypeData)->Type != MobTypePlayer) { arrow->Type = 1; }
	else { arrow->Damage = 7; }
	float d = length2f(arrow->Delta.xz);
	arrow->OldRotation = arrow->Rotation = (float2){ atan2(arrow->Delta.x, arrow->Delta.z), atan2(arrow->Delta.y, d) } * deg;
	return entity;
}

void ArrowTick(Arrow arrow)
{
	ArrowData this = arrow->TypeData;
	this->Time++;
	this->OldRotation = this->Rotation;
	arrow->OldPosition = arrow->Position;
	if (this->HasHit)
	{
		this->StickTime++;
		if (this->Type == 0 && this->StickTime >= 300 && RandomUniform() < 0.01)
		{
			EntityRemove(arrow);
			return;
		}
		else if (this->Type == 1 && this->StickTime >= 20) { EntityRemove(arrow); }
	}
	else
	{
		this->Delta *= 0.998;
		this->Delta.y -= 0.02 * this->Gravity;
		int r = length3f(this->Delta) / 0.2 + 1.0;
		float3 radius = this->Delta / r;
		for (int i = 0; i < r && !arrow->Collision; i++)
		{
			AABB aabb = AABBExpand(arrow->AABB, radius);
			list(AABB) cubes = LevelGetCubes(arrow->Level, aabb);
			if (ListCount(cubes) > 0) { arrow->Collision = true; }
			ListDestroy(cubes);
			
			list(Entity) entities = BlockMapGetEntitiesAABB(arrow->Level->BlockMap, arrow, aabb);
			for (int j = 0; j < ListCount(entities); j++)
			{
				if (EntityIsShootable(entities[j]) && (entities[j] != this->Owner || this->Time > 5))
				{
					EntityHurt(entities[j], arrow, this->Damage);
					arrow->Collision = true;
					EntityRemove(arrow);
					ListDestroy(entities);
					return;
				}
			}
			ListDestroy(entities);
			
			if (!arrow->Collision)
			{
				arrow->AABB = AABBMove(arrow->AABB, radius);
				arrow->Position += radius;
				BlockMapMoved(arrow->BlockMap, arrow);
			}
		}
		
		if (arrow->Collision)
		{
			this->HasHit = true;
			this->Delta = zero3f;
		}
		
		if (!this->HasHit)
		{
			float len = length2f(this->Delta.xz);
			this->Rotation.y = atan2(this->Delta.x, this->Delta.z) * deg;
			for (this->Rotation.x = atan2(this->Delta.y, len) * deg; this->Rotation.x - this->OldRotation.x < -180.0; this->OldRotation.x -= 360.0) {}
			while (this->Rotation.x - this->OldRotation.x >= 180.0) { this->OldRotation.x += 360.0; }
			while (this->Rotation.y - this->OldRotation.y < -180.0) { this->OldRotation.y -= 360.0; }
			while (this->Rotation.y - this->OldRotation.y >= 180.0) { this->OldRotation.y += 360.0; }
		}
	}
}

void ArrowRender(Arrow arrow, TextureManager textures, float t)
{
	ArrowData this = arrow->TypeData;
	arrow->TextureID = TextureManagerLoad(textures, "Item/Arrows.png");
	glBindTexture(GL_TEXTURE_2D, arrow->TextureID);
	float brightness = LevelGetBrightness(arrow->Level, arrow->Position.x, arrow->Position.y, arrow->Position.z);
	glPushMatrix();
	glColor4f(brightness, brightness, brightness, 1.0);
	float3 v = arrow->OldPosition + (arrow->Position - arrow->OldPosition) * t;
	glTranslatef(v.x, v.y - arrow->HeightOffset / 2.0, v.z);
	float2 a = this->OldRotation + (this->Rotation - this->OldRotation) * t;
	glRotatef(a.y - 90.0, 0.0, 1.0, 0.0);
	glRotatef(a.x, 0.0, 0.0, 1.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	float2 uv0 = ((float2){ 0, 5 } + this->Type * 10.0) / 32.0;
	float2 uv1 = ((float2){ 5, 10 } + this->Type * 10.0) / 32.0;
	glScalef(0.05625, 0.05625, 0.05625);
	glNormal3f(0.05625, 0.0, 0.0);
	ShapeRendererBegin();
	ShapeRendererVertexUV((float3){ -7.0, -2.0, -2.0 }, (float2){ 0.0, uv1.x });
	ShapeRendererVertexUV((float3){ -7.0, -2.0, 2.0 }, (float2){ 0.15625, uv1.x });
	ShapeRendererVertexUV((float3){ -7.0, 2.0, 2.0 }, (float2){ 0.15625, uv1.y });
	ShapeRendererVertexUV((float3){ -7.0, 2.0, -2.0 }, (float2){ 0.0, uv1.y });
	ShapeRendererEnd();
	glNormal3f(-0.05625, 0.0, 0.0);
	ShapeRendererBegin();
	ShapeRendererVertexUV((float3){ -7.0, 2.0, -2.0 }, (float2){ 0.0, uv1.x });
	ShapeRendererVertexUV((float3){ -7.0, 2.0, 2.0 }, (float2){ 0.15625, uv1.x });
	ShapeRendererVertexUV((float3){ -7.0, -2.0, 2.0 }, (float2){ 0.15625, uv1.y });
	ShapeRendererVertexUV((float3){ -7.0, -2.0, -2.0 }, (float2){ 0.0, uv1.y });
	ShapeRendererEnd();
	for (int i = 0; i < 4; i++)
	{
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glNormal3f(0.0, -0.05625, 0.0);
		ShapeRendererVertexUV((float3){ -8.0, -2.0, 0.0 }, (float2){ 0.0, uv0.x });
		ShapeRendererVertexUV((float3){ 8.0, -2.0, 0.0 }, (float2){ 0.5, uv0.x });
		ShapeRendererVertexUV((float3){ 8.0, 2.0, 0.0 }, (float2){ 0.5, uv0.y });
		ShapeRendererVertexUV((float3){ -8.0, 2.0, 0.0 }, (float2){ 0.0, uv0.y });
		ShapeRendererEnd();
	}
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPopMatrix();
}

void ArrowAwardKillScore(Arrow arrow, Entity entity, int score)
{
	EntityAwardKillScore(((ArrowData)arrow->TypeData)->Owner, entity, score);
}

Entity ArrowGetOwner(Arrow arrow)
{
	return ((ArrowData)arrow->TypeData)->Owner;
}

void ArrowPlayerTouch(Arrow arrow, Entity player)
{
	ArrowData this = arrow->TypeData;
	PlayerData playerData = (PlayerData)((MobData)player->TypeData)->TypeData;
	if (this->HasHit && this->Owner == player && playerData->Arrows < 99)
	{
		LevelAddEntity(arrow->Level, TakeAnimationCreate(arrow->Level, arrow, player));
		playerData->Arrows++;
		EntityRemove(arrow);
	}
}

void ArrowDestroy(Arrow arrow)
{
	MemoryFree(arrow->TypeData);
}
