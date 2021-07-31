#include <string.h>
#include <OpenGL.h>
#include "Minecraft.h"
#include "GUI/PauseScreen.h"
#include "GUI/ChatInputScreen.h"
#include "GUI/BlockSelectScreen.h"
#include "Utilities/Log.h"
#include "Utilities/Time.h"
#include "Render/Texture/LavaTexture.h"
#include "Render/Texture/WaterTexture.h"
#include "Render/ShapeRenderer.h"
#include "Level/Generator/LevelGenerator.h"
#include "Particle/WaterDropParticle.h"

Minecraft MinecraftCreate(MinecraftApplet applet, int width, int height, bool fullScreen)
{
	Minecraft minecraft = MemoryAllocate(sizeof(struct Minecraft));
	*minecraft = (struct Minecraft){ 0 };
	minecraft->Timer = TimerCreate(20.0);
	minecraft->ProgressBar = ProgressBarDisplayCreate(minecraft);
	minecraft->Renderer = RendererCreate(minecraft);
	minecraft->LevelIO = LevelIOCreate(minecraft->ProgressBar);
	minecraft->Ticks = 0;
	minecraft->BlockHitTime = 0;
	minecraft->LevelName = NULL;
	minecraft->LevelID = 0;
	minecraft->Online = false;
	minecraft->Selected = (MovingObjectPosition){ .Null = true };
	minecraft->Server = NULL;
	minecraft->Port = 0;
	minecraft->Running = false;
	minecraft->HasMouse = false;
	minecraft->LastClick = 0;
	minecraft->Raining = false;
	minecraft->Applet = applet;
	minecraft->Width = width;
	minecraft->Height = height;
	minecraft->FullScreen = fullScreen;
	minecraft->Debug = StringCreate("");
	minecraft->Session = NULL;
	return minecraft;
}

void MinecraftSetCurrentScreen(Minecraft minecraft, GUIScreen screen)
{
	if (minecraft->CurrentScreen != NULL && minecraft->CurrentScreen->Type == GUIScreenTypeError) { return; }
	
	if (minecraft->CurrentScreen != NULL) { GUIScreenOnClose(minecraft->CurrentScreen); }
	minecraft->CurrentScreen = screen;
	if (screen != NULL)
	{
		if (minecraft->HasMouse)
		{
			PlayerReleaseAllKeys(minecraft->Player);
			minecraft->HasMouse = false;
			if (minecraft->LevelLoaded) { SDL_ShowCursor(true); }
			else { SDL_SetWindowGrab(minecraft->Window, false); }
		}
		
		int w = minecraft->Width * 240 / minecraft->Height;
		int h = minecraft->Height * 240 / minecraft->Height;
		GUIScreenOpen(screen, minecraft, w, h);
		minecraft->Online = false;
	}
	else { MinecraftGrabMouse(minecraft); }
}

void MinecraftShutdown(Minecraft minecraft)
{
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
	if (!minecraft->LevelLoaded)
	{
		LevelIOSave(minecraft->LevelIO, minecraft->Level, SDL_RWFromFile("level.dat", "rb"));
	}
	
	SDL_GL_DeleteContext(minecraft->Context);
	SDL_DestroyWindow(minecraft->Window);
}

static void CheckGLError(Minecraft minecraft, char * msg)
{
	int error = glGetError();
	if (error != 0)
	{
		LogError("GL: %i\n", error);
	}
}

static void OnMouseClicked(Minecraft minecraft, int button)
{
	PlayerData player = minecraft->Player->TypeData;
	if (button != SDL_BUTTON_LEFT || minecraft->BlockHitTime <= 0)
	{
		if (button == SDL_BUTTON_LEFT)
		{
			minecraft->Renderer->HeldBlock.Offset = -1;
			minecraft->Renderer->HeldBlock.Moving = true;
		}
		
		if (!minecraft->Selected.Null)
		{
			if (minecraft->Selected.EntityPosition == 0)
			{
				int3 v = minecraft->Selected.XYZ;
				if (button != SDL_BUTTON_LEFT)
				{
					if (minecraft->Selected.Face == 0) { v.y--; }
					if (minecraft->Selected.Face == 1) { v.y++; }
					if (minecraft->Selected.Face == 2) { v.z--; }
					if (minecraft->Selected.Face == 3) { v.z++; }
					if (minecraft->Selected.Face == 4) { v.x--; }
					if (minecraft->Selected.Face == 5) { v.x++; }
				}
				Block block = Blocks.Table[LevelGetTile(minecraft->Level, v.x, v.y, v.z)];
				if (button == SDL_BUTTON_LEFT)
				{
					if (block != NULL && (block->Type != BlockTypeBedrock || player->UserType >= 100))
					{
						Level level = minecraft->Level;
						Block block = Blocks.Table[LevelGetTile(level, v.x, v.y, v.z)];
						bool setTile = LevelNetSetTile(level, v.x, v.y, v.z, 0);
						if (block != NULL && setTile)
						{
							if (block->Sound.Type != TileSoundTypeNone)
							{
								LevelPlaySoundAt(level, "step.wtf", (float3){ v.x, v.y, v.z}, (TileSoundGetVolume(block->Sound) + 1.0) / 2.0, TileSoundGetPitch(block->Sound) * 0.8);
							}
							BlockSpawnBreakParticles(block, level, v.x, v.y, v.z, minecraft->ParticleManager);
						}
						return;
					}
				}
				else
				{
					int selected = InventoryGetSelected(player->Inventory);
					if (selected <= 0) { return; }
					
					Block block = Blocks.Table[LevelGetTile(minecraft->Level, v.x, v.y, v.z)];
					AABB aabb = Blocks.Table[selected] == NULL ? AABBNull : BlockGetCollisionAABB(Blocks.Table[selected], v.x, v.y, v.z);
					if ((block == NULL || block->Type == BlockTypeWater || block->Type == BlockTypeStillWater || block->Type == BlockTypeLava || block->Type == BlockTypeStillLava) && (AABBIsNull(aabb) || !AABBIntersects(minecraft->Player->AABB, aabb)))
					{
						LevelNetSetTile(minecraft->Level, v.x, v.y, v.z, selected);
						minecraft->Renderer->HeldBlock.Position = 0.0;
						BlockOnPlaced(Blocks.Table[selected], minecraft->Level, v.x, v.y, v.z);
					}
				}
			}
		}
	}
}

