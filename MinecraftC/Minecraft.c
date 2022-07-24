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

static void CheckGLError(Minecraft * minecraft, char * msg) {
	int error = glGetError();
	if (error != 0) {
		LogError("GL: %i\n", error);
	}
}

void MinecraftCreate(Minecraft * minecraft, int width, int height, bool fullScreen) {
	*minecraft = (Minecraft){ 0 };
	minecraft->ticks = 0;
	minecraft->selected = (MovingObjectPosition){ .null = true };
	minecraft->running = false;
	minecraft->hasMouse = false;
	minecraft->lastClick = 0;
	minecraft->raining = false;
	minecraft->width = width;
	minecraft->height = height;
	minecraft->fullScreen = fullScreen;
	minecraft->debug = StringCreate("");
	minecraft->workingDirectory = SDL_GetPrefPath("NotMojang", "MinecraftC");
	
	SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
	if (minecraft->fullScreen) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	minecraft->window = SDL_CreateWindow("Minecraft 0.30", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, minecraft->width, minecraft->height, flags);
	if (minecraft->window == NULL) {
		LogFatal("Failed to create window: %s\n", SDL_GetError());
	}
	SDL_GetWindowSize(minecraft->window, &minecraft->width, &minecraft->height);
	SDL_GL_GetDrawableSize(minecraft->window, &minecraft->frameWidth, &minecraft->frameHeight);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	minecraft->context = SDL_GL_CreateContext(minecraft->window);
	if (minecraft->context == NULL) {
		LogFatal("Failed to create OpenGL context: %s\n", SDL_GetError());
	}
	
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
	
	TimerCreate(&minecraft->timer, 20.0);
	ProgressBarDisplayCreate(&minecraft->progressBar, minecraft);
	RendererCreate(&minecraft->renderer, minecraft);
	LevelIOCreate(&minecraft->levelIO, &minecraft->progressBar);
	GameSettingsCreate(&minecraft->settings, minecraft);
	SDL_GL_SetSwapInterval(minecraft->settings.limitFramerate ? 1 : 0);
	TextureManagerCreate(&minecraft->textureManager, &minecraft->settings);
	AnimatedTexture * lavaTexture = malloc(sizeof(AnimatedTexture));
	LavaTextureCreate(lavaTexture);
	TextureManagerRegisterAnimation(&minecraft->textureManager, lavaTexture);
	AnimatedTexture * waterTexture = malloc(sizeof(AnimatedTexture));
	WaterTextureCreate(waterTexture);
	TextureManagerRegisterAnimation(&minecraft->textureManager, waterTexture);
	FontRendererCreate(&minecraft->font, &minecraft->settings, "Default.png", &minecraft->textureManager);
	glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
	LevelCreate(&minecraft->level, &minecraft->progressBar, 1);
	PlayerCreate(&minecraft->player, &minecraft->level);
	EntityResetPosition(&minecraft->player);
	minecraft->level.player = &minecraft->player;
	PlayerData * player = &minecraft->player.player;
	InputHandlerCreate(&player->input, &minecraft->settings);
	for (int i = 0; i < 9; i++) {
		player->inventory.slots[i] = SessionDataAllowedBlocks[i];
	}
	LevelRendererCreate(&minecraft->levelRenderer, minecraft, &minecraft->level, &minecraft->textureManager);
	LevelSetRenderer(&minecraft->level, &minecraft->levelRenderer);
	LevelRendererRefresh(&minecraft->levelRenderer);
	ParticleMangerCreate(&minecraft->particleManager, &minecraft->level, &minecraft->textureManager);
	minecraft->level.particleEngine = &minecraft->particleManager;
	HUDScreenCreate(&minecraft->hud, minecraft, minecraft->width, minecraft->height);
	CheckGLError(minecraft, "Post startup");
}

void MinecraftSetCurrentScreen(Minecraft * minecraft, GUIScreen * screen) {
	if (minecraft->currentScreen != NULL && minecraft->currentScreen->type == GUIScreenTypeError) {
		return;
	}
	
	if (minecraft->currentScreen != NULL) {
		GUIScreenOnClose(minecraft->currentScreen);
	}
	minecraft->currentScreen = screen;
	if (screen != NULL) {
		if (minecraft->hasMouse) {
			PlayerReleaseAllKeys(&minecraft->player);
			minecraft->hasMouse = false;
			SDL_ShowCursor(true);
		}
		
		int w = minecraft->width * 240 / minecraft->height;
		int h = minecraft->height * 240 / minecraft->height;
		GUIScreenOpen(screen, minecraft, w, h);
	} else {
		MinecraftGrabMouse(minecraft);
	}
}

