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
	static_assert(__cplusplus > 202302L, "Not using C++26!");

	GameManager::InitAll(true);

	SpriteManager spManager;

	GameContext& GameCtxt = GameContext::get();

	GameCtxt.m_AudioManager = GameObject::InstantiateGO<AudioManager>("AudioManager"s);
	GameCtxt.m_GameMap      = GameObject::InstantiateGO<GameMap>("GameMap"s);
	GameCtxt.m_Camera       = GameObject::InstantiateGO<Camera>("Camera"s);
	GameCtxt.m_Player       = GameObject::InstantiateGO<Entity>("Player"s);

	GameCtxt.m_Player->SetSprite(spManager.GetPlayerSprite());
	GameCtxt.m_Player->SetSize({24, 34});


	
	int cx = SCREEN_SIZE_W/2, cy = SCREEN_SIZE_H/2;

	int scale = 1 << 8;


	while (true)
	{
		scanKeys();
		u16 keys_held = keysHeld();
		u8 dir = DIRECTION::NONE;

		if (keys_held & KEY_UP)    dir |= DIRECTION::TOP;
		else if (keys_held & KEY_DOWN) dir |= DIRECTION::BOT;
		if (keys_held & KEY_LEFT)  dir |= DIRECTION::LEFT;
		else if (keys_held & KEY_RIGHT)dir |= DIRECTION::RIGHT;

		if (keys_held & KEY_SELECT) scale += 1 << 3;
		else if (keys_held & KEY_START) scale -= 1 << 3;

		GameCtxt.m_Player->SetAllDirections(dir);

		GameObject::UpdateAllGameObjects(NDSTime::Get().GetDeltaTime());

		GameCtxt.m_Player->Display();

		bgSetCenter(BG::ID, cx, cy);
		bgSetScale(BG::ID, scale, scale);

		NDSTime::Get().NewFrame();

		swiWaitForVBlank();

		bgUpdate();
		oamUpdate(&oamMain);
		GameCtxt.m_GameMap->Display();

		Debug::ClearConsole();
		Debug::LogProfile();
		Debug::LogEntityInfo(*GameCtxt.m_Player);
		Debug::LogFps();
	}
	
	return 0;
}
