#pragma once
#include "HeldBlock.h"

typedef struct Renderer {
	struct Minecraft * minecraft;
	float fogColorMultiplier;
	bool displayActive;
	float fogEnd;
	HeldBlock heldBlock;
	int levelTicks;
	Entity entity;
	RandomGenerator random;
	float buffer[16];
	float fogR, fogG, fogB;
} * Renderer;

Renderer RendererCreate(struct Minecraft * minecraft);
Vector3D RendererGetPlayerVector(Renderer renderer, float t);
void RendererApplyBobbing(Renderer renderer, float t);
void RendererSetLighting(Renderer renderer, bool lighting);
void RendererEnableGUIMode(Renderer renderer);
void RendererUpdateFog(Renderer renderer);
void RendererDestroy(Renderer renderer);
