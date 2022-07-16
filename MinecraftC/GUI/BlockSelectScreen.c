#include "BlockSelectScreen.h"
#include "Screen.h"
#include "../SessionData.h"
#include "../Minecraft.h"
#include "../Level/Tile/Block.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/OpenGL.h"

BlockSelectScreen BlockSelectScreenCreate() {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeBlockSelect;
	screen->grabsMouse = true;
	return screen;
}

static int GetBlockOnScreen(BlockSelectScreen screen, int2 m) {
	for (int32_t i = 0; i < ListCount(SessionDataAllowedBlocks); i++) {
		int x = screen->width / 2 + i % 9 * 24 + -108 - 3;
		int y = screen->height / 2 + i / 9 * 24 + -60 + 3;
		if (m.x >= x && m.x <= x + 24 && m.y >= y - 12 && m.y <= y + 12) { return i; }
	}
	return -1;
}

void BlockSelectScreenRender(BlockSelectScreen screen, int2 mouse) {
	int blockNum = GetBlockOnScreen(screen, mouse);
	ScreenDrawFadingBox((int2){ screen->width / 2 - 120, 30 }, (int2){ screen->width / 2 + 120, 180 }, ColorFromHex(0x05050090), ColorFromHex(0x303060C0));
	if (blockNum >= 0) {
		int x = screen->width / 2 + blockNum % 9 * 24 + -108;
		int y = screen->height / 2 + blockNum / 9 * 24 + -60;
		ScreenDrawFadingBox((int2){ x - 3, y - 8 }, (int2){ x + 23, y + 24 - 6 }, ColorFromHex(0xFFFFFF90), ColorFromHex(0xFFFFFFC0));
	}
	ScreenDrawCenteredString(screen->font, "Select block", (int2){ screen->width / 2, 40 }, ColorWhite);
	
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(screen->minecraft->textureManager, "Terrain.png"));
	for (int i = 0; i < ListCount(SessionDataAllowedBlocks); i++) {
		Block block = SessionDataAllowedBlocks[i];
		glPushMatrix();
		int x = screen->width / 2 + i % 9 * 24 + -108;
		int y = screen->height / 2 + i / 9 * 24 + -60;
		glTranslatef(x, y, 0.0);
		glScalef(10.0, 10.0, 10.0);
		glTranslatef(1.0, 0.5, 8.0);
		glRotatef(-30.0, 1.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		if (blockNum == i) { glScalef(1.6, 1.6, 1.6); }
		glTranslatef(-1.5, 0.5, 0.5);
		glScalef(-1.0, -1.0, -1.0);
		ShapeRendererBegin();
		BlockRenderFullBrightness(block);
		ShapeRendererEnd();
		glPopMatrix();
	}
}

void BlockSelectScreenOnMouseClicked(BlockSelectScreen screen, int x, int y, int button) {
	if (button == SDL_BUTTON_LEFT) {
		InventoryReplaceSlot(((PlayerData)screen->minecraft->player->typeData)->inventory, GetBlockOnScreen(screen, (int2){ x, y }));
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	}
}
