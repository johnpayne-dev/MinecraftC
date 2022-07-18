#pragma once
#include <SDL2/SDL.h>
#include "Render/LevelRenderer.h"
#include "Particle/ParticleManager.h"
#include "GUI/GUIScreen.h"
#include "GUI/HUDScreen.h"
#include "Render/Renderer.h"
#include "Level/LevelIO.h"
#include "Timer.h"
#include "SessionData.h"
#include "ProgressBarDisplay.h"

typedef void * MinecraftApplet;

typedef struct Minecraft {
	bool fullScreen;
	int width, frameWidth;
	int height, frameHeight;
	Timer timer;
	Level level;
	LevelRenderer levelRenderer;
	Player player;
	ParticleManager particleManager;
	SessionData session;
	SDL_Window * window;
	SDL_GLContext context;
	bool levelLoaded;
	TextureManager textureManager;
	FontRenderer font;
	GUIScreen currentScreen;
	ProgressBarDisplay progressBar;
	Renderer renderer;
	LevelIO levelIO;
	//SoundManager SoundManager;
	//ResourceDownloadThread ResourceThread;
	int ticks;
	//Robot Robot;
	HUDScreen hud;
	//NetworkManager NetworkManager;
	//SoundPlayer SoundPlayer;
	MovingObjectPosition selected;
	GameSettings settings;
	bool running;
	String debug;
	bool hasMouse;
	int lastClick;
	bool raining;
	char * workingDirectory;
} * Minecraft;

Minecraft MinecraftCreate(MinecraftApplet applet, int width, int height, bool fullScreen);
void MinecraftSetCurrentScreen(Minecraft minecraft, GUIScreen screen);
void MinecraftShutdown(Minecraft minecraft);
void MinecraftRun(Minecraft minecraft);
void MinecraftGrabMouse(Minecraft minecraft);
void MinecraftPause(Minecraft minecraft);
void MinecraftGenerateLevel(Minecraft minecraft, int size);
void MinecraftSetLevel(Minecraft minecraft, Level level);
void MinecraftDestroy(Minecraft minecraft);
