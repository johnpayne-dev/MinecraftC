#include "GameSettings.h"
#include "Minecraft.h"
#include "KeyBinding.h"
#include "Utilities/Log.h"

static void Load(GameSettings settings)
{
	SDL_RWops * file = SDL_RWFromFile(settings->File, "r");
	if (file == NULL)
	{
		LogWarning("Failed to load options: %s\n", SDL_GetError());
		return;
	}
	
	int size = (int)SDL_RWsize(file);
	char * text = MemoryAllocate(size);
	SDL_RWread(file, text, size, 1);
	for (int i = 0, j = -1; i < size; i++)
	{
		if (text[i] == '\n')
		{
			String line = StringSub(StringCreate(text), j + 1, i);
			j = i;
			int split = StringIndexOf(line, ':');
			if (split <= 0) { continue; }
			String value = StringSub(StringCreate(line), split + 1, StringLength(line));
			line = StringSub(line, 0, split);
			if (strcmp(line, "music") == 0) { settings->Music = strcmp(value, "true") == 0; }
			if (strcmp(line, "sound") == 0) { settings->Sound = strcmp(value, "true") == 0; }
			if (strcmp(line, "invertYMouse") == 0) { settings->InvertMouse = strcmp(value, "true") == 0; }
			if (strcmp(line, "showFrameRate") == 0) { settings->ShowFrameRate = strcmp(value, "true") == 0; }
			if (strcmp(line, "viewDistance") == 0) { settings->ViewDistance = StringToInt(value); }
			if (strcmp(line, "bobView") == 0) { settings->ViewBobbing = strcmp(value, "true") == 0; }
			if (strcmp(line, "anaglyph3d") == 0) { settings->Anaglyph = strcmp(value, "true") == 0; }
			if (strcmp(line, "limitFramerate") == 0) { settings->LimitFramerate = strcmp(value, "true") == 0; }
			for (int i = 0; i < ListCount(settings->Bindings); i++)
			{
				String keyName = StringConcatFront("key_", StringCreate(settings->Bindings[i]->Name));
				if (strcmp(line, keyName) == 0) { settings->Bindings[i]->Key = StringToInt(value); }
				StringDestroy(keyName);
			}
			StringDestroy(value);
			StringDestroy(line);
		}
	}
	MemoryFree(text);
	SDL_RWclose(file);
}