void MinecraftRegenerateLevel(Minecraft * minecraft, int size) {
	LevelRegenerate(&minecraft->level, size);
	PlayerCreate(&minecraft->player, &minecraft->level);
	EntityResetPosition(&minecraft->player);
	PlayerData * player = &minecraft->player.player;
	InputHandlerCreate(&player->input, &minecraft->settings);
	for (int i = 0; i < 9; i++) {
		if (player->inventory.slots[i] == -1) {
			player->inventory.slots[i] = SessionDataAllowedBlocks[i];
		}
	}
	LevelRendererDestroy(&minecraft->levelRenderer);
	LevelRendererCreate(&minecraft->levelRenderer, minecraft, &minecraft->level, &minecraft->textureManager);
	LevelSetRenderer(&minecraft->level, &minecraft->levelRenderer);
	LevelRendererRefresh(&minecraft->levelRenderer);
	ParticleManagerDestroy(&minecraft->particleManager);
	ParticleMangerCreate(&minecraft->particleManager, &minecraft->level, &minecraft->textureManager);
}

static void OnMouseClicked(Minecraft * minecraft, int button) {
	PlayerData * player = &minecraft->player.player;
	
	if (button == SDL_BUTTON_LEFT) {
		minecraft->renderer.heldBlock.offset = -1;
		minecraft->renderer.heldBlock.moving = true;
	}
	
	if (!minecraft->selected.null) {
		if (minecraft->selected.entityPosition == 1 && button == SDL_BUTTON_LEFT && minecraft->selected.entity->type == EntityTypePrimedTNT) {
			PrimedTNTOnHit(minecraft->selected.entity);
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
			Block * block = &Blocks.table[LevelGetTile(&minecraft->level, vx, vy, vz)];
			if (button == SDL_BUTTON_LEFT) {
				if (block->type != BlockTypeNone && (block->type != BlockTypeBedrock || player->userType >= 100)) {
					Level * level = &minecraft->level;
					Block * block = &Blocks.table[LevelGetTile(level, vx, vy, vz)];
					bool setTile = LevelSetTile(level, vx, vy, vz, 0);
					if (block->type != BlockTypeNone && setTile) {
						if (block->sound.type != TileSoundTypeNone) {
							LevelPlaySoundAt(level, "step.wtf", vx, vy, vz, (TileSoundGetVolume(block->sound) + 1.0) / 2.0, TileSoundGetPitch(block->sound) * 0.8);
						}
						BlockSpawnBreakParticles(block, level, vx, vy, vz, &minecraft->particleManager);
					}
				}
			} else {
				int selected = InventoryGetSelected(&player->inventory);
				if (selected <= 0) { return; }
				
				Block * block = &Blocks.table[LevelGetTile(&minecraft->level, vx, vy, vz)];
				AABB aabb = Blocks.table[selected].type == BlockTypeNone ? (AABB){ .null = true } : BlockGetCollisionAABB(&Blocks.table[selected], vx, vy, vz);
				if ((block->type == BlockTypeNone || block->type == BlockTypeWater || block->type == BlockTypeStillWater || block->type == BlockTypeLava || block->type == BlockTypeStillLava) && (aabb.null || !AABBIntersects(minecraft->player.aabb, aabb))) {
					LevelSetTile(&minecraft->level, vx, vy, vz, selected);
					minecraft->renderer.heldBlock.position = 0.0;
					BlockOnPlaced(&Blocks.table[selected], &minecraft->level, vx, vy, vz);
				}
			}
		}
	}
}

