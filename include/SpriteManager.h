#pragma once

#include "PCH.h"
#include "Sprite.h"

class SpriteManager
{
	public:
		SpriteManager();
		~SpriteManager();
		Sprite* createSprite(const void* tiles, uint32 tilesLen, const void* pal, uint32 palLen, SpriteSize size = SpriteSize_32x32, SpriteColorFormat format = SpriteColorFormat_16Color, int h = 32, int w = 32);
	private:
		std::list<Sprite*> m_sprites;
};


#define CREATE_SPRITE(spriteManager,spriteName) spriteManager.createSprite(spriteName ## Tiles, spriteName ## TilesLen, spriteName ## Pal, spriteName ## PalLen)
#define CREATE_PARAMETRIZED_SPRITE(spriteManager,spriteName,format,sx,sy) spriteManager.createSprite(spriteName ## Tiles, spriteName ## TilesLen, spriteName ## Pal, spriteName ## PalLen, SpriteSize_ ## sx ## x ## sy, SpriteColorFormat ## format, sx, sy)