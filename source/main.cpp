#include "pch.h"
#include "SpriteManager.h"
#include "NDSTime.h"
#include "GameMap.h"
#include "GameContext.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GameManager.h"
using namespace std::string_literals;

#define G_CTXT GameContext::get()

int main(void)
{
	GameManager::initAll(true);

	SpriteManager spManager;

	G_CTXT.audioManager = GameObject::instantiateGO<AudioManager>("AudioManager"s);

	G_CTXT.gameMap = GameObject::instantiateGO<GameMap>("GameMap"s);

	G_CTXT.camera = GameObject::instantiateGO<Camera>("Camera"s);

	G_CTXT.player = GameObject::instantiateGO<Entity>("Player"s);
	G_CTXT.player->setSprite(spManager.getPlayerSprite());
	G_CTXT.player->setSize({24, 34});


	
	int cx = SCREEN_SIZE_W/2, cy = SCREEN_SIZE_H/2;

	int scale = 1 << 8;
	
	//player.move({1,1});

	//bool init_ok = nitroFSInit(NULL);
    //if (!init_ok) perror("nitroFSInit()");
    Debug::get();


	while (1)
	{
		//Debug::get().clearConsole();
		//Debug::get().logProfile();

		// Update registers during the vertical blanking period to prevent
		// screen tearing.
		bgUpdate();

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

		G_CTXT.player->setAllDirections(dir);

		if (keys_held & KEY_SELECT)
			scale += 1 << 3;
		else if (keys_held & KEY_START)
			scale -= 1 << 3;

		//if(keysDown() & KEY_X) mainCharacterSprite->skipFrame();
		//if(keysDown() & KEY_Y) mainCharacterSprite->setState(6);

		//Debug::get().logEntityInfo(*G_CTXT.player);

		GameObject::updateAllGameObjects(NDSTime::get().getDeltaTime());
		G_CTXT.player->display();

		oamUpdate(&oamMain);

		bgSetCenter(BG::ID, cx, cy);
		bgSetScale(BG::ID, scale, scale);

		NDSTime::get().newFrame();
		//Debug::get().logFps();

		
		swiWaitForVBlank();
	}
	
	return 0;
}
