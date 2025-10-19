#include "pch.h"
#include "SpriteManager.h"
#include "NDSTime.h"
#include "GameMap.h"
#include "GameContext.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GameManager.h"


int main(void)
{
	GameManager::initAll();

	//init debug console
	Debug::get();

	SpriteManager spManager;
	Sprite* mainCharacterSprite = spManager.getPlayerSprite();

	Entity player(mainCharacterSprite, {24, 34});

	GameMap gameMap;

    AudioManager audioManager;

	GameContext::get().gameMap = &gameMap;
	GameContext::get().camera = GameObject::instantiateGO<Camera>("Camera");;
	GameContext::get().player = &player;
	GameContext::get().audioManager = &audioManager;
	
	int cx = SCREEN_SIZE_W/2, cy = SCREEN_SIZE_H/2;

	int scale = 1 << 8;
	
	//player.move({1,1});

	//bool init_ok = nitroFSInit(NULL);
    //if (!init_ok) perror("nitroFSInit()");


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
		player.display(*GameContext::get().camera);
		//Debug::get().displayEntityInfo(player);

		GameObject::updateAllGameObjects(NDSTime::get().getDeltaTime());

		oamUpdate(&oamMain);

		bgSetCenter(BG::ID, cx, cy);
		bgSetScale(BG::ID, scale, scale);

		NDSTime::get().newFrame();
		//Debug::get().displayFps();
		
		swiWaitForVBlank();
	}
	
	return 0;
}
