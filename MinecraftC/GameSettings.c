#include <string.h>
#include "GameSettings.h"
#include "Minecraft.h"
#include "Utilities/Log.h"

static void Load(GameSettings * settings) {
	SDL_RWops * file = SDL_RWFromFile(settings->file, "r");
	if (file == NULL) {
		return;
	}
	
	int size = (int)SDL_RWsize(file);
	char * text = malloc(size);
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
			for (int i = 0; i < ListLength(settings->bindings); i++) {
				String keyName = StringCreate(settings->bindings[i]->name);
				StringConcatFront("key_", &keyName);
				if (strcmp(line, keyName) == 0) { settings->bindings[i]->key = StringToInt(value); }
				StringFree(keyName);
			}
			StringFree(value);
			StringFree(line);
		}
	}
	free(text);
	SDL_RWclose(file);
}

static void Save(GameSettings * settings) {
	SDL_RWops * file = SDL_RWFromFile(settings->file, "w");
	if (file == NULL) {
		LogWarning("Failed to save options: %s\n", SDL_GetError());
		return;
	}
	
	String line = StringCreate("");
	StringSet(&line, settings->music ? "true\n" : "false\n");
	StringConcatFront("music:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->sound ? "true\n" : "false\n");
	StringConcatFront("sound:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->invertMouse ? "true\n" : "false\n");
	StringConcatFront("invertYMouse:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->invertMouse ? "true\n" : "false\n");
	StringConcatFront("invertYMouse:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->showFrameRate ? "true\n" : "false\n");
	StringConcatFront("showFrameRate:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSetFromInt(&line, settings->viewDistance);
	StringConcatFront("viewDistance:", &line);
	StringConcat(&line, "\n");
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->viewBobbing ? "true\n" : "false\n");
	StringConcatFront("bobView:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->anaglyph ? "true\n" : "false\n");
	StringConcatFront("anaglyph3d:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	StringSet(&line, settings->limitFramerate ? "true\n" : "false\n");
	StringConcatFront("limitFramerate:", &line);
	SDL_RWwrite(file, line, StringLength(line), 1);
	
	for (int i = 0; i < ListLength(settings->bindings); i++) {
		String keyName = StringCreate(settings->bindings[i]->name);
		StringConcatFront("key_", &keyName);
		StringConcat(&keyName, ":");
		
		StringSetFromInt(&line, settings->bindings[i]->key);
		StringConcatFront(keyName, &line);
		StringConcat(&line, "\n");
		SDL_RWwrite(file, line, StringLength(line), 1);
		StringFree(keyName);
	}
	StringFree(line);
	SDL_RWclose(file);
}

void GameSettingsCreate(GameSettings * settings, Minecraft * minecraft) {
	*settings = (GameSettings) {
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
	};
	settings->file = StringCreate(minecraft->workingDirectory);
	StringConcat(&settings->file, "Options.txt");
	KeyBinding * bindings[] = { &settings->forwardKey, &settings->leftKey, &settings->backKey, &settings->rightKey, &settings->jumpKey, &settings->buildKey, &settings->chatKey, &settings->toggleFogKey, &settings->saveLocationKey, &settings->loadLocationKey };
	for (int i = 0; i < sizeof(bindings) / sizeof(bindings[0]); i++) { settings->bindings = ListPush(settings->bindings, &bindings[i]); }
	Load(settings);
}

String GameSettingsGetBinding(GameSettings * settings, int binding) {
	String string = StringCreate(settings->bindings[binding]->name);
	StringConcat(&string, ": ");
	StringConcat(&string, (char *)SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(settings->bindings[binding]->key)));
	return string;
}

void GameSettingsSetBinding(GameSettings * settings, int binding, int key) {
	settings->bindings[binding]->key = key;
	Save(settings);
}

void GameSettingsToggleSetting(GameSettings * settings, int setting) {
	if (setting == 0) { settings->music = !settings->music; }
	if (setting == 1) { settings->sound = !settings->sound; }
	if (setting == 2) { settings->invertMouse = !settings->invertMouse; }
	if (setting == 3) { settings->showFrameRate = !settings->showFrameRate; }
	if (setting == 4) { settings->viewDistance = (settings->viewDistance + 1) % 4; }
	if (setting == 5) { settings->viewBobbing = !settings->viewBobbing; }
	if (setting == 6) {
		settings->anaglyph = !settings->anaglyph;
		TextureManagerReload(&settings->minecraft->textureManager);
		settings->minecraft->font.texture = TextureManagerLoad(&settings->minecraft->textureManager, settings->minecraft->font.textureName);
	}
	if (setting == 7) {
		settings->limitFramerate = !settings->limitFramerate;
		SDL_GL_SetSwapInterval(settings->limitFramerate ? 1 : 0);
	}
	Save(settings);
}

static char * RenderDistances[] = { "FAR", "NORMAL", "SHORT", "TINY" };

String GameSettingsGetSetting(GameSettings * settings, int setting) {
	String string;
	switch (setting) {
		case 0:
			string = StringCreate("Music: ");
			StringConcat(&string, settings->music ? "ON" : "OFF");
			break;
		case 1:
			string = StringCreate("Sound: ");
			StringConcat(&string, settings->sound ? "ON" : "OFF");
			break;
		case 2:
			string = StringCreate("Invert mouse: ");
			StringConcat(&string, settings->invertMouse ? "ON" : "OFF");
			break;
		case 3:
			string = StringCreate("Show FPS: ");
			StringConcat(&string, settings->showFrameRate ? "ON" : "OFF");
			break;
		case 4:
			string = StringCreate("Render distance: ");
			StringConcat(&string, RenderDistances[settings->viewDistance]);
			break;
		case 5:
			string = StringCreate("View bobbing: ");
			StringConcat(&string, settings->viewBobbing ? "ON" : "OFF");
			break;
		case 6:
			string = StringCreate("3d anaglyph: ");
			StringConcat(&string, settings->anaglyph ? "ON" : "OFF");
			break;
		case 7:
			string = StringCreate("Limit framerate: ");
			StringConcat(&string, settings->limitFramerate ? "ON" : "OFF");
			break;
		default:
			string = StringCreate("Error");
			break;
	}
	return string;
}

void GameSettingsDestroy(GameSettings * settings) {
	StringFree(settings->file);
	ListFree(settings->bindings);
	free(settings);
}
