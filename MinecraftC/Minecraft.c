#include <string.h>
#include "Minecraft.h"
#include "GUI/PauseScreen.h"
#include "GUI/ChatInputScreen.h"
#include "GUI/BlockSelectScreen.h"
#include "Utilities/Log.h"
#include "Utilities/SinTable.h"
#include "Utilities/Time.h"
#include "Utilities/OpenGL.h"
#include "Render/Texture/LavaTexture.h"
#include "Render/Texture/WaterTexture.h"
#include "Render/ShapeRenderer.h"
#include "Level/Generator/LevelGenerator.h"
#include "Particle/WaterDropParticle.h"
#include "Particle/PrimedTNT.h"

Minecraft MinecraftCreate(MinecraftApplet applet, int width, int height, bool fullScreen) {
	Minecraft minecraft = MemoryAllocate(sizeof(struct Minecraft));
	*minecraft = (struct Minecraft){ 0 };
	minecraft->timer = TimerCreate(20.0);
	minecraft->progressBar = ProgressBarDisplayCreate(minecraft);
	minecraft->renderer = RendererCreate(minecraft);
	minecraft->levelIO = LevelIOCreate(minecraft->progressBar);
	minecraft->ticks = 0;
	minecraft->blockHitTime = 0;
	minecraft->levelName = NULL;
	minecraft->levelID = 0;
	minecraft->online = false;
	minecraft->selected = (MovingObjectPosition){ .null = true };
	minecraft->server = NULL;
	minecraft->port = 0;
	minecraft->running = false;
	minecraft->hasMouse = false;
	minecraft->lastClick = 0;
	minecraft->raining = false;
	minecraft->applet = applet;
	minecraft->width = width;
	minecraft->height = height;
	minecraft->fullScreen = fullScreen;
	minecraft->debug = StringCreate("");
	minecraft->session = NULL;
	return minecraft;
}

void MinecraftSetCurrentScreen(Minecraft minecraft, GUIScreen screen) {
	if (minecraft->currentScreen != NULL && minecraft->currentScreen->type == GUIScreenTypeError) { return; }
	
	if (minecraft->currentScreen != NULL) { GUIScreenOnClose(minecraft->currentScreen); }
	minecraft->currentScreen = screen;
	if (screen != NULL) {
		if (minecraft->hasMouse) {
			PlayerReleaseAllKeys(minecraft->player);
			minecraft->hasMouse = false;
			if (minecraft->levelLoaded) { SDL_ShowCursor(true); }
			else { SDL_SetWindowGrab(minecraft->window, false); }
		}
		
		int w = minecraft->width * 240 / minecraft->height;
		int h = minecraft->height * 240 / minecraft->height;
		GUIScreenOpen(screen, minecraft, w, h);
		minecraft->online = false;
	} else { MinecraftGrabMouse(minecraft); }
}

void MinecraftShutdown(Minecraft minecraft) {
	/*try {
		if(this.soundPlayer != null) {
		     SoundPlayer var1 = this.soundPlayer;
		     this.soundPlayer.running = false;
		  }

		  if(this.resourceThread != null) {
		     ResourceDownloadThread var4 = this.resourceThread;
		     this.resourceThread.stopping = true;
		  }
	       } catch (Exception var3) {
		  ;
	       }*/
	if (!minecraft->levelLoaded) {
		LevelIOSave(minecraft->levelIO, minecraft->level, SDL_RWFromFile("level.dat", "rb"));
	}
	
	SDL_GL_DeleteContext(minecraft->context);
	SDL_DestroyWindow(minecraft->window);
}

static void CheckGLError(Minecraft minecraft, char * msg) {
	int error = glGetError();
	if (error != 0) {
		LogError("GL: %i\n", error);
	}
}

static void OnMouseClicked(Minecraft minecraft, int button) {
	PlayerData player = minecraft->player->typeData;
	if (button != SDL_BUTTON_LEFT || minecraft->blockHitTime <= 0) {
		if (button == SDL_BUTTON_LEFT) {
			minecraft->renderer->heldBlock.offset = -1;
			minecraft->renderer->heldBlock.moving = true;
		}
		
		if (!minecraft->selected.null) {
			if (minecraft->selected.entityPosition == 1 && button == SDL_BUTTON_LEFT && minecraft->selected.entity->type == EntityTypePrimedTNT) {
				PrimedTNTHurt(minecraft->selected.entity);
			}
			if (minecraft->selected.entityPosition == 0) {
				int vx = minecraft->selected.x;
				int vy = minecraft->selected.y;
				int vz = minecraft->selected.z;
				if (button != SDL_BUTTON_LEFT) {
					if (minecraft->selected.face == 0) { vy--; }
					if (minecraft->selected.face == 1) { vy++; }
					if (minecraft->selected.face == 2) { vz--; }
					if (minecraft->selected.face == 3) { vz++; }
					if (minecraft->selected.face == 4) { vx--; }
					if (minecraft->selected.face == 5) { vx++; }
				}
				Block block = Blocks.table[LevelGetTile(minecraft->level, vx, vy, vz)];
				if (button == SDL_BUTTON_LEFT) {
					if (block != NULL && (block->type != BlockTypeBedrock || player->userType >= 100)) {
						Level level = minecraft->level;
						Block block = Blocks.table[LevelGetTile(level, vx, vy, vz)];
						bool setTile = LevelNetSetTile(level, vx, vy, vz, 0);
						if (block != NULL && setTile) {
							if (block->sound.type != TileSoundTypeNone) {
								LevelPlaySoundAt(level, "step.wtf", vx, vy, vz, (TileSoundGetVolume(block->sound) + 1.0) / 2.0, TileSoundGetPitch(block->sound) * 0.8);
							}
							BlockSpawnBreakParticles(block, level, vx, vy, vz, minecraft->particleManager);
						}
						return;
					}
				} else {
					int selected = InventoryGetSelected(player->inventory);
					if (selected <= 0) { return; }
					
					Block block = Blocks.table[LevelGetTile(minecraft->level, vx, vy, vz)];
					AABB aabb = Blocks.table[selected] == NULL ? AABBNull : BlockGetCollisionAABB(Blocks.table[selected], vx, vy, vz);
					if ((block == NULL || block->type == BlockTypeWater || block->type == BlockTypeStillWater || block->type == BlockTypeLava || block->type == BlockTypeStillLava) && (AABBIsNull(aabb) || !AABBIntersects(minecraft->player->aabb, aabb))) {
						LevelNetSetTile(minecraft->level, vx, vy, vz, selected);
						minecraft->renderer->heldBlock.position = 0.0;
						BlockOnPlaced(Blocks.table[selected], minecraft->level, vx, vy, vz);
					}
				}
			}
		}
	}
}

