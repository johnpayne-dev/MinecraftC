#pragma once
#include "HeldBlock.h"

typedef struct Renderer
{
	struct Minecraft * Minecraft;
	float FogColorMultiplier;
	bool DisplayActive;
	float FogEnd;
	HeldBlock HeldBlock;
	int LevelTicks;
	Entity Entity;
	RandomGenerator Random;
	float Buffer[16];
	float4 FogColor;
} * Renderer;

Renderer RendererCreate(struct Minecraft * minecraft);
float3 RendererGetPlayerVector(Renderer renderer, float t);
void RendererHurtEffect(Renderer renderer, float t);
void RendererApplyBobbing(Renderer renderer, float t);
void RendererSetLighting(Renderer renderer, bool lighting);
void RendererEnableGUIMode(Renderer renderer);
void RendererUpdateFog(Renderer renderer);
void RendererDestroy(Renderer renderer);
