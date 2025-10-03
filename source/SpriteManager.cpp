#include "SpriteManager.h"
#include "MainCharacterSprite.h"

using namespace std;

SpriteManager::SpriteManager(): m_playerSprite(nullptr)
{
	//alloc memory banks dedicated to sprites
	vramSetBankE(VRAM_E_MAIN_SPRITE);
	vramSetBankF(VRAM_F_MAIN_SPRITE_0x06410000);
	vramSetBankG(VRAM_G_MAIN_SPRITE_0x06414000);
	oamInit(&oamMain, SpriteMapping_1D_128 , false);
}
SpriteManager::~SpriteManager()
{
	for (auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
		delete *i;
}


Sprite* SpriteManager::createSprite(
	const void* tiles,
	uint32 tilesLen,
	const void* pal,
	uint32 palLen,
	SpriteSize size,
	SpriteColorFormat format,
	int h, int w
)
{
	//memory allocation
	u16* memoryLocation = oamAllocateGfx(&oamMain, size, format);
	dmaCopy(tiles, memoryLocation, tilesLen);
	dmaCopy(pal, SPRITE_PALETTE + m_sprites.size()*PALETTE_SIZE, palLen);

	//sprite creation
	Sprite* newSprite = new Sprite(this,m_sprites.size(),size,format,memoryLocation, (u16*)tiles, {h, w});

	//add the sprite to the sprite list
	m_sprites.push_back(newSprite);
	return newSprite;
}

Sprite* SpriteManager::getPlayerSprite()
{
	if(!m_playerSprite)
	{
		m_playerSprite = CREATE_PARAMETRIZED_SPRITE((*this), MainCharacterSprite, _16Color, 32, 64);
		m_playerSprite->enableAnim(4,8,8);
	}

	return m_playerSprite;
}