#pragma once

#include "pch.h"
#include "Sprite.h"

class SpriteManager
{
	public:
		SpriteManager();
		~SpriteManager();

		Sprite* createSprite(
			std::string const& name,
			uint32 tilesLen,
			uint32 palLen,
			SpriteSize spriteSize = SpriteSize_32x32,
			Vector2i pixelSize = {32, 32},
			int frameCount = 1,
			int stateCount = 1,
			int animSpeed = 1
		);

		Sprite* getPlayerSprite();
	private:
		Sprite* m_playerSprite;
		std::list<Sprite*> m_sprites;
};


#define CREATE_SPRITE(spriteManager,spriteName)\
spriteManager.createSprite(\
#spriteName,\
spriteName ## TilesLen,\
spriteName ## PalLen)

#define CREATE_PARAMETRIZED_SPRITE(spriteManager,spriteName,sx,sy,frameCount,stateCount,animSpeed)\
spriteManager.createSprite(\
#spriteName,\
spriteName ## TilesLen,\
spriteName ## PalLen,\
SpriteSize_ ## sx ## x ## sy,\
{sx,sy},\
frameCount,\
stateCount,\
animSpeed)
