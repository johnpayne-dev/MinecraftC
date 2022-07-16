#include <string.h>
#include "GameSettings.h"
#include "Minecraft.h"
#include "KeyBinding.h"
#include "Utilities/Log.h"

static void Load(GameSettings settings) {
	SDL_RWops * file = SDL_RWFromFile(settings->file, "r");
	if (file == NULL) {
		LogWarning("Failed to load options: %s\n", SDL_GetError());
		return;
	}
	
	int size = (int)SDL_RWsize(file);
	char * text = MemoryAllocate(size);
	SDL_RWread(file, text, size, 1);
	for (int i = 0, j = -1; i < size; i++) {
		if (text[i] == '\n') {
			String line = StringSub(StringCreate(text), j + 1, i);
			j = i;
			int split = StringIndexOf(line, ':');
			if (split <= 0) { continue; }
			String value = StringSub(StringCreate(line), split + 1, StringLength(line));
			line = StringSub(line, 0, split);
			if (strcmp(line, "music") == 0) { settings->music = strcmp(value, "true") == 0; }
			if (strcmp(line, "sound") == 0) { settings->sound = strcmp(value, "true") == 0; }
			if (strcmp(line, "invertYMouse") == 0) { settings->invertMouse = strcmp(value, "true") == 0; }
			if (strcmp(line, "showFrameRate") == 0) { settings->showFrameRate = strcmp(value, "true") == 0; }
			if (strcmp(line, "viewDistance") == 0) { settings->viewDistance = StringToInt(value); }
			if (strcmp(line, "bobView") == 0) { settings->viewBobbing = strcmp(value, "true") == 0; }
			if (strcmp(line, "anaglyph3d") == 0) { settings->anaglyph = strcmp(value, "true") == 0; }
			if (strcmp(line, "limitFramerate") == 0) { settings->limitFramerate = strcmp(value, "true") == 0; }
			for (int i = 0; i < ListCount(settings->bindings); i++) {
				String keyName = StringConcatFront("key_", StringCreate(settings->bindings[i]->name));
				if (strcmp(line, keyName) == 0) { settings->bindings[i]->key = StringToInt(value); }
				StringDestroy(keyName);
			}
			StringDestroy(value);
			StringDestroy(line);
		}
	}
	MemoryFree(text);
	SDL_RWclose(file);
}

