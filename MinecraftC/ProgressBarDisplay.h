#pragma once
#include <stdint.h>

typedef struct ProgressBarDisplay {
	struct Minecraft * minecraft;
	char * text;
	char * title;
	int64_t start;
} * ProgressBarDisplay;

ProgressBarDisplay ProgressBarDisplayCreate(struct Minecraft * minecraft);
void ProgressBarDisplaySetTitle(ProgressBarDisplay display, char * title);
void ProgressBarDisplaySetText(ProgressBarDisplay display, char * text);
void ProgressBarDisplaySetProgress(ProgressBarDisplay display, int progress);
void ProgressBarDisplayDestroy(ProgressBarDisplay display);
