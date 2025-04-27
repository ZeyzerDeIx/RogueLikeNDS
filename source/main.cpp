#include "pch.h"
#include "TileMap.h"
#include "TileSet.h"
#include "SpriteManager.h"
#include "MainCharacterSprite.h"
#include "NDSTime.h"
#include "Entity.h"
#include "GameMap.h"
#include "GameContext.h"
#include "Camera.h"


int main(void)
{
	std::srand(std::time({}));
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

	GameContext context;

	SpriteManager spManager;
	Sprite* mainCharacterSprite = CREATE_PARAMETRIZED_SPRITE(spManager, MainCharacterSprite, _16Color, 32, 64);
	mainCharacterSprite->enableAnim(4,8,8);

	Entity player(mainCharacterSprite, {24, 34}, context);

	Camera camera(context);

	GameMap gameMap(context);

	context.gameMap = &gameMap;
	context.camera = &camera;
	context.player = &player;
	
	int cx = SCREEN_SIZE_W/2, cy = SCREEN_SIZE_H/2;

	int scale = 1 << 8;
	
	player.move({1,1});

	bool init_ok = nitroFSInit(NULL);
    if (!init_ok) perror("nitroFSInit()");

    // Load and play background music
    mmInitDefault("nitro:/soundbank.bin");
    mmLoad(MOD_CORRIDORSUIT);
    soundEnable();
    mmStart(MOD_CORRIDORSUIT, MM_PLAY_LOOP);

	while (1)
	{
		//Debug::get().clearConsole();
		// Update registers during the vertical blanking period to prevent
		// screen tearing.
		bgUpdate();

		gameMap.update();

		scanKeys();

		u16 keys_held = keysHeld();

		u8 dir = DIRECTION::NONE;

		if (keys_held & KEY_UP)
			dir |= DIRECTION::TOP;
		else if (keys_held & KEY_DOWN)
			dir |= DIRECTION::BOT;
		if (keys_held & KEY_LEFT)
			dir |= DIRECTION::LEFT;
		else if (keys_held & KEY_RIGHT)
			dir |= DIRECTION::RIGHT;

		player.setAllDirections(dir);

		if (keys_held & KEY_SELECT)
			scale += 1 << 3;
		else if (keys_held & KEY_START)
			scale -= 1 << 3;

		if(keysDown() & KEY_X) mainCharacterSprite->skipFrame();
		if(keysDown() & KEY_Y) mainCharacterSprite->setState(6);

		player.update(NDSTime::get().getDeltaTime());
		player.display(camera);
		//Debug::get().displayEntityInfo(player);

		camera.update();

		oamUpdate(&oamMain);

		bgSetCenter(BG::ID, cx, cy);
		bgSetScale(BG::ID, scale, scale);

		NDSTime::get().newFrame();
		//Debug::get().displayFps();
		
		swiWaitForVBlank();
	}
	
	return 0;
}
