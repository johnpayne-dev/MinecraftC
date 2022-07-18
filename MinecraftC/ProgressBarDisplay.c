#include "ProgressBarDisplay.h"
#include "Minecraft.h"
#include "Utilities/Time.h"
#include "Utilities/Log.h"
#include "Utilities/OpenGL.h"
#include "Render/ShapeRenderer.h"

ProgressBarDisplay ProgressBarDisplayCreate(Minecraft minecraft) {
	ProgressBarDisplay display = malloc(sizeof(struct ProgressBarDisplay));
	*display = (struct ProgressBarDisplay) {
		.text = "",
		.minecraft = minecraft,
		.title = "",
		.start = TimeMilli(),
	};
	return display;
}

void ProgressBarDisplaySetTitle(ProgressBarDisplay display, char * title) {
	if (!display->minecraft->running) { LogFatal("\n"); }
	
	display->title = title;
	int a1 = display->minecraft->width * 240 / display->minecraft->height;
	int a2 = display->minecraft->height * 240 / display->minecraft->height;
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, a1, a2, 0.0, 100.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -200.0);
}

void ProgressBarDisplaySetText(ProgressBarDisplay display, char * text) {
	if (!display->minecraft->running) { LogFatal("\n"); }
	
	display->text = text;
	ProgressBarDisplaySetProgress(display, -1);
}

void ProgressBarDisplaySetProgress(ProgressBarDisplay display, int progress) {
	if (!display->minecraft->running) { LogFatal("\n"); }
	
	int64_t time = TimeMilli();
	if (time - display->start < 0 || time - display->start >= 20) {
		display->start = time;
		int a1 = display->minecraft->width * 240 / display->minecraft->height;
		int a2 = display->minecraft->height * 240 / display->minecraft->height;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(display->minecraft->textureManager, "Dirt.png"));
		ShapeRendererBegin();
		ShapeRendererColor(0x404040ff);
		ShapeRendererVertexUV(0.0, a2, 0.0, 0.0, a2 / 32.0);
		ShapeRendererVertexUV(a1, a2, 0.0, a1 / 32.0, a2 / 32.0);
		ShapeRendererVertexUV(a1, 0.0, 0.0, a1 / 32.0, 0.0);
		ShapeRendererVertexUV(0.0, 0.0, 0.0, 0.0, 0.0);
		ShapeRendererEnd();
		if (progress >= 0) {
			int b1 = a1 / 2 - 50;
			int b2 = a2 / 2 + 16;
			glDisable(GL_TEXTURE_2D);
			ShapeRendererBegin();
			ShapeRendererColor(0x808080ff);
			ShapeRendererVertex(b1, b2, 0.0);
			ShapeRendererVertex(b1, b2 + 2, 0.0);
			ShapeRendererVertex(b1 + 100, b2 + 2, 0.0);
			ShapeRendererVertex(b1 + 100, b2, 0.0);
			ShapeRendererColor(0x80ff80ff);
			ShapeRendererVertex(b1, b2, 0.0);
			ShapeRendererVertex(b1, b2 + 2, 0.0);
			ShapeRendererVertex(b1 + progress, b2 + 2, 0.0);
			ShapeRendererVertex(b1 + progress, b2, 0.0);
			ShapeRendererEnd();
			glEnable(GL_TEXTURE_2D);
		}
		
		FontRendererRender(display->minecraft->font, display->title, (a1 - FontRendererGetWidth(display->minecraft->font, display->title)) / 2, a2 / 2 - 4 - 16, 0xffffffff);
		FontRendererRender(display->minecraft->font, display->text, (a1 - FontRendererGetWidth(display->minecraft->font, display->text)) / 2, a2 / 2 - 4 + 8, 0xffffffff);
		
		while (SDL_PollEvent(&(SDL_Event){ 0 }));
		SDL_GL_SwapWindow(display->minecraft->window);
	}
}

void ProgressBarDisplayDestroy(ProgressBarDisplay display) {
	free(display);
}
