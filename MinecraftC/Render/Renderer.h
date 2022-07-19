#pragma once
#include "HeldBlock.h"
#include "../Entity.h"

typedef struct Renderer {
	struct Minecraft * minecraft;
	float fogColorMultiplier;
	bool displayActive;
	float fogEnd;
	HeldBlock heldBlock;
	Entity * entity;
	RandomGenerator random;
	float fogR, fogG, fogB;
} Renderer;

void RendererCreate(Renderer * renderer, struct Minecraft * minecraft);
Vector3D RendererGetPlayerVector(Renderer * renderer, float dt);
void RendererApplyBobbing(Renderer * renderer, float dt);
void RendererSetLighting(Renderer * renderer, bool lighting);
void RendererEnableGUIMode(Renderer * renderer);
void RendererUpdateFog(Renderer * renderer);