static void Save(GameSettings settings) {
	SDL_RWops * file = SDL_RWFromFile(settings->file, "w");
	if (file == NULL) {
		LogWarning("Failed to save options: %s\n", SDL_GetError());
		return;
	}
	
	String line = StringCreate("");
	line = StringConcatFront("music:", StringSet(line, settings->music ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("sound:", StringSet(line, settings->sound ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("invertYMouse:", StringSet(line, settings->invertMouse ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("invertYMouse:", StringSet(line, settings->invertMouse ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("showFrameRate:", StringSet(line, settings->showFrameRate ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcat(StringConcatFront("viewDistance:", StringSetFromInt(line, settings->viewDistance)), "\n");
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("bobView:", StringSet(line, settings->viewBobbing ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("anaglyph3d:", StringSet(line, settings->anaglyph ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	line = StringConcatFront("limitFramerate:", StringSet(line, settings->limitFramerate ? "true\n" : "false\n"));
	SDL_RWwrite(file, line, StringLength(line), 1);
	for (int i = 0; i < ListCount(settings->bindings); i++) {
		String keyName = StringConcat(StringConcatFront("key_", StringCreate(settings->bindings[i]->name)), ":");
		line = StringConcat(StringConcatFront(keyName, StringSetFromInt(line, settings->bindings[i]->key)), "\n");
		SDL_RWwrite(file, line, StringLength(line), 1);
		StringDestroy(keyName);
	}
	StringDestroy(line);
	SDL_RWclose(file);
}

GameSettings GameSettingsCreate(Minecraft minecraft) {
	GameSettings settings = MemoryAllocate(sizeof(struct GameSettings));
	*settings = (struct GameSettings) {
		.music = true,
		.sound = true,
		.invertMouse = false,
		.showFrameRate = true,
		.viewDistance = 0,
		.viewBobbing = true,
		.anaglyph = false,
		.limitFramerate = false,
		.forwardKey = (KeyBinding){ .name = "Forward", .key = SDL_SCANCODE_W },
		.leftKey = (KeyBinding){ .name = "Left", .key = SDL_SCANCODE_A },
		.backKey = (KeyBinding){ .name = "Back", .key = SDL_SCANCODE_S },
		.rightKey = (KeyBinding){ .name = "Right", .key = SDL_SCANCODE_D },
		.jumpKey = (KeyBinding){ .name = "Jump", .key = SDL_SCANCODE_SPACE },
		.buildKey = (KeyBinding){ .name = "Build", .key = SDL_SCANCODE_B },
		.chatKey = (KeyBinding){ .name = "Chat", .key = SDL_SCANCODE_T },
		.toggleFogKey = (KeyBinding){ .name = "Toggle fog", .key = SDL_SCANCODE_F },
		.saveLocationKey = (KeyBinding){ .name = "Save location", .key = SDL_SCANCODE_RETURN },
		.loadLocationKey = (KeyBinding){ .name = "Load location", .key = SDL_SCANCODE_R },
		.bindings = ListCreate(sizeof(KeyBinding *)),
		.settingsCount = 8,
		.minecraft = minecraft,
		.file = StringConcat(StringCreate(minecraft->workingDirectory), "Options.txt"),
	};
	KeyBinding * bindings[] = { &settings->forwardKey, &settings->leftKey, &settings->backKey, &settings->rightKey, &settings->jumpKey, &settings->buildKey, &settings->chatKey, &settings->toggleFogKey, &settings->saveLocationKey, &settings->loadLocationKey };
	for (int i = 0; i < sizeof(bindings) / sizeof(bindings[0]); i++) { settings->bindings = ListPush(settings->bindings, &bindings[i]); }
	Load(settings);
	return settings;
}

String GameSettingsGetBinding(GameSettings settings, int binding) {
	return StringConcat(StringConcat(StringCreate(settings->bindings[binding]->name), ": "), (char *)SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(settings->bindings[binding]->key)));
}

void GameSettingsSetBinding(GameSettings settings, int binding, int key) {
	settings->bindings[binding]->key = key;
	Save(settings);
}

void GameSettingsToggleSetting(GameSettings settings, int setting, int var2) {
	if (setting == 0) { settings->music = !settings->music; }
	if (setting == 1) { settings->sound = !settings->sound; }
	if (setting == 2) { settings->invertMouse = !settings->invertMouse; }
	if (setting == 3) { settings->showFrameRate = !settings->showFrameRate; }
	if (setting == 4) { settings->viewDistance = (settings->viewDistance + 1) % 4; }
	if (setting == 5) { settings->viewBobbing = !settings->viewBobbing; }
	if (setting == 6) {
		settings->anaglyph = !settings->anaglyph;
		TextureManagerReload(settings->minecraft->textureManager);
		settings->minecraft->font->texture = TextureManagerLoad(settings->minecraft->textureManager, settings->minecraft->font->textureName);
	}
	if (setting == 7) {
		settings->limitFramerate = !settings->limitFramerate;
		SDL_GL_SetSwapInterval(settings->limitFramerate ? 1 : 0);
	}
	Save(settings);
}

static char * RenderDistances[] = { "FAR", "NORMAL", "SHORT", "TINY" };

String GameSettingsGetSetting(GameSettings settings, int setting) {
	switch (setting) {
		case 0: return StringConcat(StringCreate("Music: "), settings->music ? "ON" : "OFF");
		case 1: return StringConcat(StringCreate("Sound: "), settings->sound ? "ON" : "OFF");
		case 2: return StringConcat(StringCreate("Invert mouse: "), settings->invertMouse ? "ON" : "OFF");
		case 3: return StringConcat(StringCreate("Show FPS: "), settings->showFrameRate ? "ON" : "OFF");
		case 4: return StringConcat(StringCreate("Render distance: "), RenderDistances[settings->viewDistance]);
		case 5: return StringConcat(StringCreate("View bobbing: "), settings->viewBobbing ? "ON" : "OFF");
		case 6: return StringConcat(StringCreate("3d anaglyph: "), settings->anaglyph ? "ON" : "OFF");
		case 7: return StringConcat(StringCreate("Limit framerate: "), settings->limitFramerate ? "ON" : "OFF");
		default: return StringCreate("Error");
	}
}

void GameSettingsDestroy(GameSettings settings) {
	StringDestroy(settings->file);
	ListDestroy(settings->bindings);
	MemoryFree(settings);
}