static void Tick(Minecraft minecraft, list(SDL_Event) events) {
	/*if (this.soundPlayer != null)
	{
		SoundPlayer var1 = this.soundPlayer;
		SoundManager var2 = this.sound;
		if (System.currentTimeMillis() > var2.lastMusic && var2.playMusic(var1, "calm")) { var2.lastMusic = System.currentTimeMillis() + (long)var2.random.nextInt(900000) + 300000L; }
	}*/
	
	HUDScreen hud = minecraft->hud;
	hud->ticks++;
	for (int i = 0; i < ListCount(hud->chat); i++) { hud->chat[i]->time++; }
	
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Terrain.png"));
	for (int i = 0; i < ListCount(minecraft->textureManager->animations); i++) {
		AnimatedTexture texture = minecraft->textureManager->animations[i];
		texture->anaglyph = minecraft->settings->anaglyph;
		AnimatedTextureAnimate(texture);
		memcpy(minecraft->textureManager->textureBuffer, texture->data, 1024);
		glTexSubImage2D(GL_TEXTURE_2D, 0, texture->textureID % 16 << 4, texture->textureID / 16 << 4, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, minecraft->textureManager->textureBuffer);
	}
	
	PlayerData player = minecraft->player->typeData;
	if (minecraft->currentScreen == NULL || minecraft->currentScreen->grabsMouse) {
		for (int i = 0; i < ListCount(events); i++) {
			if (events[i].type == SDL_MOUSEWHEEL) { InventorySwapPaint(player->inventory, events[i].wheel.y); }
			if (minecraft->currentScreen == NULL) {
				if (!minecraft->hasMouse && events[i].type == SDL_MOUSEBUTTONDOWN) { MinecraftGrabMouse(minecraft); }
				else if (events[i].type == SDL_MOUSEBUTTONDOWN) {
					if (events[i].button.button == SDL_BUTTON_LEFT)
					{
						OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
						minecraft->lastClick = minecraft->ticks;
					}
					if (events[i].button.button == SDL_BUTTON_RIGHT)
					{
						OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
						minecraft->lastClick = minecraft->ticks;
					}
					if (events[i].button.button == SDL_BUTTON_MIDDLE && !minecraft->selected.null)
					{
						BlockType tile = LevelGetTile(minecraft->level, minecraft->selected.x, minecraft->selected.y, minecraft->selected.z);
						if (tile == BlockTypeGrass) { tile = BlockTypeDirt; }
						if (tile == BlockTypeDoubleSlab) { tile = BlockTypeSlab; }
						if (tile == BlockTypeBedrock) { tile = BlockTypeStone; }
						InventoryGrabTexture(player->inventory, tile);
					}
				}
			} else { GUIScreenMouseEvent(minecraft->currentScreen, events[i]); }
		}
		
		if (minecraft->blockHitTime > 0) { minecraft->blockHitTime--; }
		
		for (int i = 0; i < ListCount(events); i++) {
			if (events[i].type == SDL_KEYDOWN || events[i].type == SDL_KEYUP) { PlayerSetKey(minecraft->player, events[i].key.keysym.scancode, events[i].type == SDL_KEYDOWN); }
			if (events[i].type == SDL_KEYDOWN) {
				if (minecraft->currentScreen != NULL) { GUIScreenKeyboardEvent(minecraft->currentScreen, events[i]); }
				else {
					if (events[i].key.keysym.scancode == SDL_SCANCODE_ESCAPE) { MinecraftPause(minecraft); events[i] = (SDL_Event){ 0 }; }
					if (events[i].key.keysym.scancode == minecraft->settings->loadLocationKey.key) { EntityResetPosition(minecraft->player); }
					if (events[i].key.keysym.scancode == minecraft->settings->saveLocationKey.key) {
						LevelSetSpawnPosition(minecraft->level, minecraft->player->x, minecraft->player->y, minecraft->player->z, minecraft->player->yRot);
						EntityResetPosition(minecraft->player);
					}
					if (events[i].key.keysym.scancode == SDL_SCANCODE_F5) { minecraft->raining = !minecraft->raining; }
					if (events[i].key.keysym.scancode == minecraft->settings->buildKey.key) { MinecraftSetCurrentScreen(minecraft, BlockSelectScreenCreate()); }
					if (events[i].key.keysym.scancode == minecraft->settings->chatKey.key)
					{
						//PlayerReleaseAllKeys(minecraft->Player);
						//MinecraftSetCurrentScreen(minecraft, ChatInputScreenCreate());
						//events[i] = (SDL_Event){ 0 };
					}
				}
				for (int j = 0; j < 9; j++) {
					if (events[i].key.keysym.scancode == SDL_SCANCODE_1 + j) { player->inventory->selected = j; }
				}
				if (events[i].key.keysym.scancode == minecraft->settings->toggleFogKey.key) { GameSettingsToggleSetting(minecraft->settings, 4, 1); }
			}
		}
		
		if (minecraft->currentScreen == NULL) {
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_LEFT)) > 0 && (minecraft->ticks - minecraft->lastClick) >= minecraft->timer->ticksPerSecond / 4.0 && minecraft->hasMouse) {
				OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
				minecraft->lastClick = minecraft->ticks;
			}
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_RIGHT)) > 0 && (minecraft->ticks - minecraft->lastClick) >= minecraft->timer->ticksPerSecond / 4.0 && minecraft->hasMouse) {
				OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
				minecraft->lastClick = minecraft->ticks;
			}
		}
	}
	
	if (minecraft->currentScreen != NULL) {
		minecraft->lastClick = minecraft->ticks + 10000;
		GUIScreenDoInput(minecraft->currentScreen, events);
		if (minecraft->currentScreen != NULL) { GUIScreenTick(minecraft->currentScreen); }
	}
	
	if (minecraft->level != NULL) {
		Renderer renderer = minecraft->renderer;
		renderer->levelTicks++;
		renderer->heldBlock.lastPosition = renderer->heldBlock.position;
		if (renderer->heldBlock.moving) {
			renderer->heldBlock.offset++;
			if (renderer->heldBlock.offset == 7) {
				renderer->heldBlock.offset = 0;
				renderer->heldBlock.moving = false;
			}
		}
		PlayerData player = minecraft->player->typeData;
		int selected = InventoryGetSelected(player->inventory);
		Block block = NULL;
		if (selected >= 0) { block = Blocks.table[selected]; }
		float s = (block == renderer->heldBlock.block ? 1.0 : 0.0) - renderer->heldBlock.position;
		if (s < -0.4) { s = -0.4; }
		if (s > 0.4) { s = 0.4; }
		renderer->heldBlock.position += s;
		if (renderer->heldBlock.position < 0.1) { renderer->heldBlock.block = block; }
		
		if (minecraft->raining) {
			Level level = minecraft->level;
			int vx = minecraft->player->x;
			int vy = minecraft->player->y;
			int vz = minecraft->player->z;
			for (int i = 0; i < 50; i++) {
				int rx = vx + (int)RandomGeneratorIntegerRange(renderer->random, 0, 8) - 4;
				int rz = vz + (int)RandomGeneratorIntegerRange(renderer->random, 0, 8) - 4;
				int ry = LevelGetHighestTile(level, rx, rz);
				if (ry <= vy + 4 && ry >= vy - 4) {
					float xo = RandomGeneratorUniform(renderer->random);
					float zo = RandomGeneratorUniform(renderer->random);
					ParticleManagerSpawnParticle(minecraft->particleManager, WaterDropParticleCreate(level, rx + xo, ry + 0.1, rz + zo));
				}
			}
		}
		
		minecraft->levelRenderer->ticks++;
		LevelTickEntities(minecraft->level);
		LevelTick(minecraft->level);
		ParticleManagerTick(minecraft->particleManager);
	}
}