static void Tick(Minecraft * minecraft, List(SDL_Event) events) {
	HUDScreen * hud = &minecraft->hud;
	hud->ticks++;
	for (int i = 0; i < ListLength(hud->chat); i++) {
		hud->chat[i].time++;
	}
	
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Terrain.png"));
	for (int i = 0; i < ListLength(minecraft->textureManager.animations); i++) {
		AnimatedTexture * texture = minecraft->textureManager.animations[i];
		texture->anaglyph = minecraft->settings.anaglyph;
		AnimatedTextureAnimate(texture);
		memcpy(minecraft->textureManager.textureBuffer, texture->data, 1024);
		glTexSubImage2D(GL_TEXTURE_2D, 0, texture->textureID % 16 << 4, texture->textureID / 16 << 4, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, minecraft->textureManager.textureBuffer);
	}
	
	PlayerData * player = &minecraft->player.player;
	if (minecraft->currentScreen == NULL || minecraft->currentScreen->grabsMouse) {
		for (int i = 0; i < ListLength(events); i++) {
			if (events[i].type == SDL_MOUSEWHEEL) {
				InventorySwapPaint(&player->inventory, events[i].wheel.y);
			}
			if (minecraft->currentScreen == NULL) {
				if (!minecraft->hasMouse && events[i].type == SDL_MOUSEBUTTONDOWN) {
					MinecraftGrabMouse(minecraft);
				} else if (events[i].type == SDL_MOUSEBUTTONDOWN) {
					if (events[i].button.button == SDL_BUTTON_LEFT) {
						OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
						minecraft->lastClick = minecraft->ticks;
					}
					if (events[i].button.button == SDL_BUTTON_RIGHT) {
						OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
						minecraft->lastClick = minecraft->ticks;
					}
					if (events[i].button.button == SDL_BUTTON_MIDDLE && !minecraft->selected.null) {
						BlockType tile = LevelGetTile(&minecraft->level, minecraft->selected.x, minecraft->selected.y, minecraft->selected.z);
						if (tile == BlockTypeGrass) { tile = BlockTypeDirt; }
						if (tile == BlockTypeDoubleSlab) { tile = BlockTypeSlab; }
						if (tile == BlockTypeBedrock) { tile = BlockTypeStone; }
						InventoryGrabTexture(&player->inventory, tile);
					}
				}
			} else {
				GUIScreenMouseEvent(minecraft->currentScreen, events[i]);
			}
		}
		
		for (int i = 0; i < ListLength(events); i++) {
			if (events[i].type == SDL_KEYDOWN || events[i].type == SDL_KEYUP) {
				PlayerSetKey(&minecraft->player, events[i].key.keysym.scancode, events[i].type == SDL_KEYDOWN);
			}
			if (events[i].type == SDL_KEYDOWN) {
				if (minecraft->currentScreen != NULL) {
					GUIScreenKeyboardEvent(minecraft->currentScreen, events[i]);
				} else {
					if (events[i].key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						MinecraftPause(minecraft);
						events[i] = (SDL_Event){ 0 };
					}
					if (events[i].key.keysym.scancode == minecraft->settings.loadLocationKey.key) {
						EntityResetPosition(&minecraft->player);
					}
					if (events[i].key.keysym.scancode == minecraft->settings.saveLocationKey.key) {
						LevelSetSpawnPosition(&minecraft->level, minecraft->player.x, minecraft->player.y, minecraft->player.z, minecraft->player.yRot);
						EntityResetPosition(&minecraft->player);
					}
					if (events[i].key.keysym.scancode == SDL_SCANCODE_F5) {
						minecraft->raining = !minecraft->raining;
					}
					if (events[i].key.keysym.scancode == minecraft->settings.buildKey.key) {
						BlockSelectScreen * blockSelect = malloc(sizeof(BlockSelectScreen));
						BlockSelectScreenCreate(blockSelect);
						MinecraftSetCurrentScreen(minecraft, blockSelect);
					}
					if (events[i].key.keysym.scancode == minecraft->settings.chatKey.key) {
						PlayerReleaseAllKeys(&minecraft->player);
						ChatInputScreen * chatInput = malloc(sizeof(ChatInputScreen));
						ChatInputScreenCreate(chatInput);
						MinecraftSetCurrentScreen(minecraft, chatInput);
						events[i] = (SDL_Event){ 0 };
					}
				}
				for (int j = 0; j < 9; j++) {
					if (events[i].key.keysym.scancode == SDL_SCANCODE_1 + j) {
						player->inventory.selected = j;
					}
				}
				if (events[i].key.keysym.scancode == minecraft->settings.toggleFogKey.key) {
					GameSettingsToggleSetting(&minecraft->settings, 4);
				}
			}
		}
		
		if (minecraft->currentScreen == NULL) {
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_LEFT)) > 0 && (minecraft->ticks - minecraft->lastClick) >= minecraft->timer.ticksPerSecond / 4.0 && minecraft->hasMouse) {
				OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
				minecraft->lastClick = minecraft->ticks;
			}
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_RIGHT)) > 0 && (minecraft->ticks - minecraft->lastClick) >= minecraft->timer.ticksPerSecond / 4.0 && minecraft->hasMouse) {
				OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
				minecraft->lastClick = minecraft->ticks;
			}
		}
	}
	
	if (minecraft->currentScreen != NULL) {
		minecraft->lastClick = minecraft->ticks + 10000;
		GUIScreenDoInput(minecraft->currentScreen, events);
		if (minecraft->currentScreen != NULL) {
			GUIScreenTick(minecraft->currentScreen);
		}
	}
	
	Renderer * renderer = &minecraft->renderer;
	renderer->heldBlock.lastPosition = renderer->heldBlock.position;
	if (renderer->heldBlock.moving) {
		renderer->heldBlock.offset++;
		if (renderer->heldBlock.offset == 7) {
			renderer->heldBlock.offset = 0;
			renderer->heldBlock.moving = false;
		}
	}
	int selected = InventoryGetSelected(&player->inventory);
	Block * block = &Blocks.table[BlockTypeNone];
	if (selected >= 0) { block = &Blocks.table[selected]; }
	float s = (block == renderer->heldBlock.block ? 1.0 : 0.0) - renderer->heldBlock.position;
	if (s < -0.4) { s = -0.4; }
	if (s > 0.4) { s = 0.4; }
	renderer->heldBlock.position += s;
	if (renderer->heldBlock.position < 0.1) {
		renderer->heldBlock.block = block;
	}
	
	if (minecraft->raining) {
		Level * level = &minecraft->level;
		int vx = minecraft->player.x;
		int vy = minecraft->player.y;
		int vz = minecraft->player.z;
		for (int i = 0; i < 50; i++) {
			int rx = vx + (int)RandomGeneratorIntegerRange(&renderer->random, 0, 8) - 4;
			int rz = vz + (int)RandomGeneratorIntegerRange(&renderer->random, 0, 8) - 4;
			int ry = LevelGetHighestTile(level, rx, rz);
			if (ry <= vy + 4 && ry >= vy - 4) {
				float xo = RandomGeneratorUniform(&renderer->random);
				float zo = RandomGeneratorUniform(&renderer->random);
				WaterDropParticle * particle = malloc(sizeof(WaterDropParticle));
				WaterDropParticleCreate(particle, level, rx + xo, ry + 0.1, rz + zo);
				ParticleManagerSpawnParticle(&minecraft->particleManager, particle);
			}
		}
	}
	
	minecraft->levelRenderer.ticks++;
	LevelTickEntities(&minecraft->level);
	LevelTick(&minecraft->level);
	ParticleManagerTick(&minecraft->particleManager);
}

