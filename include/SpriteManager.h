#pragma once

#include "pch.h"
#include "Sprite.h"

class SpriteManager
{
	public:
		SpriteManager();
		~SpriteManager();

		Sprite* CreateSprite(
			std::string const& name,
			uint32 tilesLen,
			uint32 palLen,
			SpriteSize spriteSize = SpriteSize_32x32,
			Vector2i pixelSize = {32, 32},
			int frameCount = 1,
			int stateCount = 1,
			int animSpeed = 1
		);

		Sprite* GetPlayerSprite();
	private:
		Sprite* m_PlayerSprite;
		std::list<Sprite*> m_Sprites;
};


#define CREATE_SPRITE(spriteManager,spriteName)\
spriteManager.CreateSprite(\
#spriteName,\
spriteName ## TilesLen,\
spriteName ## PalLen)

#define CREATE_PARAMETRIZED_SPRITE(spriteManager,spriteName,sx,sy,frameCount,stateCount,animSpeed)\
spriteManager.CreateSprite(\
#spriteName,\
spriteName ## TilesLen,\
spriteName ## PalLen,\
SpriteSize_ ## sx ## x ## sy,\
{sx,sy},\
frameCount,\
stateCount,\
animSpeed)