void MinecraftRun(Minecraft minecraft) {
	minecraft->running = true;
	
	SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
	if (minecraft->fullScreen) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
	minecraft->window = SDL_CreateWindow("Minecraft 0.30", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, minecraft->width, minecraft->height, flags);
	if (minecraft->window == NULL) { LogFatal("Failed to create window: %s\n", SDL_GetError()); }
	SDL_GetWindowSize(minecraft->window, &minecraft->width, &minecraft->height);
	SDL_GL_GetDrawableSize(minecraft->window, &minecraft->frameWidth, &minecraft->frameHeight);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	minecraft->context = SDL_GL_CreateContext(minecraft->window);
	if (minecraft->context == NULL) { LogFatal("Failed to create OpenGL context: %s\n", SDL_GetError()); }
	
	CheckGLError(minecraft, "Pre startup");
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);
	glCullFace(GL_BACK);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	CheckGLError(minecraft, "Startup");
	
	BlocksInitialize();
	ShapeRendererInitialize();
	SessionDataInitialize();
	minecraft->workingDirectory = SDL_GetPrefPath("NotMojang", "MinecraftC");
	minecraft->settings = GameSettingsCreate(minecraft);
	SDL_GL_SetSwapInterval(minecraft->settings->limitFramerate ? 1 : 0);
	minecraft->textureManager = TextureManagerCreate(minecraft->settings);
	TextureManagerRegisterAnimation(minecraft->textureManager, LavaTextureCreate());
	TextureManagerRegisterAnimation(minecraft->textureManager, WaterTextureCreate());
	minecraft->font = FontRendererCreate(minecraft->settings, "Default.png", minecraft->textureManager);
	minecraft->levelRenderer = LevelRendererCreate(minecraft, minecraft->textureManager);
	glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
	
	if (!minecraft->levelLoaded) {
		Level level = LevelIOLoad(minecraft->levelIO, SDL_RWFromFile("level.dat", "rb"));
		if (level != NULL) { MinecraftSetLevel(minecraft, level); }
	}
	
	if (minecraft->level == NULL) { MinecraftGenerateLevel(minecraft, 1); }
	minecraft->levelLoaded = true;
	
	minecraft->particleManager = ParticleMangerCreate(minecraft->level, minecraft->textureManager);
	if (minecraft->levelLoaded) {
		/*try {
		    var1.cursor = new Cursor(16, 16, 0, 0, 1, var9, (IntBuffer)null);
		 } catch (LWJGLException var53) {
		    var53.printStackTrace();
		 } */
	}
	
	/*try {
		var1.soundPlayer = new SoundPlayer(var1.settings);
		SoundPlayer var4 = var1.soundPlayer;
		try {
			AudioFormat var67 = new AudioFormat(44100.0F, 16, 2, true, true);
			var4.dataLine = AudioSystem.getSourceDataLine(var67);
			var4.dataLine.open(var67, 4410);
			var4.dataLine.start();
			var4.running = true;
			Thread var73;
			(var73 = new Thread(var4)).setDaemon(true);
			var73.setPriority(10);
			var73.start();
		} catch (Exception var51) {
			var51.printStackTrace();
			var4.running = false;
		}
	} catch (Exception var52) { ; }*/
	
	CheckGLError(minecraft, "Post startup");
	minecraft->hud = HUDScreenCreate(minecraft, minecraft->width, minecraft->height);
	
	int frame = 0;
	uint64_t start = TimeMilli();
	list(SDL_Event) events = ListCreate(sizeof(SDL_Event));
	while (minecraft->running) {
		if (minecraft->timer->elapsedTicks > 0) { events = ListClear(events); };
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { minecraft->running = false; }
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_GetWindowSize(minecraft->window, &minecraft->width, &minecraft->height);
				SDL_GL_GetDrawableSize(minecraft->window, &minecraft->frameWidth, &minecraft->frameHeight);
				glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
				HUDScreenDestroy(minecraft->hud);
				minecraft->hud = HUDScreenCreate(minecraft, minecraft->width, minecraft->height);
				if (minecraft->currentScreen != NULL) {
					int w = minecraft->width * 240 / minecraft->height;
					int h = minecraft->height * 240 / minecraft->height;
					GUIScreenOpen(minecraft->currentScreen, minecraft, w, h);
				}
			}
			SDL_Event copy;
			memcpy(&copy, &event, sizeof(SDL_Event));
			events = ListPush(events, &copy);
		}
		
		Timer timer = minecraft->timer;
		uint64_t time = TimeMilli();
		uint64_t d = time - timer->lastSystemClock;
		uint64_t t2 = TimeNano() / 1000000;
		double b;
		if (d > 1000) {
			uint64_t d2 = t2 - timer->lastHRClock;
			b = (double)d / d2;
			timer->adjustment += (b - timer->adjustment) * 0.2;
			timer->lastSystemClock = time;
			timer->lastHRClock = t2;
		}
		if (d < 0) {
			timer->lastSystemClock = time;
			timer->lastHRClock = t2;
		}
		double a = t2 / 1000.0;
		b = (a - timer->lastHR) * timer->adjustment;
		timer->lastHR = a;
		if (b < 0.0) { b = 0.0; }
		if (b > 1.0) { b = 1.0; }
		timer->elapsedDelta += b * timer->speed * timer->ticksPerSecond;
		timer->elapsedTicks = timer->elapsedDelta;
		if (timer->elapsedTicks > 100) { timer->elapsedTicks = 100; }
		timer->elapsedDelta -= timer->elapsedTicks;
		timer->delta = timer->elapsedDelta;
		
		for (int i = 0; i < timer->elapsedTicks; i++) {
			minecraft->ticks++;
			Tick(minecraft, events);
			events = ListClear(events);
		}
		
		CheckGLError(minecraft, "Pre render");
		glEnable(GL_TEXTURE_2D);
		if (!minecraft->online) {
			float delta = timer->delta;
			Renderer renderer = minecraft->renderer;
			if (renderer->displayActive && (SDL_GetWindowFlags(minecraft->window) & SDL_WINDOW_INPUT_FOCUS) == 0) {
				MinecraftPause(minecraft);
			}
			renderer->displayActive = (SDL_GetWindowFlags(minecraft->window) & SDL_WINDOW_INPUT_FOCUS) > 0;
			
			if (minecraft->hasMouse) {
				int dx = 0, dy = 0;
				if (minecraft->levelLoaded) {
					int x = 0, y = 0;
					SDL_GetWindowPosition(minecraft->window, &x, &y);
					x += minecraft->width / 2;
					y += minecraft->height / 2;
					SDL_GetGlobalMouseState(&dx, &dy);
					dx -= x;
					dy = -(dy - y);
					SDL_WarpMouseGlobal(x, y);
				} else { SDL_GetRelativeMouseState(&dx, &dy); }
				
				EntityTurn(minecraft->player, dy * (minecraft->settings->invertMouse ? -1.0 : 1.0), dx);
			}
			
			int w = minecraft->width * 240 / minecraft->height;
			int h = minecraft->height * 240 / minecraft->height;
			int mx = 0, my = 0;
			SDL_GetMouseState(&mx, &my);
			mx = mx * w / minecraft->width;
			my = my * h / minecraft->height - 1;
			if (minecraft->level != NULL) {
				Player player = minecraft->player;
				float rotx = player->xRotO + (player->xRot - player->xRotO) * delta;
				float roty = player->yRotO + (player->yRot - player->yRotO) * delta;
				Vector3D v = RendererGetPlayerVector(renderer, delta);
				float c1 = tcos(-roty * M_PI / 180.0 - M_PI);
				float s1 = tsin(-roty * M_PI / 180.0 - M_PI);
				float c2 = tcos(-rotx * M_PI / 180.0);
				float s2 = tsin(-rotx * M_PI / 180.0);
				float sc = s1 * c2;
				float cc = c1 * c2;
				float reach = 5.0;
				Vector3D v2 = { v.x + sc * reach, v.y + s2 * reach, v.z + cc * reach };
				minecraft->selected = LevelClip(minecraft->level, v, v2);
				renderer->entity = NULL;
				float a = 0.0;
				for (int i = 0; i < ListCount(minecraft->level->entities); i++) {
					Entity entity = minecraft->level->entities[i];
					float dist = sqrtf((entity->x - minecraft->level->player->x) * (entity->x - minecraft->level->player->x) + (entity->y - minecraft->level->player->y) * (entity->y - minecraft->level->player->y) + (entity->z - minecraft->level->player->z) * (entity->z - minecraft->level->player->z));
					if (EntityIsPickable(entity) && dist < reach) {
						float r = 0.1;
						MovingObjectPosition pos = AABBClip(AABBGrow(entity->aabb, r, r, r), v, v2);
						if (!pos.null) { r = sqrtf(Vector3DSqDistance(v, pos.vector)); }
						if ((!pos.null && r < a) || a == 0.0) {
							renderer->entity = entity;
							a = r;
						}
					}
				}
				
				if (renderer->entity != NULL) {
					minecraft->selected = (MovingObjectPosition){ .entityPosition = 1, .entity = renderer->entity };
				}
			
				for (int i = 0; i < 2; i++) {
					if (minecraft->settings->anaglyph) {
						if (i == 0) { glColorMask(false, true, true, false); }
						else { glColorMask(true, false, false, false); }
					}
					
					Level level = minecraft->level;
					glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
					float a = 1.0 / (4 - minecraft->settings->viewDistance);
					a = 1.0 - pow(a, 0.25);
					float skyR = ((level->skyColor >> 24) & 0xff) / 255.0;
					float skyG = ((level->skyColor >> 16) & 0xff) / 255.0;
					float skyB = ((level->skyColor >> 8) & 0xff) / 255.0;
					renderer->fogR = ((level->fogColor >> 24) & 0xff) / 255.0;
					renderer->fogG = ((level->fogColor >> 16) & 0xff) / 255.0;
					renderer->fogB = ((level->fogColor >> 8) & 0xff) / 255.0;
					renderer->fogR = (renderer->fogR + (skyR - renderer->fogR) * a) * renderer->fogColorMultiplier;
					renderer->fogG = (renderer->fogG + (skyG - renderer->fogG) * a) * renderer->fogColorMultiplier;
					renderer->fogB = (renderer->fogB + (skyB - renderer->fogB) * a) * renderer->fogColorMultiplier;
					Block block = Blocks.table[LevelGetTile(level, player->x, player->y + 0.12, player->z)];
					if (block != NULL && BlockGetLiquidType(block) != LiquidTypeNone) {
						if (BlockGetLiquidType(block) == LiquidTypeWater) {
							renderer->fogR = 0.02;
							renderer->fogG = 0.02;
							renderer->fogB = 0.2;
						}
						if (BlockGetLiquidType(block) == LiquidTypeLava) {
							renderer->fogR = 0.6;
							renderer->fogG = 0.0;
							renderer->fogB = 0.1;
						}
					}
					if (minecraft->settings->anaglyph) {
						float ar = (renderer->fogR * 30.0 + renderer->fogG * 59.0 + renderer->fogB * 11.0) / 100.0;
						float ag = (renderer->fogR * 30.0 + renderer->fogG * 70.0) / 100.0;
						float ab = (renderer->fogR * 30.0 + renderer->fogB * 70.0) / 100.0;
						renderer->fogR = ar;
						renderer->fogG = ag;
						renderer->fogB = ab;
					}
					
					glClearColor(renderer->fogR, renderer->fogG, renderer->fogB, 0.0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					renderer->fogColorMultiplier = 1.0;
					glEnable(GL_CULL_FACE);
					renderer->fogEnd = (512 >> (minecraft->settings->viewDistance << 1));
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					if (minecraft->settings->anaglyph) { glTranslatef(-((i << 1) - 1) * 0.07, 0.0, 0.0); }
					
					gluPerspective(70.0, (float)minecraft->width / (float)minecraft->height, 0.05, renderer->fogEnd);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					if (minecraft->settings->anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
					if (minecraft->settings->viewBobbing) { RendererApplyBobbing(renderer, delta); }
					
					glTranslatef(0.0, 0.0, -0.1);
					float rotx = player->xRotO + (player->xRot - player->xRotO) * delta;
					float roty = player->yRotO + (player->yRot - player->yRotO) * delta;
					glRotatef(rotx, 1.0, 0.0, 0.0);
					glRotatef(roty, 0.0, 1.0, 0.0);
					float posx = player->xo + (player->x - player->xo) * delta;
					float posy = player->yo + (player->y - player->yo) * delta;
					float posz = player->zo + (player->z - player->zo) * delta;
					glTranslatef(-posx, -posy, -posz);
					
					Frustum frustum = FrustumUpdate();
					LevelRenderer lrenderer = minecraft->levelRenderer;
					for (int j = 0; j < lrenderer->chunkCacheCount; j++) { ChunkClip(lrenderer->chunkCache[j], frustum); }
					qsort(lrenderer->chunks, ListCount(lrenderer->chunks), sizeof(Chunk), ChunkVisibleDistanceComparator);
					
					int limit = ListCount(lrenderer->chunks);
					if (limit > 10) { limit = 10; }
					for (int j = 0; j < limit; j++) {
						Chunk chunk = lrenderer->chunks[0];
						lrenderer->chunks = ListRemove(lrenderer->chunks, 0);
						ChunkUpdate(chunk);
						chunk->loaded = false;
					}
					
					RendererUpdateFog(renderer);
					glEnable(GL_FOG);
					LevelRendererSortChunks(lrenderer, player, 0);
					if (LevelIsSolidSearch(level, player->x, player->y, player->z, 0.1)) {
						int px = player->x, py = player->y, pz = player->z;
						for (int x = px - 1; x <= px + 1; x++) {
							for (int y = py - 1; y <= py + 1; y++) {
								for (int z = pz - 1; z <= pz + 1; z++) {
									int vx = x, vy = y, vz = z;
									BlockType tile = LevelGetTile(level, x, y, z);
									if (tile != BlockTypeNone && BlockIsSolid(Blocks.table[tile])) {
										glColor4f(0.2, 0.2, 0.2, 1.0);
										glDepthFunc(GL_LESS);
										ShapeRendererBegin();
										for (int j = 0; j < 6; j++) { BlockRenderInside(Blocks.table[tile], vx, vy, vz, j); }
										ShapeRendererEnd();
										glCullFace(GL_FRONT);
										ShapeRendererBegin();
										for (int j = 0; j < 6; j++) { BlockRenderInside(Blocks.table[tile], vx, vy, vz, j); }
										ShapeRendererEnd();
										glCullFace(GL_BACK);
										glDepthFunc(GL_LEQUAL);
									}
								}
							}
						}
					}
					
					RendererSetLighting(renderer, true);
					LevelRenderEntities(minecraft->level, minecraft->textureManager, delta);
					ParticleManager particles = minecraft->particleManager;
					RendererSetLighting(renderer, false);
					RendererUpdateFog(renderer);
					float dt = delta;
					float c = -tcos(player->yRot * M_PI / 180.0);
					float s = -tsin(player->yRot * M_PI / 180.0);
					float ss = -s * tsin(player->xRot * M_PI / 180.0);
					float cs = c * tsin(player->xRot * M_PI / 180.0);
					float c2 = tcos(player->xRot * M_PI / 180.0);
					for (int j = 0; j < 2; j++) {
						if (ListCount(particles->particles[j]) != 0) {
							int tex = 0;
							if (j == 0) { tex = TextureManagerLoad(minecraft->textureManager, "Particles.png"); }
							if (j == 1) { tex = TextureManagerLoad(minecraft->textureManager, "Terrain.png"); }
							glBindTexture(GL_TEXTURE_2D, tex);
							ShapeRendererBegin();
							for (int k = 0; k < ListCount(particles->particles[j]); k++) {
								ParticleRender(particles->particles[j][k], dt, c, c2, s, ss, cs);
							}
							ShapeRendererEnd();
						}
					}
					
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Rock.png"));
					glEnable(GL_TEXTURE_2D);
					glCallList(lrenderer->listID);
					RendererUpdateFog(renderer);
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Clouds.png"));
					glColor4f(1.0, 1.0, 1.0, 1.0);
					float cloudR = ((level->cloudColor >> 24) & 0xff) / 255.0;
					float cloudG = ((level->cloudColor >> 16) & 0xff) / 255.0;
					float cloudB = ((level->cloudColor >> 8) & 0xff) / 255.0;
					if (minecraft->settings->anaglyph) {
						float ar = (cloudR * 30.0 + cloudG * 59.0 + cloudB * 11.0) / 100.0;
						float ag = (cloudR * 30.0 + cloudG * 70.0) / 100.0;
						float ab = (cloudR * 30.0 + cloudB * 70.0) / 100.0;
						cloudR = ar;
						cloudG = ag;
						cloudB = ab;
					}
					float z = level->depth + 2.0;
					float t = 0.03 * (lrenderer->ticks + delta);
					ShapeRendererBegin();
					ShapeRendererColorf(cloudR, cloudG, cloudB);
					for (int x = -2048; x < level->width + 2048; x += 512) {
						for (int y = -2048; y < level->height + 2048; y += 512) {
							ShapeRendererVertexUV(x, z, y + 512, (x + t) / 2048.0, (y + 512) / 2048.0);
							ShapeRendererVertexUV(x + 512, z, y + 512, (x + 512 + t) / 2048.0, (y + 512) / 2048.0);
							ShapeRendererVertexUV(x + 512, z, y, (x + 512 + t) / 2048.0, y / 2048.0);
							ShapeRendererVertexUV(x, z, y, (x + t) / 2048.0, y / 2048.0);
							ShapeRendererVertexUV(x, z, y, (x + t) / 2048.0, y / 2048.0);
							ShapeRendererVertexUV(x + 512, z, y, (x + 512 + t) / 2048.0, y / 2048.0);
							ShapeRendererVertexUV(x + 512, z, y + 512, (x + 512 + t) / 2048.0, (y + 512) / 2048.0);
							ShapeRendererVertexUV(x, z, y + 512, (x + t) / 2048.0, (y + 512) / 2048.0);
						}
					}
					ShapeRendererEnd();
					
					glDisable(GL_TEXTURE_2D);
					ShapeRendererBegin();
					if (minecraft->settings->anaglyph) {
						float ar = (skyR * 30.0 + skyG * 59.0 + skyB * 11.0) / 100.0;
						float ag = (skyR * 30.0 + skyG * 70.0) / 100.0;
						float ab = (skyR * 30.0 + skyB * 70.0) / 100.0;
						skyR = ar;
						skyG = ag;
						skyB = ab;
					}
					ShapeRendererColorf(skyR, skyG, skyB);
					z = level->depth + 10.0;
					for (int x = -2048; x < level->width + 2048; x += 512) {
						for (int y = -2048; y < level->height + 2048; y += 512) {
							ShapeRendererVertex(x, z, y);
							ShapeRendererVertex(x + 512, z, y);
							ShapeRendererVertex(x + 512, z, y + 512);
							ShapeRendererVertex(x, z, y + 512);
						}
					}
					ShapeRendererEnd();
					glEnable(GL_TEXTURE_2D);
					RendererUpdateFog(renderer);
					
					if (!minecraft->selected.null) {
						glDisable(GL_ALPHA_TEST);
						MovingObjectPosition pos = minecraft->selected;
						glEnable(GL_BLEND);
						glEnable(GL_ALPHA_TEST);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						glColor4f(1.0, 1.0, 1.0, (tsin(TimeMilli() / 100.0) * 0.2 + 0.4) * 0.5);
						if (lrenderer->cracks > 0.0) {
							glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
							int tex = TextureManagerLoad(minecraft->textureManager, "Terrain.png");
							glBindTexture(GL_TEXTURE_2D, tex);
							glColor4f(1.0, 1.0, 1.0, 0.5);
							glPushMatrix();
							Block block = Blocks.table[LevelGetTile(level, pos.x, pos.y, pos.z)];
							float vx = (block->x0 + block->x1) / 2.0;
							float vy = (block->y0 + block->y1) / 2.0;
							float vz = (block->z0 + block->z1) / 2.0;
							glTranslatef(pos.x + vx, pos.y + vy, pos.z + vz);
							glScalef(1.01, 1.01, 1.01);
							glTranslatef(-(pos.x + vx), -(pos.y + vy), -(pos.z + vz));
							ShapeRendererBegin();
							ShapeRendererNoColor();
							glDepthMask(false);
							if (block == NULL) { block = Blocks.table[BlockTypeStone]; }
							for (int j = 0; j < 6; j++) { BlockRenderSideWithTexture(block, pos.x, pos.y, pos.z, j, 240 + (int)(lrenderer->cracks * 10.0)); }
							ShapeRendererEnd();
							glDepthMask(true);
							glPopMatrix();
						}
						glDisable(GL_BLEND);
						glDisable(GL_ALPHA_TEST);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glColor4f(0.0, 0.0, 0.0, 0.4);
						glLineWidth(2.0);
						glDisable(GL_TEXTURE_2D);
						glDepthMask(false);
						BlockType tile = LevelGetTile(level, pos.x, pos.y, pos.z);
						if (tile > BlockTypeNone) {
							AABB aabb = AABBGrow(BlockGetSelectionAABB(Blocks.table[tile], pos.x, pos.y, pos.z), 0.002, 0.002, 0.002);
							glBegin(GL_LINE_STRIP);
							glVertex3f(aabb.x0, aabb.y0, aabb.z0);
							glVertex3f(aabb.x1, aabb.y0, aabb.z0);
							glVertex3f(aabb.x1, aabb.y0, aabb.z1);
							glVertex3f(aabb.x0, aabb.y0, aabb.z1);
							glVertex3f(aabb.x0, aabb.y0, aabb.z0);
							glEnd();
							glBegin(GL_LINE_STRIP);
							glVertex3f(aabb.x0, aabb.y1, aabb.z0);
							glVertex3f(aabb.x1, aabb.y1, aabb.z0);
							glVertex3f(aabb.x1, aabb.y1, aabb.z1);
							glVertex3f(aabb.x0, aabb.y1, aabb.z1);
							glVertex3f(aabb.x0, aabb.y1, aabb.z0);
							glEnd();
							glBegin(GL_LINES);
							glVertex3f(aabb.x0, aabb.y0, aabb.z0);
							glVertex3f(aabb.x0, aabb.y1, aabb.z0);
							glVertex3f(aabb.x1, aabb.y0, aabb.z0);
							glVertex3f(aabb.x1, aabb.y1, aabb.z0);
							glVertex3f(aabb.x1, aabb.y0, aabb.z1);
							glVertex3f(aabb.x1, aabb.y1, aabb.z1);
							glVertex3f(aabb.x0, aabb.y0, aabb.z1);
							glVertex3f(aabb.x0, aabb.y1, aabb.z1);
							glEnd();
						}
						glDepthMask(true);
						glEnable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
						glEnable(GL_ALPHA_TEST);
					}
					
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					RendererUpdateFog(renderer);
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Water.png"));
					glCallList(lrenderer->listID + 1);
					glDisable(GL_BLEND);
					glEnable(GL_BLEND);
					glColorMask(false, false, false, false);
					int count = LevelRendererSortChunks(lrenderer, player, 1);
					glColorMask(true, true, true, true);
					if (minecraft->settings->anaglyph) {
						if (i == 0) { glColorMask(false, true, true, false); }
						else { glColorMask(true, false, false, false); }
					}
					glDisable(GL_CULL_FACE);
					if (count > 0) {
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Terrain.png"));
						glCallLists(count, GL_INT, lrenderer->chunkDataCache);
					}
					glEnable(GL_CULL_FACE);
					
					glDepthMask(true);
					glDisable(GL_BLEND);
					glDisable(GL_FOG);
					if (minecraft->raining) {
						float t = delta;
						int px = player->x, py = player->y, pz = player->z;
						glDisable(GL_CULL_FACE);
						glNormal3f(0.0, 1.0, 0.0);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Rain.png"));
						for (int x = px - 5; x <= px + 5; x++) {
							for (int z = pz - 5; z <= pz + 5; z++) {
								int y = LevelGetHighestTile(level, x, z);
								int ymin = py - 5;
								int ymax = py + 5;
								if (ymin < y) { ymin = y; }
								if (ymax < y) { ymax = y; }
								if (ymin != ymax) {
									float tt = (((lrenderer->ticks + x * 3121 + z * 418711) % 32) + t) / 32.0;
									float d = sqrtf((x + 0.5 - player->x) * (x + 0.5 - player->x) + (z + 0.5 - player->z) * (z + 0.5 - player->z)) / 5.0;
									glColor4f(1.0, 1.0, 1.0, (1.0 - d * d) * 0.7);
									ShapeRendererBegin();
									ShapeRendererVertexUV(x, ymin, z, 0.0, ymin / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x + 1, ymin, z + 1, 2.0, ymin / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x + 1, ymax, z + 1, 2.0, ymax / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x, ymax, z, 0.0, ymax / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x, ymin, z + 1, 0.0, ymin / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x + 1, ymin, z, 2.0, ymin / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x + 1, ymax, z, 2.0, ymax / 4.0 + tt * 2.0);
									ShapeRendererVertexUV(x, ymax, z + 1, 0.0, ymax / 4.0 + tt * 2.0);
									ShapeRendererEnd();
								}
							}
						}
						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);
					}
					
					glClear(GL_DEPTH_BUFFER_BIT);
					glLoadIdentity();
					if (minecraft->settings->anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
					if (minecraft->settings->viewBobbing) { RendererApplyBobbing(renderer, delta); }
					
					HeldBlock held = renderer->heldBlock;
					float heldPos = held.lastPosition + (held.position - held.lastPosition) * delta;
					glPushMatrix();
					glRotatef(rotx, 1.0, 0.0, 0.0);
					glRotatef(roty, 0.0, 1.0, 0.0);
					RendererSetLighting(renderer, true);
					glPopMatrix();
					glPushMatrix();
					if (held.moving) {
						float a = (held.offset + delta) / 7.0;
						glTranslatef(-tsin(sqrt(a) * M_PI) * 0.4, tsin(sqrt(a) * M_PI * 2.0) * 0.2, -tsin(a * M_PI) * 0.2);
					}
					glTranslatef(0.7 * 0.8, -0.65 * 0.8 - (1.0 - heldPos) * 0.6, -0.9 * 0.8);
					glRotatef(45.0, 0.0, 1.0, 0.0);
					glEnable(GL_NORMALIZE);
					if (held.moving) {
						float a = (held.offset + delta) / 7.0;
						glRotatef(tsin(sqrt(a) * M_PI) * 80.0, 0.0, 1.0, 0.0);
						glRotatef(-tsin(a * a * M_PI), 1.0, 0.0, 0.0);
					}
					float brightness = LevelGetBrightness(level, player->x, player->y, player->z);
					glColor4f(brightness, brightness, brightness, 1.0);
					if (held.block != NULL) {
						glScalef(0.4, 0.4, 0.4);
						glTranslatef(-0.5, -0.5, -0.5);
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->textureManager, "Terrain.png"));
						BlockRenderPreview(held.block);
					}
					glDisable(GL_NORMALIZE);
					glPopMatrix();
					RendererSetLighting(renderer, false);
					
					if (!minecraft->settings->anaglyph) { break; }
					
					if (i == 1) { glColorMask(true, true, true, true); }
				}
				
				HUDScreenRender(minecraft->hud, delta, minecraft->currentScreen != NULL, mx, my);
			} else {
				glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
				glClearColor(0.0, 0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				RendererEnableGUIMode(renderer);
			}
			
			if (minecraft->currentScreen != NULL) { GUIScreenRender(minecraft->currentScreen, mx, my); }
			
			SDL_GL_SwapWindow(minecraft->window);
			
			CheckGLError(minecraft, "Post render");
			frame++;
			
			while (TimeMilli() >= start + 1000) {
				String chunks = StringConcat(StringCreateFromInt(ChunkUpdates), " chunk updates");
				minecraft->debug = StringConcat(StringConcat(StringSetFromInt(minecraft->debug, frame), " fps, "), chunks);
				StringDestroy(chunks);
				start += 1000;
				frame = 0;
				ChunkUpdates = 0;
			}
		}
	}
	MinecraftShutdown(minecraft);
}