static void Save(GameSettings settings)
{
	SDL_RWops * file = SDL_RWFromFile(settings->File, "w");
	if (file == NULL)
	{
		LogWarning("Failed to save options: %s\n", SDL_GetError());
		return;
	}
	
	String line = StringCreate("");
	line = StringConcatFront("music:", StringSet(line, settings->Music ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("sound:", StringSet(line, settings->Sound ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("invertYMouse:", StringSet(line, settings->InvertMouse ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("invertYMouse:", StringSet(line, settings->InvertMouse ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("showFrameRate:", StringSet(line, settings->ShowFrameRate ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcat(StringConcatFront("viewDistance:", StringSetFromInt(line, settings->ViewDistance)), "\n");
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("bobView:", StringSet(line, settings->ViewBobbing ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("anaglyph3d:", StringSet(line, settings->Anaglyph ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("limitFramerate:", StringSet(line, settings->LimitFramerate ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	for (int i = 0; i < ListCount(settings->Bindings); i++)
	{
		String keyName = StringConcat(StringConcatFront("key_", StringCreate(settings->Bindings[i]->Name)), ":");
		line = StringConcat(StringConcatFront(keyName, StringSetFromInt(line, settings->Bindings[i]->Key)), "\n");
		SDL_RWwrite(file, line, StringLength(line), 1);
		StringDestroy(keyName);
	}
	StringDestroy(line);
	SDL_RWclose(file);
}

GameSettings GameSettingsCreate(Minecraft minecraft)
{
	GameSettings settings = MemoryAllocate(sizeof(struct GameSettings));
	*settings = (struct GameSettings)
	{
		.Music = true,
		.Sound = true,
		.InvertMouse = false,
		.ShowFrameRate = true,
		.ViewDistance = 0,
		.ViewBobbing = true,
		.Anaglyph = false,
		.LimitFramerate = false,
		.ForwardKey = (KeyBinding){ .Name = "Forward", .Key = SDL_SCANCODE_W },
		.LeftKey = (KeyBinding){ .Name = "Left", .Key = SDL_SCANCODE_A },
		.BackKey = (KeyBinding){ .Name = "Back", .Key = SDL_SCANCODE_S },
		.RightKey = (KeyBinding){ .Name = "Right", .Key = SDL_SCANCODE_D },
		.JumpKey = (KeyBinding){ .Name = "Jump", .Key = SDL_SCANCODE_SPACE },
		.BuildKey = (KeyBinding){ .Name = "Build", .Key = SDL_SCANCODE_B },
		.ChatKey = (KeyBinding){ .Name = "Chat", .Key = SDL_SCANCODE_T },
		.ToggleFogKey = (KeyBinding){ .Name = "Toggle fog", .Key = SDL_SCANCODE_F },
		.SaveLocationKey = (KeyBinding){ .Name = "Save location", .Key = SDL_SCANCODE_RETURN },
		.LoadLocationKey = (KeyBinding){ .Name = "Load location", .Key = SDL_SCANCODE_R },
		.Bindings = ListCreate(sizeof(KeyBinding *)),
		.SettingsCount = 8,
		.Minecraft = minecraft,
		.File = StringConcat(StringCreate(minecraft->WorkingDirectory), "Options.txt"),
	};
	KeyBinding * bindings[] = { &settings->ForwardKey, &settings->LeftKey, &settings->BackKey, &settings->RightKey, &settings->JumpKey, &settings->BuildKey, &settings->ChatKey, &settings->ToggleFogKey, &settings->SaveLocationKey, &settings->LoadLocationKey };
	for (int i = 0; i < sizeof(bindings) / sizeof(bindings[0]); i++) { settings->Bindings = ListPush(settings->Bindings, &bindings[i]); }
	Load(settings);
	return settings;
}

String GameSettingsGetBinding(GameSettings settings, int binding)
{
	return StringConcat(StringConcat(StringCreate(settings->Bindings[binding]->Name), ": "), (char *)SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(settings->Bindings[binding]->Key)));
}

void GameSettingsSetBinding(GameSettings settings, int binding, int key)
{
	settings->Bindings[binding]->Key = key;
	Save(settings);
}

void GameSettingsToggleSetting(GameSettings settings, int setting, int var2)
{
	if (setting == 0) { settings->Music = !settings->Music; }
	if (setting == 1) { settings->Sound = !settings->Sound; }
	if (setting == 2) { settings->InvertMouse = !settings->InvertMouse; }
	if (setting == 3) { settings->ShowFrameRate = !settings->ShowFrameRate; }
	if (setting == 4) { settings->ViewDistance = (settings->ViewDistance + 1) % 4; }
	if (setting == 5) { settings->ViewBobbing = !settings->ViewBobbing; }
	if (setting == 6)
	{
		settings->Anaglyph = !settings->Anaglyph;
		
	}
	if (setting == 7)
	{
		settings->LimitFramerate = !settings->LimitFramerate;
		SDL_GL_SetSwapInterval(settings->LimitFramerate ? 1 : 0);
	}
	Save(settings);
}

static char * RenderDistances[] = { "FAR", "NORMAL", "SHORT", "TINY" };

String GameSettingsGetSetting(GameSettings settings, int setting)
{
	switch (setting)
	{
		case 0: return StringConcat(StringCreate("Music: "), settings->Music ? "ON" : "OFF");
		case 1: return StringConcat(StringCreate("Sound: "), settings->Sound ? "ON" : "OFF");
		case 2: return StringConcat(StringCreate("Invert mouse: "), settings->InvertMouse ? "ON" : "OFF");
		case 3: return StringConcat(StringCreate("Show FPS: "), settings->ShowFrameRate ? "ON" : "OFF");
		case 4: return StringConcat(StringCreate("Render distance: "), RenderDistances[settings->ViewDistance]);
		case 5: return StringConcat(StringCreate("View bobbing: "), settings->ViewBobbing ? "ON" : "OFF");
		case 6: return StringConcat(StringCreate("3d anaglyph: "), settings->Anaglyph ? "ON" : "OFF");
		case 7: return StringConcat(StringCreate("Limit framerate: "), settings->LimitFramerate ? "ON" : "OFF");
		default: return StringCreate("Error");
	}
}

void GameSettingsDestroy(GameSettings settings)
{
	StringDestroy(settings->File);
	ListDestroy(settings->Bindings);
	MemoryFree(settings);
}
