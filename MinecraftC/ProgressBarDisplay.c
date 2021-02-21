#include <OpenGL.h>
#include "ProgressBarDisplay.h"
#include "Minecraft.h"
#include "Utilities/Time.h"
#include "Utilities/Log.h"
#include "Render/ShapeRenderer.h"

ProgressBarDisplay ProgressBarDisplayCreate(Minecraft minecraft)
{
	ProgressBarDisplay display = MemoryAllocate(sizeof(struct ProgressBarDisplay));
	*display = (struct ProgressBarDisplay)
	{
		.Text = "",
		.Minecraft = minecraft,
		.Title = "",
		.Start = TimeMilli(),
	};
	return display;
}

void ProgressBarDisplaySetTitle(ProgressBarDisplay display, char * title)
{
	if (!display->Minecraft->Running) { LogFatal("\n"); }
	
	display->Title = title;
	int a1 = display->Minecraft->Width * 240 / display->Minecraft->Height;
	int a2 = display->Minecraft->Height * 240 / display->Minecraft->Height;
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, a1, a2, 0.0, 100.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -200.0);
}

void ProgressBarDisplaySetText(ProgressBarDisplay display, char * text)
{
	if (!display->Minecraft->Running) { LogFatal("\n"); }
	
	display->Text = text;
	ProgressBarDisplaySetProgress(display, -1);
}

void ProgressBarDisplaySetProgress(ProgressBarDisplay display, int progress)
{
	if (!display->Minecraft->Running) { LogFatal("\n"); }
	
	long time = TimeMilli();
	if (time - display->Start < 0 || time - display->Start >= 20)
	{
		display->Start = time;
		int a1 = display->Minecraft->Width * 240 / display->Minecraft->Height;
		int a2 = display->Minecraft->Height * 240 / display->Minecraft->Height;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(display->Minecraft->TextureManager, "Dirt.png"));
		ShapeRendererBegin();
		ShapeRendererColor(ColorToFloat4(ColorFromHex(0x404040ff)).rgb);
		ShapeRendererVertexUV((float3){ 0.0, a2, 0.0 }, (float2){ 0.0, a2 / 32.0 });
		ShapeRendererVertexUV((float3){ a1, a2, 0.0 }, (float2){ a1 / 32.0, a2 / 32.0 });
		ShapeRendererVertexUV((float3){ a1, 0.0, 0.0 }, (float2){ a1 / 32.0, 0.0 });
		ShapeRendererVertexUV((float3){ 0.0, 0.0, 0.0 }, (float2){ 0.0, 0.0 });
		ShapeRendererEnd();
		if (progress >= 0)
		{
			int b1 = a1 / 2 - 50;
			int b2 = a2 / 2 + 16;
			glDisable(GL_TEXTURE_2D);
			ShapeRendererBegin();
			ShapeRendererColor(ColorToFloat4(ColorFromHex(0x808080ff)).rgb);
			ShapeRendererVertex((float3){ b1, b2, 0.0 });
			ShapeRendererVertex((float3){ b1, b2 + 2, 0.0 });
			ShapeRendererVertex((float3){ b1 + 100, b2 + 2, 0.0 });
			ShapeRendererVertex((float3){ b1 + 100, b2, 0.0 });
			ShapeRendererColor(ColorToFloat4(ColorFromHex(0x80ff80ff)).rgb);
			ShapeRendererVertex((float3){ b1, b2, 0.0 });
			ShapeRendererVertex((float3){ b1, b2 + 2, 0.0 });
			ShapeRendererVertex((float3){ b1 + progress, b2 + 2, 0.0 });
			ShapeRendererVertex((float3){ b1 + progress, b2, 0.0 });
			ShapeRendererEnd();
			glEnable(GL_TEXTURE_2D);
		}
		
		FontRendererRender(display->Minecraft->Font, display->Title, (a1 - FontRendererGetWidth(display->Minecraft->Font, display->Title)) / 2, a2 / 2 - 4 - 16, ColorWhite);
		FontRendererRender(display->Minecraft->Font, display->Text, (a1 - FontRendererGetWidth(display->Minecraft->Font, display->Text)) / 2, a2 / 2 - 4 + 8, ColorWhite);
		
		while (SDL_PollEvent(&(SDL_Event){ 0 }));
		SDL_GL_SwapWindow(display->Minecraft->Window);
	}
}

void ProgressBarDisplayDestroy(ProgressBarDisplay display)
{
	MemoryFree(display);
}
