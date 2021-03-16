#include <OpenGL.h>
#include "Renderer.h"
#include "../Minecraft.h"

Renderer RendererCreate(Minecraft minecraft)
{
	Renderer renderer = MemoryAllocate(sizeof(struct Renderer));
	*renderer = (struct Renderer)
	{
		.Minecraft = minecraft,
		.FogColorMultiplier = 1.0,
		.DisplayActive = false,
		.FogEnd = 0.0,
		.HeldBlock = (HeldBlock){ .Minecraft = minecraft },
		.Entity = NULL,
		.Random = RandomGeneratorCreate(time(NULL)),
	};
	return renderer;
}

float3 RendererGetPlayerVector(Renderer renderer, float t)
{
	Player entity = renderer->Minecraft->Player;
	return entity->OldPosition + (entity->Position - entity->Position) * t;
}

void RendererHurtEffect(Renderer renderer, float t)
{
	Player entity = renderer->Minecraft->Player;
	MobData playerMob = entity->TypeData;
	float hurt = playerMob->HurtTime - t;
	if (playerMob->Health <= 0)
	{
		t += playerMob->DeathTime;
		glRotatef(40.0 - 8000.0 / (t + 200.0), 0.0, 0.0, 1.0);
	}
	
	if (hurt >= 0.0)
	{
		hurt /= playerMob->HurtDuration;
		hurt = sin(hurt * hurt * hurt * hurt * pi);
		glRotatef(-playerMob->HurtDirection, 0.0, 1.0, 0.0);
		glRotatef(-hurt * 14.0, 0.0, 0.0, 1.0);
		glRotatef(playerMob->HurtDirection, 0.0, 1.0, 0.0);
	}
}

void RendererApplyBobbing(Renderer renderer, float t)
{
	Player entity = renderer->Minecraft->Player;
	MobData playerMob = entity->TypeData;
	PlayerData player = playerMob->TypeData;
	float walk = entity->WalkDistance - entity->OldWalkDistance;
	walk = entity->WalkDistance + walk * t;
	float bob = player->OldBobbing + (player->Bobbing - player->OldBobbing) * t;
	float tilt = playerMob->OldTilt + (playerMob->Tilt - playerMob->OldTilt) * t;
	glTranslatef(sin(walk * pi) * bob * 0.5, -fabs(cos(walk * pi) * bob), 0.0);
	glRotatef(sin(walk * pi) * bob * 3.0, 0.0, 0.0, 1.0);
	glRotatef(fabs(cos(walk * pi + 0.2) * bob) * 5.0, 1.0, 0.0, 0.0);
	glRotatef(tilt, 1.0, 0.0, 0.0);
}

void RendererSetLighting(Renderer renderer, bool lighting)
{
	if (!lighting)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		float3 pos = normalize3f((float3){ 0.0, -1.0, 0.5 });
		glLightfv(GL_LIGHT0, GL_POSITION, (float[]){ pos.x, pos.y, pos.z, 0.0 });
		glLightfv(GL_LIGHT0, GL_DIFFUSE, (float[]){ 0.3, 0.3, 0.3, 1.0 });
		glLightfv(GL_LIGHT0, GL_AMBIENT, (float[]){ 0.0, 0.0, 0.0, 1.0 });
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float[]){ 0.7, 0.7, 0.7, 1.0 });
	}
}

void RendererEnableGUIMode(Renderer renderer)
{
	int w = renderer->Minecraft->Width * 240 / renderer->Minecraft->Height;
	int h = renderer->Minecraft->Height * 240 / renderer->Minecraft->Height;
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, h, 0.0, 100.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -200.0);
}

void RendererUpdateFog(Renderer renderer)
{
	Level level = renderer->Minecraft->Level;
	Player player = renderer->Minecraft->Player;
	glFogfv(GL_FOG_COLOR, (float *)&renderer->FogColor);
	glNormal3f(0.0, -1.0, 0.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	Block block = Blocks.Table[LevelGetTile(level, player->Position.x, player->Position.y + 0.12, player->Position.z)];
	if (block != NULL && BlockGetLiquidType(block) != LiquidTypeNone)
	{
		LiquidType liquid = BlockGetLiquidType(block);
		glFogi(GL_FOG_MODE, GL_EXP);
		if (liquid == LiquidTypeWater)
		{
			glFogf(GL_FOG_DENSITY, 0.1);
			float4 a = { 0.4, 0.4, 0.9, 1.0 };
			if (renderer->Minecraft->Settings->Anaglyph)
			{
				a.xyz = (float3){ a.x * 30.0 + a.y * 59.0 + a.z * 11.0, a.x * 30.0 + a.y * 70.0, a.x * 30.0 + a.z * 70.0 } / 100.0;
			}
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&a);
		}
		else if (liquid == LiquidTypeLava)
		{
			glFogf(GL_FOG_DENSITY, 2.0);
			float4 a = { 0.4, 0.3, 0.3, 1.0 };
			if (renderer->Minecraft->Settings->Anaglyph)
			{
				a.xyz = (float3){ a.x * 30.0 + a.y * 59.0 + a.z * 11.0, a.x * 30.0 + a.y * 70.0, a.x * 30.0 + a.z * 70.0 } / 100.0;
			}
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&a);
		}
	}
	else
	{
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 0.0);
		glFogf(GL_FOG_END, renderer->FogEnd);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&one4f);
	}
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
}

void RendererDestroy(Renderer renderer)
{
	RandomGeneratorDestroy(renderer->Random);
	MemoryFree(renderer);
}
