#include "HUDScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/Time.h"
#include "../Utilities/SinTable.h"
#include "../Utilities/OpenGL.h"

HUDScreen HUDScreenCreate(struct Minecraft * minecraft, int width, int height) {
	HUDScreen hud = MemoryAllocate(sizeof(struct HUDScreen));
	*hud = (struct HUDScreen) {
		.chat = ListCreate(sizeof(ChatLine)),
		.random = RandomGeneratorCreate(time(NULL)),
		.hoveredPlayer = NULL,
		.minecraft = minecraft,
		.width = width * 240 / height,
		.height = height * 240 / height,
		
	};
	return hud;
}

void HUDScreenRender(HUDScreen hud, float var1, bool var2, int2 mousePos) {
	PlayerData player = hud->minecraft->player->typeData;
	RendererEnableGUIMode(hud->minecraft->renderer);
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->minecraft->textureManager, "GUI/GUI.png"));
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	ScreenDrawImage((int2){ hud->width / 2 - 91, hud->height - 22 }, (int2){ 0, 0 }, (int2){ 182, 22 }, -90.0);
	ScreenDrawImage((int2){ hud->width / 2 - 92 + player->inventory->selected * 20, hud->height - 23 }, (int2){ 0, 22 }, (int2){ 24, 22 }, -90.0);
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->minecraft->textureManager, "GUI/Icons.png"));
	ScreenDrawImage((int2){ hud->width / 2 - 7, hud->height / 2 - 7 }, (int2){ 0, 0 }, (int2){ 16, 16 }, -90.0);
	
	glDisable(GL_BLEND);
	for (int i = 0; i < 9; i++) {
		int x = hud->width / 2 - 90 + i * 20;
		int y = hud->height - 16;
		BlockType tile = player->inventory->slots[i];
		if (tile != -1 && tile != 0) {
			glPushMatrix();
			glTranslatef(x, y, -50.0);
			if (player->inventory->popTime[i] > 0) {
				float f1 = (player->inventory->popTime[i] - var1) / 5.0;
				float f2 = -tsin(f1 * f1 * pi) * 8.0;
				float f3 = tsin(f1 * f1 * pi) + 1.0;
				float f4 = tsin(f1 * pi) + 1.0;
				glTranslatef(10.0, f2 + 10.0, 0.0);
				glScalef(f3, f4, 1.0);
				glTranslatef(-10.0, -10.0, 0.0);
			}
			glScalef(10.0, 10.0, 10.0);
			glTranslatef(1.0, 0.5, 0.0);
			glRotatef(-30.0, 1.0, 0.0, 0.0);
			glRotatef(45.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.5, 0.5, 0.5);
			glScalef(-1.0, -1.0, -1.0);
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->minecraft->textureManager, "Terrain.png"));
			ShapeRendererBegin();
			BlockRenderFullBrightness(Blocks.table[tile]);
			ShapeRendererEnd();
			glPopMatrix();
			if (player->inventory->count[i] > 1) {
				String string = StringCreateFromInt(player->inventory->count[i]);
				FontRendererRender(hud->minecraft->font, string, x + 19 - FontRendererGetWidth(hud->minecraft->font, string), y + 6, ColorWhite);
				StringDestroy(string);
			}
		}
	}
		
	FontRendererRender(hud->minecraft->font, "0.30", 2, 2, ColorWhite);
	if (hud->minecraft->settings->showFrameRate) { FontRendererRender(hud->minecraft->font, hud->minecraft->debug, 2, 12, ColorWhite); }
		
	int maxLines = 10;
	bool chatScreen = false;
	if (hud->minecraft->currentScreen != NULL && hud->minecraft->currentScreen->type == GUIScreenTypeChatInput) {
		maxLines = 20;
		chatScreen = true;
	}
	for (int i = 0; i < ListCount(hud->chat) && i < maxLines; i++) {
		if (hud->chat[i]->time < 200 || chatScreen) { FontRendererRender(hud->minecraft->font, hud->chat[i]->message, 2, hud->height - 28 - i * 9, ColorWhite); }
	}
	
	hud->hoveredPlayer = NULL;
	/*if (Keyboard.isKeyDown(15) && hud->Minecraft->NetworkManager != NULL && NetworkManagerIsConnected(hud->Minecraft->NetworkManager))
	{
		List var22 = this.mc.networkManager.getPlayers();
		GL11.glEnable(3042);
		GL11.glDisable(3553);
		GL11.glBlendFunc(770, 771);
		GL11.glBegin(7);
		GL11.glColor4f(0.0F, 0.0F, 0.0F, 0.7F);
		GL11.glVertex2f((float)(var14 + 128), (float)(var15 - 68 - 12));
		GL11.glVertex2f((float)(var14 - 128), (float)(var15 - 68 - 12));
		GL11.glColor4f(0.2F, 0.2F, 0.2F, 0.8F);
		GL11.glVertex2f((float)(var14 - 128), (float)(var15 + 68));
		GL11.glVertex2f((float)(var14 + 128), (float)(var15 + 68));
		GL11.glEnd();
		GL11.glDisable(3042);
		GL11.glEnable(3553);
		var21 = "Connected players:";
		var5.render(var21, var14 - var5.getWidth(var21) / 2, var15 - 64 - 12, 16777215);
		for(var11 = 0; var11 < var22.size(); ++var11)
		{
			int var28 = var14 + var11 % 2 * 120 - 120;
			int var17 = var15 - 64 + (var11 / 2 << 3);
			if(var2 && var3 >= var28 && var4 >= var17 && var3 < var28 + 120 && var4 < var17 + 8)
			{
				this.hoveredPlayer = (String)var22.get(var11);
				var5.renderNoShadow((String)var22.get(var11), var28 + 2, var17, 16777215);
			}
			else
			{
				var5.renderNoShadow((String)var22.get(var11), var28, var17, 15658734);
			}
		}
	}*/
}

void HUDScreenAddChat(HUDScreen screen, char * message) {
	screen->chat = ListPush(screen->chat, &(ChatLine){ ChatLineCreate(message) });
	while (ListCount(screen->chat) > 50) { screen->chat = ListRemove(screen->chat, 0); }
}

void HUDScreenDestroy(HUDScreen hud) {
	for (int i = 0; i < ListCount(hud->chat); i++) { ChatLineDestroy(hud->chat[i]); }
	ListDestroy(hud->chat);
	RandomGeneratorDestroy(hud->random);
	MemoryFree(hud);
}
