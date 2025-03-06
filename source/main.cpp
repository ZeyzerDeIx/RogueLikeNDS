#include <stdio.h>
#include "PCH.h"
#include "TileMap.h"
#include "TileSet.h"
#include "SpriteManager.h"
#include "MainCharacterSprite.h"

// Utility function to print the background color mode based on BGxCNT register value.
void printBgColorMode(u16 bgcnt)
{
    // If the BG_COLOR_256 bit is set, the background is in 8bpp (256 colors) mode.
    if(bgcnt & BG_COLOR_256)
        printf("Background mode: 8bpp (256 colors)\n");
    else
        printf("Background mode: 4bpp (16 colors)\n");
}

int main(void)
{
	videoSetMode(MODE_5_2D);

	// Memory mapping
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_MAIN_BG_0x06060000);
	//vramSetBankE(VRAM_E_MAIN_SPRITE);
	//vramSetBankF(VRAM_F_MAIN_SPRITE_0x06410000);
	//vramSetBankG(VRAM_G_MAIN_SPRITE_0x06414000);
	
	int bg0 = bgInit(BG::ID, BG::TYPE, BG::SIZE, 0, 1);

	consoleDemoInit();

	TileMap tileMap;
	tileMap.flush();

	SpriteManager spManager;
	Sprite* mainCharacterSprite = CREATE_PARAMETRIZED_SPRITE(spManager, MainCharacterSprite, _16Color, 32, 64);
	mainCharacterSprite->enableAnim(4,8,8);
	
	int x = 0, y = 0;
	int cx = 0, cy = 0;

	int angle = 10;
	int scale = 1 << 8;
	#if __cplusplus >= 202002L
   	printf("C++20 enabled\n");
	#endif

   printBgColorMode(REG_BG2CNT);


	while (1)
	{
		swiWaitForVBlank();

		// Update registers during the vertical blanking period to prevent
		// screen tearing.
		bgUpdate();

		scanKeys();

		u16 keys_held = keysHeld();

		if (keys_held & KEY_UP)
			y++;
		else if (keys_held & KEY_DOWN)
			y--;
		if (keys_held & KEY_LEFT)
			x++;
		else if (keys_held & KEY_RIGHT)
			x--;

		if (keys_held & KEY_L)
			angle -= 1 << 4;
		else if (keys_held & KEY_R)
			angle += 1 << 4;

		if (keys_held & KEY_SELECT)
			scale += 1 << 3;
		else if (keys_held & KEY_START)
			scale -= 1 << 3;

		if(keysDown() & KEY_X) mainCharacterSprite->skipFrame();
		if(keysDown() & KEY_Y) mainCharacterSprite->setState(6);

		mainCharacterSprite->display(0,0);
		
      oamUpdate(&oamMain);

		bgSetCenter(bg0, cx, cy);
		bgSetRotateScale(bg0, angle, scale, scale);
		bgSetScroll(bg0, x, y);
	}
	
	return 0;
}
