#pragma once
#include <stdint.h>

typedef struct ProgressBarDisplay
{
	struct Minecraft * Minecraft;
	char * Text;
	char * Title;
	int64_t Start;
} * ProgressBarDisplay;

ProgressBarDisplay ProgressBarDisplayCreate(struct Minecraft * minecraft);
void ProgressBarDisplaySetTitle(ProgressBarDisplay display, char * title);
void ProgressBarDisplaySetText(ProgressBarDisplay display, char * text);
void ProgressBarDisplaySetProgress(ProgressBarDisplay display, int progress);
void ProgressBarDisplayDestroy(ProgressBarDisplay display);
