#include "pch.h"
#include "SpriteManager.h"
#include "NDSTime.h"
#include "GameMap.h"
#include "GameContext.h"
#include "Camera.h"
#include "AudioManager.h"
#include "GameManager.h"
using namespace std::string_literals;

int main()
{
	GameManager::initAll(true);

	SpriteManager spManager;

	GameContext& GameCtxt = GameContext::get();

	GameCtxt.audioManager = GameObject::instantiateGO<AudioManager>("AudioManager"s);
	GameCtxt.gameMap      = GameObject::instantiateGO<GameMap>("GameMap"s);
	GameCtxt.camera       = GameObject::instantiateGO<Camera>("Camera"s);
	GameCtxt.player       = GameObject::instantiateGO<Entity>("Player"s);

	GameCtxt.player->setSprite(spManager.getPlayerSprite());
	GameCtxt.player->setSize({24, 34});


	
	int cx = SCREEN_SIZE_W/2, cy = SCREEN_SIZE_H/2;

	int scale = 1 << 8;


	while (true)
	{
		Debug::clearConsole();
		Debug::logProfile();

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

		GameCtxt.player->setAllDirections(dir);

		if (keys_held & KEY_SELECT)
			scale += 1 << 3;
		else if (keys_held & KEY_START)
			scale -= 1 << 3;

		//if(keysDown() & KEY_X) mainCharacterSprite->skipFrame();
		//if(keysDown() & KEY_Y) mainCharacterSprite->setState(6);

		Debug::logEntityInfo(*GameCtxt.player);

		GameObject::updateAllGameObjects(NDSTime::get().getDeltaTime());
		GameCtxt.player->display();

		oamUpdate(&oamMain);

		bgSetCenter(BG::ID, cx, cy);
		bgSetScale(BG::ID, scale, scale);

		NDSTime::get().newFrame();
		Debug::logFps();

		
		swiWaitForVBlank();
	}
	
	return 0;
}