void MinecraftRun(Minecraft * minecraft) {
	minecraft->running = true;
	int frame = 0;
	uint64_t start = TimeMilli();
	List(SDL_Event) events = ListCreate(sizeof(SDL_Event));
	while (minecraft->running) {
		if (minecraft->timer.elapsedTicks > 0) { events = ListClear(events); };
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { minecraft->running = false; }
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_GetWindowSize(minecraft->window, &minecraft->width, &minecraft->height);
				SDL_GL_GetDrawableSize(minecraft->window, &minecraft->frameWidth, &minecraft->frameHeight);
				glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
				HUDScreenDestroy(&minecraft->hud);
				HUDScreenCreate(&minecraft->hud, minecraft, minecraft->width, minecraft->height);
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
		
		Timer * timer = &minecraft->timer;
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
		
		float delta = timer->delta;
		Renderer * renderer = &minecraft->renderer;
		if (renderer->displayActive && (SDL_GetWindowFlags(minecraft->window) & SDL_WINDOW_INPUT_FOCUS) == 0) {
			MinecraftPause(minecraft);
		}
		renderer->displayActive = (SDL_GetWindowFlags(minecraft->window) & SDL_WINDOW_INPUT_FOCUS) > 0;
		
		if (minecraft->hasMouse) {
			int dx = 0, dy = 0;
			int x = 0, y = 0;
			SDL_GetWindowPosition(minecraft->window, &x, &y);
			x += minecraft->width / 2;
			y += minecraft->height / 2;
			SDL_GetGlobalMouseState(&dx, &dy);
			dx -= x;
			dy = -(dy - y);
			SDL_WarpMouseGlobal(x, y);
			
			EntityTurn(&minecraft->player, dy * (minecraft->settings.invertMouse ? -1.0 : 1.0), dx);
		}
		
		int w = minecraft->width * 240 / minecraft->height;
		int h = minecraft->height * 240 / minecraft->height;
		int mx = 0, my = 0;
		SDL_GetMouseState(&mx, &my);
		mx = mx * w / minecraft->width;
		my = my * h / minecraft->height - 1;
		
		Player * player = &minecraft->player;
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
		minecraft->selected = LevelClip(&minecraft->level, v, v2);
		renderer->entity = NULL;
		a = 0.0;
		for (int i = 0; i < ListLength(minecraft->level.entities); i++) {
			Entity * entity = minecraft->level.entities[i];
			float dist = sqrtf((entity->x - minecraft->level.player->x) * (entity->x - minecraft->level.player->x) + (entity->y - minecraft->level.player->y) * (entity->y - minecraft->level.player->y) + (entity->z - minecraft->level.player->z) * (entity->z - minecraft->level.player->z));
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
			if (minecraft->settings.anaglyph) {
				if (i == 0) { glColorMask(false, true, true, false); }
				else { glColorMask(true, false, false, false); }
			}
			
			Level * level = &minecraft->level;
			glViewport(0, 0, minecraft->frameWidth, minecraft->frameHeight);
			float a = 1.0 / (4 - minecraft->settings.viewDistance);
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
			Block * block = &Blocks.table[LevelGetTile(level, player->x, player->y + 0.12, player->z)];
			if (block->type != BlockTypeNone && BlockGetLiquidType(block) != LiquidTypeNone) {
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
			if (minecraft->settings.anaglyph) {
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
			renderer->fogEnd = (512 >> (minecraft->settings.viewDistance << 1));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if (minecraft->settings.anaglyph) { glTranslatef(-((i << 1) - 1) * 0.07, 0.0, 0.0); }
			
			gluPerspective(70.0, (float)minecraft->width / (float)minecraft->height, 0.05, renderer->fogEnd);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			if (minecraft->settings.anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
			if (minecraft->settings.viewBobbing) { RendererApplyBobbing(renderer, delta); }
			
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
			LevelRenderer * lrenderer = &minecraft->levelRenderer;
			for (int j = 0; j < lrenderer->chunkCacheCount; j++) { ChunkClip(lrenderer->chunkCache[j], frustum); }
			qsort(lrenderer->chunks, ListLength(lrenderer->chunks), sizeof(Chunk *), ChunkVisibleDistanceComparator);
			
			int limit = ListLength(lrenderer->chunks);
			if (limit > 10) { limit = 10; }
			for (int j = 0; j < limit; j++) {
				Chunk * chunk = lrenderer->chunks[0];
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
							if (tile != BlockTypeNone && BlockIsSolid(&Blocks.table[tile])) {
								glColor4f(0.2, 0.2, 0.2, 1.0);
								glDepthFunc(GL_LESS);
								ShapeRendererBegin();
								for (int j = 0; j < 6; j++) { BlockRenderInside(&Blocks.table[tile], vx, vy, vz, j); }
								ShapeRendererEnd();
								glCullFace(GL_FRONT);
								ShapeRendererBegin();
								for (int j = 0; j < 6; j++) { BlockRenderInside(&Blocks.table[tile], vx, vy, vz, j); }
								ShapeRendererEnd();
								glCullFace(GL_BACK);
								glDepthFunc(GL_LEQUAL);
							}
						}
					}
				}
			}
			
			RendererSetLighting(renderer, true);
			LevelRenderEntities(&minecraft->level, &minecraft->textureManager, delta);
			ParticleManager * particles = &minecraft->particleManager;
			RendererSetLighting(renderer, false);
			RendererUpdateFog(renderer);
			float dt = delta;
			float c = -tcos(player->yRot * M_PI / 180.0);
			float s = -tsin(player->yRot * M_PI / 180.0);
			float ss = -s * tsin(player->xRot * M_PI / 180.0);
			float cs = c * tsin(player->xRot * M_PI / 180.0);
			float c2 = tcos(player->xRot * M_PI / 180.0);
			for (int j = 0; j < 2; j++) {
				if (ListLength(particles->particles[j]) != 0) {
					int tex = 0;
					if (j == 0) { tex = TextureManagerLoad(&minecraft->textureManager, "Particles.png"); }
					if (j == 1) { tex = TextureManagerLoad(&minecraft->textureManager, "Terrain.png"); }
					glBindTexture(GL_TEXTURE_2D, tex);
					ShapeRendererBegin();
					for (int k = 0; k < ListLength(particles->particles[j]); k++) {
						ParticleRender(particles->particles[j][k], dt, c, c2, s, ss, cs);
					}
					ShapeRendererEnd();
				}
			}
			
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Rock.png"));
			glEnable(GL_TEXTURE_2D);
			glCallList(lrenderer->listID);
			RendererUpdateFog(renderer);
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Clouds.png"));
			glColor4f(1.0, 1.0, 1.0, 1.0);
			float cloudR = ((level->cloudColor >> 24) & 0xff) / 255.0;
			float cloudG = ((level->cloudColor >> 16) & 0xff) / 255.0;
			float cloudB = ((level->cloudColor >> 8) & 0xff) / 255.0;
			if (minecraft->settings.anaglyph) {
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
			if (minecraft->settings.anaglyph) {
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
					AABB aabb = AABBGrow(BlockGetSelectionAABB(&Blocks.table[tile], pos.x, pos.y, pos.z), 0.002, 0.002, 0.002);
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
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Water.png"));
			glCallList(lrenderer->listID + 1);
			glDisable(GL_BLEND);
			glEnable(GL_BLEND);
			glColorMask(false, false, false, false);
			int count = LevelRendererSortChunks(lrenderer, player, 1);
			glColorMask(true, true, true, true);
			if (minecraft->settings.anaglyph) {
				if (i == 0) { glColorMask(false, true, true, false); }
				else { glColorMask(true, false, false, false); }
			}
			glDisable(GL_CULL_FACE);
			if (count > 0) {
				glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Terrain.png"));
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
				glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Rain.png"));
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
			if (minecraft->settings.anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
			if (minecraft->settings.viewBobbing) { RendererApplyBobbing(renderer, delta); }
			
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
				glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(&minecraft->textureManager, "Terrain.png"));
				BlockRenderPreview(held.block);
			}
			glDisable(GL_NORMALIZE);
			glPopMatrix();
			RendererSetLighting(renderer, false);
			
			if (!minecraft->settings.anaglyph) { break; }
			
			if (i == 1) { glColorMask(true, true, true, true); }
		}
		
		HUDScreenRender(&minecraft->hud, delta, mx, my);
		
		if (minecraft->currentScreen != NULL) { GUIScreenRender(minecraft->currentScreen, mx, my); }
		
		SDL_GL_SwapWindow(minecraft->window);
		
		CheckGLError(minecraft, "Post render");
		frame++;
		
		while (TimeMilli() >= start + 1000) {
			String chunks = StringCreateFromInt(ChunkUpdates);
			StringConcat(&chunks, " chunk updates");
			StringSetFromInt(&minecraft->debug, frame);
			StringConcat(&minecraft->debug, " fps, ");
			StringConcat(&minecraft->debug, chunks);
			StringFree(chunks);
			start += 1000;
			frame = 0;
			ChunkUpdates = 0;
		}
	}
}

void MinecraftGrabMouse(Minecraft * minecraft) {
	if (!minecraft->hasMouse) {
		minecraft->hasMouse = true;
		SDL_ShowCursor(false);
		SDL_WarpMouseInWindow(minecraft->window, minecraft->width / 2, minecraft->height / 2);
		MinecraftSetCurrentScreen(minecraft, NULL);
		minecraft->lastClick = minecraft->ticks + 10000;
	}
}

void MinecraftPause(Minecraft * minecraft) {
	if (minecraft->currentScreen == NULL) {
		PauseScreen * pause = malloc(sizeof(PauseScreen));
		PauseScreenCreate(pause);
		MinecraftSetCurrentScreen(minecraft, pause);
	}
}

void MinecraftDestroy(Minecraft * minecraft) {
	StringFree(minecraft->debug);
}

static Minecraft minecraft;

int main(int argc, char * argv[]) {
#ifdef _WIN32
#include <Windows.h>
	SetProcessDPIAware();
#endif
	SinTableInitialize();
	BlocksInitialize();
	ShapeRendererInitialize();
	SessionDataInitialize();
	RandomSetSeed((unsigned int)time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	MinecraftCreate(&minecraft, 860, 480, false);
	MinecraftRun(&minecraft);
	return 0;
}
