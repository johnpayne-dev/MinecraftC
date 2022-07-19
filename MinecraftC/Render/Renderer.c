#include "Renderer.h"
#include "../Minecraft.h"
#include "../Utilities/SinTable.h"
#include "../Utilities/OpenGL.h"

void RendererCreate(Renderer * renderer, Minecraft * minecraft) {
	*renderer = (Renderer) {
		.minecraft = minecraft,
		.fogColorMultiplier = 1.0,
		.displayActive = false,
		.fogEnd = 0.0,
		.heldBlock = (HeldBlock){ .minecraft = minecraft },
		.entity = NULL,
	};
	RandomGeneratorCreate(&renderer->random, time(NULL));
}

Vector3D RendererGetPlayerVector(Renderer * renderer, float dt) {
	Player * entity = &renderer->minecraft->player;
	return (Vector3D){ entity->xo + (entity->x - entity->xo) * dt, entity->yo + (entity->y - entity->yo) * dt, entity->zo + (entity->z - entity->zo) * dt };
}

void RendererApplyBobbing(Renderer * renderer, float dt) {
	Player * entity = &renderer->minecraft->player;
	PlayerData * player = &entity->player;
	float walk = entity->walkDistance - entity->oldWalkDistance;
	walk = entity->walkDistance + walk * dt;
	float bob = player->oldBobbing + (player->bobbing - player->oldBobbing) * dt;
	float tilt = player->oldTilt + (player->tilt - player->oldTilt) * dt;
	glTranslatef(tsin(walk * M_PI) * bob * 0.5, -fabs(tcos(walk * M_PI) * bob), 0.0);
	glRotatef(tsin(walk * M_PI) * bob * 3.0, 0.0, 0.0, 1.0);
	glRotatef(fabs(tcos(walk * M_PI + 0.2) * bob) * 5.0, 1.0, 0.0, 0.0);
	glRotatef(tilt, 1.0, 0.0, 0.0);
}

void RendererSetLighting(Renderer * renderer, bool lighting) {
	if (!lighting) {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	} else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		Vector3D pos = Vector3DNormalize((Vector3D){ 0.0, -1.0, 0.5 });
		glLightfv(GL_LIGHT0, GL_POSITION, (float[]){ pos.x, pos.y, pos.z, 0.0 });
		glLightfv(GL_LIGHT0, GL_DIFFUSE, (float[]){ 0.3, 0.3, 0.3, 1.0 });
		glLightfv(GL_LIGHT0, GL_AMBIENT, (float[]){ 0.0, 0.0, 0.0, 1.0 });
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float[]){ 0.7, 0.7, 0.7, 1.0 });
	}
}

void RendererEnableGUIMode(Renderer * renderer) {
	int w = renderer->minecraft->width * 240 / renderer->minecraft->height;
	int h = renderer->minecraft->height * 240 / renderer->minecraft->height;
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, h, 0.0, 100.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -200.0);
}

void RendererUpdateFog(Renderer * renderer) {
	Level * level = renderer->minecraft->level;
	Player * player = &renderer->minecraft->player;
	glFogfv(GL_FOG_COLOR, (float []){ renderer->fogR, renderer->fogG, renderer->fogB, 1.0 });
	glNormal3f(0.0, -1.0, 0.0);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	Block * block = &Blocks.table[LevelGetTile(level, player->x, player->y + 0.12, player->z)];
	if (block->type != BlockTypeNone && BlockGetLiquidType(block) != LiquidTypeNone) {
		LiquidType liquid = BlockGetLiquidType(block);
		glFogi(GL_FOG_MODE, GL_EXP);
		if (liquid == LiquidTypeWater) {
			glFogf(GL_FOG_DENSITY, 0.1);
			Vector3D a = { 0.4, 0.4, 0.9 };
			if (renderer->minecraft->settings.anaglyph) {
				a = (Vector3D){ (a.x * 30.0 + a.y * 59.0 + a.z * 11.0) / 100.0, (a.x * 30.0 + a.y * 70.0) / 100.0, (a.x * 30.0 + a.z * 70.0) / 100.0 };
			}
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float []){ a.x, a.y, a.z, 1.0 });
		} else if (liquid == LiquidTypeLava) {
			glFogf(GL_FOG_DENSITY, 2.0);
			Vector3D a = { 0.4, 0.3, 0.3 };
			if (renderer->minecraft->settings.anaglyph) {
				a = (Vector3D){ (a.x * 30.0 + a.y * 59.0 + a.z * 11.0) / 100.0, (a.x * 30.0 + a.y * 70.0) / 100.0, (a.x * 30.0 + a.z * 70.0) / 100.0 };
			}
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float []){ a.x, a.y, a.z, 1.0 });
		}
	} else {
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, 0.0);
		glFogf(GL_FOG_END, renderer->fogEnd);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float []){ 1.0, 1.0, 1.0, 1.0 });
	}
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
}