void MinecraftGrabMouse(Minecraft minecraft) {
	if (!minecraft->hasMouse) {
		minecraft->hasMouse = true;
		if (minecraft->levelLoaded) {
			SDL_ShowCursor(false);
			SDL_WarpMouseInWindow(minecraft->window, minecraft->width / 2, minecraft->height / 2);
		} else { SDL_SetWindowGrab(minecraft->window, true); }
		MinecraftSetCurrentScreen(minecraft, NULL);
		minecraft->lastClick = minecraft->ticks + 10000;
	}
}

void MinecraftPause(Minecraft minecraft) {
	if (minecraft->currentScreen == NULL) {
		MinecraftSetCurrentScreen(minecraft, PauseScreenCreate());
	}
}

bool MinecraftIsOnline(Minecraft minecraft) {
	return false;
}

void MinecraftGenerateLevel(Minecraft minecraft, int size) {
	char * user = "anonymous";
	LevelGenerator generator = LevelGeneratorCreate(minecraft->progressBar);
	Level level = LevelGeneratorGenerate(generator, user, 128 << size, 128 << size, 64);
	MinecraftSetLevel(minecraft, level);
	LevelGeneratorDestroy(generator);
}

bool MinecraftLoadOnlineLevel(Minecraft minecraft, char * username, int userID) {
	Level level = LevelIOLoadOnline(minecraft->levelIO, minecraft->host, username, userID);
	if (level == NULL) { return false; }
	else {
		MinecraftSetLevel(minecraft, level);
		return true;
	}
}

