#include "SpriteManager.h"
#include "MainCharacterSprite.h"
#include "AssetManager.h"

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
	string const& name,
	uint32 tilesLen,
	uint32 palLen,
	SpriteSize spriteSize,
	Vector2i pixelSize,
	int frameCount,
	int stateCount,
	int animSpeed
)
{
	//memory allocation
	u16* spriteData = NULL;
    for (int i = 0 ; i < frameCount*stateCount ; i++)
    {
        u16* ptr = oamAllocateGfx(&oamMain, spriteSize, SpriteColorFormat_16Color);
        if (i == 0) spriteData = ptr; 
    }

    //binary files loading
	AssetManager::loadBin(name + ".img.bin"s, spriteData, tilesLen);
	AssetManager::loadBin(name + ".pal.bin"s, SPRITE_PALETTE + m_sprites.size()*PALETTE_SIZE, palLen);

	//sprite creation
	Sprite* newSprite = new Sprite(this,m_sprites.size(),spriteSize,spriteData,pixelSize, frameCount, stateCount, animSpeed);

	//add the sprite to the sprite list
	m_sprites.push_back(newSprite);
	return newSprite;
}

Sprite* SpriteManager::getPlayerSprite()
{
	if(!m_playerSprite)
		m_playerSprite = CREATE_PARAMETRIZED_SPRITE((*this), MainCharacterSprite, 32, 64, 4, 8, 8);

	return m_playerSprite;
}