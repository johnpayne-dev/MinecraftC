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

typedef struct Minecraft
{
	bool FullScreen;
	int Width, FrameWidth;
	int Height, FrameHeight;
	Timer Timer;
	Level Level;
	LevelRenderer LevelRenderer;
	Player Player;
	ParticleManager ParticleManager;
	SessionData Session;
	char * Host;
	SDL_Window * Window;
	SDL_GLContext Context;
	bool LevelLoaded;
	bool Waiting;
	TextureManager TextureManager;
	FontRenderer Font;
	GUIScreen CurrentScreen;
	ProgressBarDisplay ProgressBar;
	Renderer Renderer;
	LevelIO LevelIO;
	//SoundManager SoundManager;
	//ResourceDownloadThread ResourceThread;
	int Ticks;
	int BlockHitTime;
	char * LevelName;
	int LevelID;
	//Robot Robot;
	HUDScreen HUD;
	bool Online;
	//NetworkManager NetworkManager;
	//SoundPlayer SoundPlayer;
	MovingObjectPosition Selected;
	GameSettings Settings;
	struct MinecraftApplet * Applet;
	char * Server;
	int Port;
	bool Running;
	String Debug;
	bool HasMouse;
	int LastClick;
	bool Raining;
	char * WorkingDirectory;
} * Minecraft;

Minecraft MinecraftCreate(MinecraftApplet applet, int width, int height, bool fullScreen);
void MinecraftSetCurrentScreen(Minecraft minecraft, GUIScreen screen);
void MinecraftShutdown(Minecraft minecraft);
void MinecraftRun(Minecraft minecraft);
void MinecraftGrabMouse(Minecraft minecraft);
void MinecraftPause(Minecraft minecraft);
bool MinecraftIsOnline(Minecraft minecraft);
void MinecraftGenerateLevel(Minecraft minecraft, int size);
bool MinecraftLoadOnlineLevel(Minecraft minecraft, char * username, int userID);
void MinecraftSetLevel(Minecraft minecraft, Level level);
void MinecraftDestroy(Minecraft minecraft);
