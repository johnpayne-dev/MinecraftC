#include <OpenGL.h>
#include "HUDScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Render/ShapeRenderer.h"
#include "../Utilities/Time.h"

HUDScreen HUDScreenCreate(struct Minecraft * minecraft, int width, int height)
{
	HUDScreen hud = MemoryAllocate(sizeof(struct HUDScreen));
	*hud = (struct HUDScreen)
	{
		.Chat = ListCreate(sizeof(ChatLine)),
		.Random = RandomGeneratorCreate(time(NULL)),
		.HoveredPlayer = NULL,
		.Minecraft = minecraft,
		.Width = width * 240 / height,
		.Height = height * 240 / height,
		
	};
	return hud;
}

void HUDScreenRender(HUDScreen hud, float var1, bool var2, int2 mousePos)
{
	PlayerData player = hud->Minecraft->Player->TypeData;
	RendererEnableGUIMode(hud->Minecraft->Renderer);
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->Minecraft->TextureManager, "GUI/GUI.png"));
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	ScreenDrawImage((int2){ hud->Width / 2 - 91, hud->Height - 22 }, (int2){ 0, 0 }, (int2){ 182, 22 }, -90.0);
	ScreenDrawImage((int2){ hud->Width / 2 - 92 + player->Inventory->Selected * 20, hud->Height - 23 }, (int2){ 0, 22 }, (int2){ 24, 22 }, -90.0);
	glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->Minecraft->TextureManager, "GUI/Icons.png"));
	ScreenDrawImage((int2){ hud->Width / 2 - 7, hud->Height / 2 - 7 }, (int2){ 0, 0 }, (int2){ 16, 16 }, -90.0);
	
	glDisable(GL_BLEND);
	for (int i = 0; i < 9; i++)
	{
		int x = hud->Width / 2 - 90 + i * 20;
		int y = hud->Height - 16;
		BlockType tile = player->Inventory->Slots[i];
		if (tile != -1 && tile != 0)
		{
			glPushMatrix();
			glTranslatef(x, y, -50.0);
			if (player->Inventory->PopTime[i] > 0)
			{
				float f1 = (player->Inventory->PopTime[i] - var1) / 5.0;
				float f2 = -sin(f1 * f1 * pi) * 8.0;
				float f3 = sin(f1 * f1 * pi) + 1.0;
				float f4 = sin(f1 * pi) + 1.0;
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
			glBindTexture(GL_TEXTURE_2D, TextureManagerLoad(hud->Minecraft->TextureManager, "Terrain.png"));
			ShapeRendererBegin();
			BlockRenderFullBrightness(Blocks.Table[tile]);
			ShapeRendererEnd();
			glPopMatrix();
			if (player->Inventory->Count[i] > 1)
			{
				String string = StringCreateFromInt(player->Inventory->Count[i]);
				FontRendererRender(hud->Minecraft->Font, string, x + 19 - FontRendererGetWidth(hud->Minecraft->Font, string), y + 6, ColorWhite);
				StringDestroy(string);
			}
		}
	}
		
	FontRendererRender(hud->Minecraft->Font, "0.30", 2, 2, ColorWhite);
	if (hud->Minecraft->Settings->ShowFrameRate) { FontRendererRender(hud->Minecraft->Font, hud->Minecraft->Debug, 2, 12, ColorWhite); }
		
	int maxLines = 10;
	bool chatScreen = false;
	if (hud->Minecraft->CurrentScreen != NULL && hud->Minecraft->CurrentScreen->Type == GUIScreenTypeChatInput)
	{
		maxLines = 20;
		chatScreen = true;
	}
	for (int i = 0; i < ListCount(hud->Chat) && i < maxLines; i++)
	{
		if (hud->Chat[i]->Time < 200 || chatScreen) { FontRendererRender(hud->Minecraft->Font, hud->Chat[i]->Message, 2, hud->Height - 28 - i * 9, ColorWhite); }
	}
	
	hud->HoveredPlayer = NULL;
	//if (Keyboard.isKeyDown(15) && hud->Minecraft->NetworkManager != NULL && NetworkManagerIsConnected(hud->Minecraft->NetworkManager))
	{
		/*List var22 = this.mc.networkManager.getPlayers();
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
		}*/
	}
}

void HUDScreenAddChat(HUDScreen screen, char * message)
{
	screen->Chat = ListPush(screen->Chat, &(ChatLine){ ChatLineCreate(message) });
	while (ListCount(screen->Chat) > 50) { screen->Chat = ListRemove(screen->Chat, 0); }
}

void HUDScreenDestroy(HUDScreen hud)
{
	for (int i = 0; i < ListCount(hud->Chat); i++) { ChatLineDestroy(hud->Chat[i]); }
	ListDestroy(hud->Chat);
	RandomGeneratorDestroy(hud->Random);
	MemoryFree(hud);
}
