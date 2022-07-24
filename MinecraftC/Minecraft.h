#pragma once
#include <SDL.h>
#include "Render/LevelRenderer.h"
#include "Particle/ParticleManager.h"
#include "GUI/GUIScreen.h"
#include "GUI/HUDScreen.h"
#include "Render/Renderer.h"
#include "Timer.h"
#include "SessionData.h"
#include "ProgressBarDisplay.h"

typedef struct Minecraft {
	bool fullScreen;
	int width, frameWidth;
	int height, frameHeight;
	Timer timer;
	Level level;
	LevelRenderer levelRenderer;
	Player player;
	ParticleManager particleManager;
	SDL_Window * window;
	SDL_GLContext context;
	TextureManager textureManager;
	FontRenderer font;
	GUIScreen * currentScreen;
	ProgressBarDisplay progressBar;
	Renderer renderer;
	int ticks;
	HUDScreen hud;
	MovingObjectPosition selected;
	GameSettings settings;
	bool running;
	String debug;
	bool hasMouse;
	int lastClick;
	bool raining;
	char * workingDirectory;
} Minecraft;

void MinecraftCreate(Minecraft * minecraft, int width, int height, bool fullScreen);
void MinecraftSetCurrentScreen(Minecraft * minecraft, GUIScreen * screen);
void MinecraftRun(Minecraft * minecraft);
void MinecraftGrabMouse(Minecraft * minecraft);
void MinecraftRegenerateLevel(Minecraft * minecraft, int size);
void MinecraftPause(Minecraft * minecraft);
void MinecraftDestroy(Minecraft * minecraft);