static void Tick(Minecraft minecraft, list(SDL_Event) events)
{
	/*if (this.soundPlayer != null)
	{
		SoundPlayer var1 = this.soundPlayer;
		SoundManager var2 = this.sound;
		if (System.currentTimeMillis() > var2.lastMusic && var2.playMusic(var1, "calm")) { var2.lastMusic = System.currentTimeMillis() + (long)var2.random.nextInt(900000) + 300000L; }
	}*/
	
	HUDScreen hud = minecraft->HUD;
	hud->Ticks++;
	for (int i = 0; i < ListCount(hud->Chat); i++) { hud->Chat[i]->Time++; }
	
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Terrain.png"));
	for (int i = 0; i < ListCount(minecraft->TextureManager->Animations); i++)
	{
		AnimatedTexture texture = minecraft->TextureManager->Animations[i];
		texture->Anaglyph = minecraft->Settings->Anaglyph;
		AnimatedTextureAnimate(texture);
		memcpy(minecraft->TextureManager->TextureBuffer, texture->Data, 1024);
		glTexSubImage2D(GL_TEXTURE_2D, 0, texture->TextureID % 16 << 4, texture->TextureID / 16 << 4, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, minecraft->TextureManager->TextureBuffer);
	}
	
	PlayerData player = minecraft->Player->TypeData;
	if (minecraft->CurrentScreen == NULL || minecraft->CurrentScreen->GrabsMouse)
	{
		for (int i = 0; i < ListCount(events); i++)
		{
			if (events[i].type == SDL_MOUSEWHEEL) { InventorySwapPaint(player->Inventory, events[i].wheel.y); }
			if (minecraft->CurrentScreen == NULL)
			{
				if (!minecraft->HasMouse && events[i].type == SDL_MOUSEBUTTONDOWN) { MinecraftGrabMouse(minecraft); }
				else if (events[i].type == SDL_MOUSEBUTTONDOWN)
				{
					if (events[i].button.button == SDL_BUTTON_LEFT)
					{
						OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
						minecraft->LastClick = minecraft->Ticks;
					}
					if (events[i].button.button == SDL_BUTTON_RIGHT)
					{
						OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
						minecraft->LastClick = minecraft->Ticks;
					}
					if (events[i].button.button == SDL_BUTTON_MIDDLE && !minecraft->Selected.Null)
					{
						BlockType tile = LevelGetTile(minecraft->Level, minecraft->Selected.XYZ.x, minecraft->Selected.XYZ.y, minecraft->Selected.XYZ.z);
						if (tile == BlockTypeGrass) { tile = BlockTypeDirt; }
						if (tile == BlockTypeDoubleSlab) { tile = BlockTypeSlab; }
						if (tile == BlockTypeBedrock) { tile = BlockTypeStone; }
						InventoryGrabTexture(player->Inventory, tile);
					}
				}
			}
			else { GUIScreenMouseEvent(minecraft->CurrentScreen, events[i]); }
		}
		
		if (minecraft->BlockHitTime > 0) { minecraft->BlockHitTime--; }
		
		for (int i = 0; i < ListCount(events); i++)
		{
			if (events[i].type == SDL_KEYDOWN || events[i].type == SDL_KEYUP) { PlayerSetKey(minecraft->Player, events[i].key.keysym.scancode, events[i].type == SDL_KEYDOWN); }
			if (events[i].type == SDL_KEYDOWN)
			{
				if (minecraft->CurrentScreen != NULL) { GUIScreenKeyboardEvent(minecraft->CurrentScreen, events[i]); }
				else
				{
					if (events[i].key.keysym.scancode == SDL_SCANCODE_ESCAPE) { MinecraftPause(minecraft); events[i] = (SDL_Event){ 0 }; }
					if (events[i].key.keysym.scancode == minecraft->Settings->LoadLocationKey.Key) { EntityResetPosition(minecraft->Player); }
					if (events[i].key.keysym.scancode == minecraft->Settings->SaveLocationKey.Key)
					{
						LevelSetSpawnPosition(minecraft->Level, minecraft->Player->Position.x, minecraft->Player->Position.y, minecraft->Player->Position.z, minecraft->Player->Rotation.y);
						EntityResetPosition(minecraft->Player);
					}
					if (events[i].key.keysym.scancode == SDL_SCANCODE_F5) { minecraft->Raining = !minecraft->Raining; }
					if (events[i].key.keysym.scancode == minecraft->Settings->BuildKey.Key) { MinecraftSetCurrentScreen(minecraft, BlockSelectScreenCreate()); }
					if (events[i].key.keysym.scancode == minecraft->Settings->ChatKey.Key)
					{
						//PlayerReleaseAllKeys(minecraft->Player);
						//MinecraftSetCurrentScreen(minecraft, ChatInputScreenCreate());
						//events[i] = (SDL_Event){ 0 };
					}
				}
				for (int j = 0; j < 9; j++)
				{
					if (events[i].key.keysym.scancode == SDL_SCANCODE_1 + j) { player->Inventory->Selected = j; }
				}
				if (events[i].key.keysym.scancode == minecraft->Settings->ToggleFogKey.Key) { GameSettingsToggleSetting(minecraft->Settings, 4, 1); }
			}
		}
		
		if (minecraft->CurrentScreen == NULL)
		{
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_LEFT)) > 0 && (minecraft->Ticks - minecraft->LastClick) >= minecraft->Timer->TicksPerSecond / 4.0 && minecraft->HasMouse)
			{
				OnMouseClicked(minecraft, SDL_BUTTON_LEFT);
				minecraft->LastClick = minecraft->Ticks;
			}
			if ((SDL_GetMouseState(&(int){ 0 }, &(int){ 0 }) & SDL_BUTTON(SDL_BUTTON_RIGHT)) > 0 && (minecraft->Ticks - minecraft->LastClick) >= minecraft->Timer->TicksPerSecond / 4.0 && minecraft->HasMouse)
			{
				OnMouseClicked(minecraft, SDL_BUTTON_RIGHT);
				minecraft->LastClick = minecraft->Ticks;
			}
		}
	}
	
	if (minecraft->CurrentScreen != NULL)
	{
		minecraft->LastClick = minecraft->Ticks + 10000;
		GUIScreenDoInput(minecraft->CurrentScreen, events);
		if (minecraft->CurrentScreen != NULL) { GUIScreenTick(minecraft->CurrentScreen); }
	}
	
	if (minecraft->Level != NULL)
	{
		Renderer renderer = minecraft->Renderer;
		renderer->LevelTicks++;
		renderer->HeldBlock.LastPosition = renderer->HeldBlock.Position;
		if (renderer->HeldBlock.Moving)
		{
			renderer->HeldBlock.Offset++;
			if (renderer->HeldBlock.Offset == 7)
			{
				renderer->HeldBlock.Offset = 0;
				renderer->HeldBlock.Moving = false;
			}
		}
		PlayerData player = minecraft->Player->TypeData;
		int selected = InventoryGetSelected(player->Inventory);
		Block block = NULL;
		if (selected >= 0) { block = Blocks.Table[selected]; }
		float s = (block == renderer->HeldBlock.Block ? 1.0 : 0.0) - renderer->HeldBlock.Position;
		if (s < -0.4) { s = -0.4; }
		if (s > 0.4) { s = 0.4; }
		renderer->HeldBlock.Position += s;
		if (renderer->HeldBlock.Position < 0.1) { renderer->HeldBlock.Block = block; }
		
		if (minecraft->Raining)
		{
			Level level = minecraft->Level;
			int3 v = int3f(minecraft->Player->Position);
			for (int i = 0; i < 50; i++)
			{
				int rx = v.x + (int)RandomGeneratorIntegerRange(renderer->Random, 0, 8) - 4;
				int rz = v.z + (int)RandomGeneratorIntegerRange(renderer->Random, 0, 8) - 4;
				int ry = LevelGetHighestTile(level, rx, rz);
				if (ry <= v.y + 4 && ry >= v.y - 4)
				{
					float xo = RandomGeneratorUniform(renderer->Random);
					float zo = RandomGeneratorUniform(renderer->Random);
					ParticleManagerSpawnParticle(minecraft->ParticleManager, WaterDropParticleCreate(level, (float3){ rx + xo, ry + 0.1, rz + zo }));
				}
			}
		}
		
		minecraft->LevelRenderer->Ticks++;
		LevelTickEntities(minecraft->Level);
		LevelTick(minecraft->Level);
		ParticleManagerTick(minecraft->ParticleManager);
	}
}

