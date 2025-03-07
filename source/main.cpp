#include "PCH.h"
#include "TileMap.h"
#include "TileSet.h"
#include "SpriteManager.h"
#include "MainCharacterSprite.h"

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
	
	bgInit(BG::ID, BG::TYPE, BG::SIZE, 0, 1);

	//init debug console
	Debug::get();

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
   	std::cout << "C++20 enabled\n";;
	#endif


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

		bgSetCenter(BG::ID, cx, cy);
		bgSetRotateScale(BG::ID, angle, scale, scale);
		bgSetScroll(BG::ID, x, y);
	}
	
	return 0;
}