void MinecraftSetLevel(Minecraft minecraft, Level level) {
	if (minecraft->level != NULL) { LevelDestroy(minecraft->level); }
	minecraft->level = level;
	if (level != NULL) {
		level->font = minecraft->font;
		level->minecraft = minecraft;
		if (minecraft->player != NULL) { EntityDestroy(minecraft->player); }
		minecraft->player = LevelFindPlayer(level);
	}
	
	if (minecraft->player == NULL) {
		minecraft->player = PlayerCreate(level);
		EntityResetPosition(minecraft->player);
		if (level != NULL) { level->player = minecraft->player; }
	}
	
	PlayerData player = minecraft->player->typeData;
	player->input = InputHandlerCreate(minecraft->settings);
	for (int i = 0; i < 9; i++) {
		player->inventory->count[i] = 1;
		if (player->inventory->slots[i] == -1) { player->inventory->slots[i] = SessionDataAllowedBlocks[i]->type; }
	}
	
	if (minecraft->levelRenderer != NULL) {
		LevelRendererDestroy(minecraft->levelRenderer);
		minecraft->levelRenderer = LevelRendererCreate(minecraft, minecraft->textureManager);
		minecraft->levelRenderer->level = level;
		if (level != NULL) {
			LevelAddRenderer(level, minecraft->levelRenderer);
			LevelRendererRefresh(minecraft->levelRenderer);
		}
	}
	
	if (minecraft->particleManager != NULL) {
		if (level != NULL) { level->particleEngine = minecraft->particleManager; }
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < ListCount(minecraft->particleManager->particles[i]); j++) { ParticleDestroy(minecraft->particleManager->particles[i][j]); }
			minecraft->particleManager->particles[i] = ListClear(minecraft->particleManager->particles[i]);
		}
	}
}

void MinecraftDestroy(Minecraft minecraft) {
	TimerDestroy(minecraft->timer);
	ProgressBarDisplayDestroy(minecraft->progressBar);
	RendererDestroy(minecraft->renderer);
	LevelIODestroy(minecraft->levelIO);
	StringDestroy(minecraft->debug);
	MemoryFree(minecraft);
}

int main(int argc, char * argv[]) {
#ifdef _WIN32
#include <Windows.h>
	SetProcessDPIAware();
#endif
	SinTableInitialize();
	RandomSetSeed((unsigned int)time(NULL));
	
	Minecraft minecraft = MinecraftCreate(NULL, 860, 480, false);
	MinecraftRun(minecraft);
	return 0;
}