void MinecraftRun(Minecraft minecraft)
{
	minecraft->Running = true;
	
	SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
	if (minecraft->FullScreen) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
	minecraft->Window = SDL_CreateWindow("Minecraft 0.30", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, minecraft->Width, minecraft->Height, flags);
	if (minecraft->Window == NULL) { LogFatal("Failed to create window: %s\n", SDL_GetError()); }
	SDL_GetWindowSize(minecraft->Window, &minecraft->Width, &minecraft->Height);
	SDL_GL_GetDrawableSize(minecraft->Window, &minecraft->FrameWidth, &minecraft->FrameHeight);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	minecraft->Context = SDL_GL_CreateContext(minecraft->Window);
	if (minecraft->Context == NULL) { LogFatal("Failed to create OpenGL context: %s\n", SDL_GetError()); }
	
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
	minecraft->WorkingDirectory = SDL_GetPrefPath("NotMojang", "MinecraftC");
	minecraft->Settings = GameSettingsCreate(minecraft);
	SDL_GL_SetSwapInterval(minecraft->Settings->LimitFramerate ? 1 : 0);
	minecraft->TextureManager = TextureManagerCreate(minecraft->Settings);
	TextureManagerRegisterAnimation(minecraft->TextureManager, LavaTextureCreate());
	TextureManagerRegisterAnimation(minecraft->TextureManager, WaterTextureCreate());
	minecraft->Font = FontRendererCreate(minecraft->Settings, "Default.png", minecraft->TextureManager);
	minecraft->LevelRenderer = LevelRendererCreate(minecraft, minecraft->TextureManager);
	glViewport(0, 0, minecraft->FrameWidth, minecraft->FrameHeight);
	
	if (!minecraft->LevelLoaded)
	{
		Level level = LevelIOLoad(minecraft->LevelIO, SDL_RWFromFile("level.dat", "rb"));
		if (level != NULL) { MinecraftSetLevel(minecraft, level); }
	}
	
	if (minecraft->Level == NULL) { MinecraftGenerateLevel(minecraft, 0); }
	minecraft->LevelLoaded = true;
	
	minecraft->ParticleManager = ParticleMangerCreate(minecraft->Level, minecraft->TextureManager);
	if (minecraft->LevelLoaded)
	{
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
	minecraft->HUD = HUDScreenCreate(minecraft, minecraft->Width, minecraft->Height);
	
	int frame = 0;
	uint64_t start = TimeMilli();
	list(SDL_Event) events = ListCreate(sizeof(SDL_Event));
	while (minecraft->Running)
	{
		if (minecraft->Timer->ElapsedTicks > 0) { events = ListClear(events); };
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) { minecraft->Running = false; }
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(minecraft->Window, &minecraft->Width, &minecraft->Height);
				SDL_GL_GetDrawableSize(minecraft->Window, &minecraft->FrameWidth, &minecraft->FrameHeight);
				glViewport(0, 0, minecraft->FrameWidth, minecraft->FrameHeight);
				HUDScreenDestroy(minecraft->HUD);
				minecraft->HUD = HUDScreenCreate(minecraft, minecraft->Width, minecraft->Height);
				if (minecraft->CurrentScreen != NULL)
				{
					int w = minecraft->Width * 240 / minecraft->Height;
					int h = minecraft->Height * 240 / minecraft->Height;
					GUIScreenOpen(minecraft->CurrentScreen, minecraft, w, h);
				}
			}
			SDL_Event copy;
			memcpy(&copy, &event, sizeof(SDL_Event));
			events = ListPush(events, &copy);
		}
		
		Timer timer = minecraft->Timer;
		uint64_t time = TimeMilli();
		uint64_t d = time - timer->LastSystemClock;
		uint64_t t2 = TimeNano() / 1000000;
		double b;
		if (d > 1000)
		{
			uint64_t d2 = t2 - timer->LastHRClock;
			b = (double)d / d2;
			timer->Adjustment += (b - timer->Adjustment) * 0.2;
			timer->LastSystemClock = time;
			timer->LastHRClock = t2;
		}
		if (d < 0)
		{
			timer->LastSystemClock = time;
			timer->LastHRClock = t2;
		}
		double a = t2 / 1000.0;
		b = (a - timer->LastHR) * timer->Adjustment;
		timer->LastHR = a;
		if (b < 0.0) { b = 0.0; }
		if (b > 1.0) { b = 1.0; }
		timer->ElapsedDelta += b * timer->Speed * timer->TicksPerSecond;
		timer->ElapsedTicks = timer->ElapsedDelta;
		if (timer->ElapsedTicks > 100) { timer->ElapsedTicks = 100; }
		timer->ElapsedDelta -= timer->ElapsedTicks;
		timer->Delta = timer->ElapsedDelta;
		
		for (int i = 0; i < timer->ElapsedTicks; i++)
		{
			minecraft->Ticks++;
			Tick(minecraft, events);
			events = ListClear(events);
		}
		
		CheckGLError(minecraft, "Pre render");
		glEnable(GL_TEXTURE_2D);
		if (!minecraft->Online)
		{
			float delta = timer->Delta;
			Renderer renderer = minecraft->Renderer;
			if (renderer->DisplayActive && (SDL_GetWindowFlags(minecraft->Window) & SDL_WINDOW_INPUT_FOCUS) == 0)
			{
				MinecraftPause(minecraft);
			}
			renderer->DisplayActive = (SDL_GetWindowFlags(minecraft->Window) & SDL_WINDOW_INPUT_FOCUS) > 0;
			
			if (minecraft->HasMouse)
			{
				int dx = 0, dy = 0;
				if (minecraft->LevelLoaded)
				{
					int x = 0, y = 0;
					SDL_GetWindowPosition(minecraft->Window, &x, &y);
					x += minecraft->Width / 2;
					y += minecraft->Height / 2;
					SDL_GetGlobalMouseState(&dx, &dy);
					dx -= x;
					dy = -(dy - y);
					SDL_WarpMouseGlobal(x, y);
				}
				else { SDL_GetRelativeMouseState(&dx, &dy); }
				
				EntityTurn(minecraft->Player, (float2){ dy * (minecraft->Settings->InvertMouse ? -1.0 : 1.0), dx });
			}
			
			int w = minecraft->Width * 240 / minecraft->Height;
			int h = minecraft->Height * 240 / minecraft->Height;
			int mx = 0, my = 0;
			SDL_GetMouseState(&mx, &my);
			mx = mx * w / minecraft->Width;
			my = my * h / minecraft->Height - 1;
			if (minecraft->Level != NULL)
			{
				Player player = minecraft->Player;
				float2 rot = player->OldRotation + (player->Rotation - player->OldRotation) * delta;
				float3 v = RendererGetPlayerVector(renderer, delta);
				float c1 = cos(-rot.y * rad - pi);
				float s1 = sin(-rot.y * rad - pi);
				float c2 = cos(-rot.x * rad);
				float s2 = sin(-rot.x * rad);
				float sc = s1 * c2;
				float cc = c1 * c2;
				float reach = 5.0;
				float3 v2 = v + (float3){ sc, s2, cc } * reach;
				minecraft->Selected = LevelClip(minecraft->Level, v, v2);
				reach = 32.0;
				v2 = v + (float3){ sc, s2, cc } * reach;
			
				for (int i = 0; i <= 2; i++)
				{
					if (i == 2)
					{
						glColorMask(true, true, true, true);
						break;
					}
					
					if (minecraft->Settings->Anaglyph)
					{
						if (i == 0) { glColorMask(false, true, true, false); }
						else { glColorMask(true, false, false, false); }
					}
					
					Level level = minecraft->Level;
					glViewport(0, 0, minecraft->FrameWidth, minecraft->FrameHeight);
					float a = 1.0 / (4 - minecraft->Settings->ViewDistance);
					a = 1.0 - pow(a, 0.25);
					float4 sky = ColorToFloat4(level->SkyColor);
					renderer->FogColor = ColorToFloat4(level->FogColor);
					renderer->FogColor.rgb += (sky.rgb - renderer->FogColor.rgb) * a;
					renderer->FogColor.rgb *= renderer->FogColorMultiplier;
					Block block = Blocks.Table[LevelGetTile(level, player->Position.x, player->Position.y + 0.12, player->Position.z)];
					if (block != NULL && BlockGetLiquidType(block) != LiquidTypeNone)
					{
						if (BlockGetLiquidType(block) == LiquidTypeWater) { renderer->FogColor.rgb = (float3){ 0.02, 0.02, 0.2 }; }
						if (BlockGetLiquidType(block) == LiquidTypeLava) { renderer->FogColor.rgb = (float3){ 0.6, 0.0, 0.1 }; }
					}
					if (minecraft->Settings->Anaglyph)
					{
						renderer->FogColor.rgb = (float3){ renderer->FogColor.r * 30.0 + renderer->FogColor.g * 59.0 + renderer->FogColor.b * 11.0, renderer->FogColor.r * 30.0 + renderer->FogColor.g * 70.0, renderer->FogColor.r * 30.0 + renderer->FogColor.b * 70.0 } / 100.0;
					}
					
					glClearColor(renderer->FogColor.r, renderer->FogColor.g, renderer->FogColor.b, 0.0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					renderer->FogColorMultiplier = 1.0;
					glEnable(GL_CULL_FACE);
					renderer->FogEnd = (512 >> (minecraft->Settings->ViewDistance << 1));
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					if (minecraft->Settings->Anaglyph) { glTranslatef(-((i << 1) - 1) * 0.07, 0.0, 0.0); }
					
					gluPerspective(70.0, (float)minecraft->Width / (float)minecraft->Height, 0.05, renderer->FogEnd);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					if (minecraft->Settings->Anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
					if (minecraft->Settings->ViewBobbing) { RendererApplyBobbing(renderer, delta); }
					
					glTranslatef(0.0, 0.0, -0.1);
					float2 rot = player->OldRotation + (player->Rotation - player->OldRotation) * delta;
					glRotatef(rot.x, 1.0, 0.0, 0.0);
					glRotatef(rot.y, 0.0, 1.0, 0.0);
					float3 pos = player->OldPosition + (player->Position - player->OldPosition) * delta;
					glTranslatef(-pos.x, -pos.y, -pos.z);
					
					Frustum frustum = FrustumUpdate();
					LevelRenderer lrenderer = minecraft->LevelRenderer;
					for (int j = 0; j < lrenderer->ChunkCacheCount; j++) { ChunkClip(lrenderer->ChunkCache[j], frustum); }
					qsort(lrenderer->Chunks, ListCount(lrenderer->Chunks), sizeof(Chunk), ChunkVisibleDistanceComparator);
					
					int limit = ListCount(lrenderer->Chunks);
					if (limit > 10) { limit = 10; }
					for (int j = 0; j < limit; j++)
					{
						Chunk chunk = lrenderer->Chunks[0];
						lrenderer->Chunks = ListRemove(lrenderer->Chunks, 0);
						ChunkUpdate(chunk);
						chunk->Loaded = false;
					}
					
					RendererUpdateFog(renderer);
					glEnable(GL_FOG);
					LevelRendererSortChunks(lrenderer, player, 0);
					if (LevelIsSolidSearch(level, player->Position, 0.1))
					{
						int3 pos = int3f(player->Position);
						for (int x = pos.x - 1; x <= pos.x + 1; x++)
						{
							for (int y = pos.y - 1; y <= pos.y + 1; y++)
							{
								for (int z = pos.z - 1; z <= pos.z + 1; z++)
								{
									int3 v = { x, y, z };
									BlockType tile = LevelGetTile(level, x, y, z);
									if (tile != BlockTypeNone && BlockIsSolid(Blocks.Table[tile]))
									{
										glColor4f(0.2, 0.2, 0.2, 1.0);
										glDepthFunc(GL_LESS);
										ShapeRendererBegin();
										for (int j = 0; j < 6; j++) { BlockRenderInside(Blocks.Table[tile], v.x, v.y, v.z, j); }
										ShapeRendererEnd();
										glCullFace(GL_FRONT);
										ShapeRendererBegin();
										for (int j = 0; j < 6; j++) { BlockRenderInside(Blocks.Table[tile], v.x, v.y, v.z, j); }
										ShapeRendererEnd();
										glCullFace(GL_BACK);
										glDepthFunc(GL_LEQUAL);
									}
								}
							}
						}
					}
					
					ParticleManager particles = minecraft->ParticleManager;
					RendererSetLighting(renderer, false);
					RendererUpdateFog(renderer);
					float dt = delta;
					float c = -cos(player->Rotation.y * rad);
					float s = -sin(player->Rotation.y * rad);
					float ss = -s * sin(player->Rotation.x * rad);
					float cs = c * sin(player->Rotation.x * rad);
					float c2 = cos(player->Rotation.x * rad);
					for (int j = 0; j < 2; j++)
					{
						if (ListCount(particles->Particles[j]) != 0)
						{
							int tex = 0;
							if (j == 0) { tex = TextureManagerLoad(minecraft->TextureManager, "Particles.png"); }
							if (j == 1) { tex = TextureManagerLoad(minecraft->TextureManager, "Terrain.png"); }
							glBindTexture(GL_TEXTURE_2D, tex);
							ShapeRendererBegin();
							for (int k = 0; k < ListCount(particles->Particles[j]); k++)
							{
								ParticleRender(particles->Particles[j][k], dt, (float3){ c, c2, s }, ss, cs);
							}
							ShapeRendererEnd();
						}
					}
					
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Rock.png"));
					glEnable(GL_TEXTURE_2D);
					glCallList(lrenderer->ListID);
					RendererUpdateFog(renderer);
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Clouds.png"));
					glColor4f(1.0, 1.0, 1.0, 1.0);
					float4 cloud = ColorToFloat4(level->CloudColor);
					if (minecraft->Settings->Anaglyph)
					{
						cloud.rgb = (float3){ cloud.r * 30.0 + cloud.g * 59.0 + cloud.b * 11.0, cloud.r * 30.0 + cloud.g * 70.0, cloud.r * 30.0 + cloud.b * 70.0 } / 100.0;
					}
					float z = level->Depth + 2.0;
					float t = 0.03 * (lrenderer->Ticks + delta);
					ShapeRendererBegin();
					ShapeRendererColor(cloud.rgb);
					for (int x = -2048; x < level->Width + 2048; x += 512)
					{
						for (int y = -2048; y < level->Height + 2048; y += 512)
						{
							ShapeRendererVertexUV((float3){ x, z, y + 512 }, (float2){ x + t, y + 512 } / 2048.0);
							ShapeRendererVertexUV((float3){ x + 512, z, y + 512 }, (float2){ x + 512 + t, y + 512 } / 2048.0);
							ShapeRendererVertexUV((float3){ x + 512, z, y }, (float2){ x + 512 + t, y } / 2048.0);
							ShapeRendererVertexUV((float3){ x, z, y }, (float2){ x + t, y } / 2048.0);
							ShapeRendererVertexUV((float3){ x, z, y }, (float2){ x + t, y } / 2048.0);
							ShapeRendererVertexUV((float3){ x + 512, z, y }, (float2){ x + 512 + t, y } / 2048.0);
							ShapeRendererVertexUV((float3){ x + 512, z, y + 512 }, (float2){ x + 512 + t, y + 512 } / 2048.0);
							ShapeRendererVertexUV((float3){ x, z, y + 512 }, (float2){ x + t, y + 512 } / 2048.0);
						}
					}
					ShapeRendererEnd();
					
					glDisable(GL_TEXTURE_2D);
					ShapeRendererBegin();
					sky = ColorToFloat4(level->SkyColor);
					if (minecraft->Settings->Anaglyph)
					{
						sky.rgb = (float3){ sky.r * 30.0 + sky.g * 59.0 + sky.b * 11.0, sky.r * 30.0 + sky.g * 70.0, sky.r * 30.0 + sky.b * 70.0 } / 100.0;
					}
					ShapeRendererColor(sky.rgb);
					z = level->Depth + 10.0;
					for (int x = -2048; x < level->Width + 2048; x += 512)
					{
						for (int y = -2048; y < level->Height + 2048; y += 512)
						{
							ShapeRendererVertex((float3){ x, z, y });
							ShapeRendererVertex((float3){ x + 512, z, y });
							ShapeRendererVertex((float3){ x + 512, z, y + 512 });
							ShapeRendererVertex((float3){ x, z, y + 512 });
						}
					}
					ShapeRendererEnd();
					glEnable(GL_TEXTURE_2D);
					RendererUpdateFog(renderer);
					
					if (!minecraft->Selected.Null)
					{
						glDisable(GL_ALPHA_TEST);
						MovingObjectPosition pos = minecraft->Selected;
						glEnable(GL_BLEND);
						glEnable(GL_ALPHA_TEST);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						glColor4f(1.0, 1.0, 1.0, (sin(TimeMilli() / 100.0) * 0.2 + 0.4) * 0.5);
						if (lrenderer->Cracks > 0.0)
						{
							glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
							int tex = TextureManagerLoad(minecraft->TextureManager, "Terrain.png");
							glBindTexture(GL_TEXTURE_2D, tex);
							glColor4f(1.0, 1.0, 1.0, 0.5);
							glPushMatrix();
							Block block = Blocks.Table[LevelGetTile(level, pos.XYZ.x, pos.XYZ.y, pos.XYZ.z)];
							float3 v = (block->XYZ0 + block->XYZ1) / 2.0;
							glTranslatef(pos.XYZ.x + v.x, pos.XYZ.y + v.y, pos.XYZ.z + v.z);
							glScalef(1.01, 1.01, 1.01);
							glTranslatef(-(pos.XYZ.x + v.x), -(pos.XYZ.y + v.y), -(pos.XYZ.z + v.z));
							ShapeRendererBegin();
							ShapeRendererNoColor();
							glDepthMask(false);
							if (block == NULL) { block = Blocks.Table[BlockTypeStone]; }
							for (int j = 0; j < 6; j++) { BlockRenderSideWithTexture(block, pos.XYZ.x, pos.XYZ.y, pos.XYZ.z, j, 240 + (int)(lrenderer->Cracks * 10.0)); }
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
						BlockType tile = LevelGetTile(level, pos.XYZ.x, pos.XYZ.y, pos.XYZ.z);
						if (tile > BlockTypeNone)
						{
							AABB aabb = AABBGrow(BlockGetSelectionAABB(Blocks.Table[tile], pos.XYZ.x, pos.XYZ.y, pos.XYZ.z), one3f * 0.002);
							glBegin(GL_LINE_STRIP);
							glVertex3f(aabb.V0.x, aabb.V0.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V0.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V0.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V0.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V0.y, aabb.V0.z);
							glEnd();
							glBegin(GL_LINE_STRIP);
							glVertex3f(aabb.V0.x, aabb.V1.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V1.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V1.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V1.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V1.y, aabb.V0.z);
							glEnd();
							glBegin(GL_LINES);
							glVertex3f(aabb.V0.x, aabb.V0.y, aabb.V0.z);
							glVertex3f(aabb.V0.x, aabb.V1.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V0.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V1.y, aabb.V0.z);
							glVertex3f(aabb.V1.x, aabb.V0.y, aabb.V1.z);
							glVertex3f(aabb.V1.x, aabb.V1.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V0.y, aabb.V1.z);
							glVertex3f(aabb.V0.x, aabb.V1.y, aabb.V1.z);
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
					glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Water.png"));
					glCallList(lrenderer->ListID + 1);
					glDisable(GL_BLEND);
					glEnable(GL_BLEND);
					glColorMask(false, false, false, false);
					int count = LevelRendererSortChunks(lrenderer, player, 1);
					glColorMask(true, true, true, true);
					if (minecraft->Settings->Anaglyph)
					{
						if (count == 0) { glColorMask(false, true, true, false); }
						else { glColorMask(true, false, false, false); }
					}
					if (count > 0)
					{
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Terrain.png"));
						glCallLists(count, GL_INT, lrenderer->ChunkDataCache);
					}
					
					glDepthMask(true);
					glDisable(GL_BLEND);
					glDisable(GL_FOG);
					if (minecraft->Raining)
					{
						float t = delta;
						int3 p = int3f(player->Position);
						glDisable(GL_CULL_FACE);
						glNormal3f(0.0, 1.0, 0.0);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Rain.png"));
						for (int x = p.x - 5; x <= p.x + 5; x++)
						{
							for (int z = p.z - 5; z <= p.z + 5; z++)
							{
								int y = LevelGetHighestTile(level, x, z);
								int ymin = p.y - 5;
								int ymax = p.y + 5;
								if (ymin < y) { ymin = y; }
								if (ymax < y) { ymax = y; }
								if (ymin != ymax)
								{
									float tt = (((lrenderer->Ticks + x * 3121 + z * 418711) % 32) + t) / 32.0;
									float d = length2f((float2){ x, z } + 0.5 - player->Position.xz) / 5.0;
									glColor4f(1.0, 1.0, 1.0, (1.0 - d * d) * 0.7);
									ShapeRendererBegin();
									ShapeRendererVertexUV((float3){ x, ymin, z }, (float2){ 0.0, ymin / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x + 1, ymin, z + 1 }, (float2){ 2.0, ymin / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x + 1, ymax, z + 1 }, (float2){ 2.0, ymax / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x, ymax, z }, (float2){ 0.0, ymax / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x, ymin, z + 1 }, (float2){ 0.0, ymin / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x + 1, ymin, z }, (float2){ 2.0, ymin / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x + 1, ymax, z }, (float2){ 2.0, ymax / 4.0 + tt * 2.0 });
									ShapeRendererVertexUV((float3){ x, ymax, z + 1 }, (float2){ 0.0, ymax / 4.0 + tt * 2.0 });
									ShapeRendererEnd();
								}
							}
						}
						glEnable(GL_CULL_FACE);
						glDisable(GL_BLEND);
					}
					
					glClear(GL_DEPTH_BUFFER_BIT);
					glLoadIdentity();
					if (minecraft->Settings->Anaglyph) { glTranslatef(((i << 1) - 1) * 0.1, 0.0, 0.0); }
					if (minecraft->Settings->ViewBobbing) { RendererApplyBobbing(renderer, delta); }
					
					HeldBlock held = renderer->HeldBlock;
					float heldPos = held.LastPosition + (held.Position - held.LastPosition) * delta;
					glPushMatrix();
					glRotatef(rot.x, 1.0, 0.0, 0.0);
					glRotatef(rot.y, 0.0, 1.0, 0.0);
					RendererSetLighting(renderer, true);
					glPopMatrix();
					glPushMatrix();
					if (held.Moving)
					{
						float a = (held.Offset + delta) / 7.0;
						glTranslatef(-sin(sqrt(a) * pi) * 0.4, sin(sqrt(a) * pi * 2.0) * 0.2, -sin(a * pi) * 0.2);
					}
					glTranslatef(0.7 * 0.8, -0.65 * 0.8 - (1.0 - heldPos) * 0.6, -0.9 * 0.8);
					glRotatef(45.0, 0.0, 1.0, 0.0);
					glEnable(GL_NORMALIZE);
					if (held.Moving)
					{
						float a = (held.Offset + delta) / 7.0;
						glRotatef(sin(sqrt(a) * pi) * 80.0, 0.0, 1.0, 0.0);
						glRotatef(-sin(a * a * pi), 1.0, 0.0, 0.0);
					}
					float brightness = LevelGetBrightness(level, player->Position.x, player->Position.y, player->Position.z);
					glColor4f(brightness, brightness, brightness, 1.0);
					if (held.Block != NULL)
					{
						glScalef(0.4, 0.4, 0.4);
						glTranslatef(-0.5, -0.5, -0.5);
						glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(minecraft->TextureManager, "Terrain.png"));
						BlockRenderPreview(held.Block);
					}
					glDisable(GL_NORMALIZE);
					glPopMatrix();
					RendererSetLighting(renderer, false);
					
					if (!minecraft->Settings->Anaglyph) { break; }
				}
				
				HUDScreenRender(minecraft->HUD, delta, minecraft->CurrentScreen != NULL, (int2){ mx, my });
			}
			else
			{
				glViewport(0, 0, minecraft->FrameWidth, minecraft->FrameHeight);
				glClearColor(0.0, 0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				RendererEnableGUIMode(renderer);
			}
			
			if (minecraft->CurrentScreen != NULL) { GUIScreenRender(minecraft->CurrentScreen, (int2){ mx, my }); }
			
			SDL_GL_SwapWindow(minecraft->Window);
			
			CheckGLError(minecraft, "Post render");
			frame++;
			
			while (TimeMilli() >= start + 1000)
			{
				String chunks = StringConcat(StringCreateFromInt(ChunkUpdates), " chunk updates");
				minecraft->Debug = StringConcat(StringConcat(StringSetFromInt(minecraft->Debug, frame), " fps, "), chunks);
				StringDestroy(chunks);
				start += 1000;
				frame = 0;
				ChunkUpdates = 0;
			}
		}
	}
	MinecraftShutdown(minecraft);
}

void MinecraftGrabMouse(Minecraft minecraft)
{
	if (!minecraft->HasMouse)
	{
		minecraft->HasMouse = true;
		if (minecraft->LevelLoaded)
		{
			SDL_ShowCursor(false);
			SDL_WarpMouseInWindow(minecraft->Window, minecraft->Width / 2, minecraft->Height / 2);
		}
		else { SDL_SetWindowGrab(minecraft->Window, true); }
		MinecraftSetCurrentScreen(minecraft, NULL);
		minecraft->LastClick = minecraft->Ticks + 10000;
	}
}

void MinecraftPause(Minecraft minecraft)
{
	if (minecraft->CurrentScreen == NULL)
	{
		MinecraftSetCurrentScreen(minecraft, PauseScreenCreate());
	}
}

bool MinecraftIsOnline(Minecraft minecraft)
{
	return false;
}

void MinecraftGenerateLevel(Minecraft minecraft, int size)
{
	char * user = "anonymous";
	LevelGenerator generator = LevelGeneratorCreate(minecraft->ProgressBar);
	Level level = LevelGeneratorGenerate(generator, user, 128 << size, 128 << size, 64);
	MinecraftSetLevel(minecraft, level);
	LevelGeneratorDestroy(generator);
}

bool MinecraftLoadOnlineLevel(Minecraft minecraft, char * username, int userID)
{
	Level level = LevelIOLoadOnline(minecraft->LevelIO, minecraft->Host, username, userID);
	if (level == NULL) { return false; }
	else
	{
		MinecraftSetLevel(minecraft, level);
		return true;
	}
}

void MinecraftSetLevel(Minecraft minecraft, Level level)
{
	if (minecraft->Level != NULL) { LevelDestroy(minecraft->Level); }
	minecraft->Level = level;
	if (level != NULL)
	{
		level->Font = minecraft->Font;
		level->Minecraft = minecraft;
		if (minecraft->Player != NULL) { EntityDestroy(minecraft->Player); }
		minecraft->Player = LevelFindPlayer(level);
	}
	
	if (minecraft->Player == NULL)
	{
		minecraft->Player = PlayerCreate(level);
		EntityResetPosition(minecraft->Player);
		if (level != NULL) { level->Player = minecraft->Player; }
	}
	
	PlayerData player = minecraft->Player->TypeData;
	player->Input = InputHandlerCreate(minecraft->Settings);
	for (int i = 0; i < 9; i++)
	{
		player->Inventory->Count[i] = 1;
		if (player->Inventory->Slots[i] == -1) { player->Inventory->Slots[i] = SessionDataAllowedBlocks[i]->Type; }
	}
	
	if (minecraft->LevelRenderer != NULL)
	{
		LevelRendererDestroy(minecraft->LevelRenderer);
		minecraft->LevelRenderer = LevelRendererCreate(minecraft, minecraft->TextureManager);
		minecraft->LevelRenderer->Level = level;
		if (level != NULL)
		{
			LevelAddRenderer(level, minecraft->LevelRenderer);
			LevelRendererRefresh(minecraft->LevelRenderer);
		}
	}
	
	if (minecraft->ParticleManager != NULL)
	{
		if (level != NULL) { level->ParticleEngine = minecraft->ParticleManager; }
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < ListCount(minecraft->ParticleManager->Particles[i]); j++) { ParticleDestroy(minecraft->ParticleManager->Particles[i][j]); }
			minecraft->ParticleManager->Particles[i] = ListClear(minecraft->ParticleManager->Particles[i]);
		}
	}
}

void MinecraftDestroy(Minecraft minecraft)
{
	TimerDestroy(minecraft->Timer);
	ProgressBarDisplayDestroy(minecraft->ProgressBar);
	RendererDestroy(minecraft->Renderer);
	LevelIODestroy(minecraft->LevelIO);
	StringDestroy(minecraft->Debug);
	MemoryFree(minecraft);
}

int main(int argc, char * argv[])
{
	Minecraft minecraft = MinecraftCreate(NULL, 860, 480, false);
	MinecraftRun(minecraft);
	return 0;
}
