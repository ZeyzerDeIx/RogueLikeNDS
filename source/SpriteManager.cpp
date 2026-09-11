#include "SpriteManager.h"
#include "MainCharacterSprite.h"
#include "AssetManager.h"

using namespace std;

SpriteManager::SpriteManager(): m_PlayerSprite(nullptr)
{
	//alloc memory banks dedicated to sprites
	vramSetBankE(VRAM_E_MAIN_SPRITE);
	vramSetBankF(VRAM_F_MAIN_SPRITE_0x06410000);
	vramSetBankG(VRAM_G_MAIN_SPRITE_0x06414000);
	oamInit(&oamMain, SpriteMapping_1D_128 , false);
}
SpriteManager::~SpriteManager()
{
	for (auto i = m_Sprites.begin(); i != m_Sprites.end(); ++i)
		delete *i;
}


Sprite* SpriteManager::CreateSprite(
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
	AssetManager::LoadBin(name + ".img.bin"s, spriteData, tilesLen);
	AssetManager::LoadBin(name + ".pal.bin"s, SPRITE_PALETTE + m_Sprites.size()*PALETTE_SIZE, palLen);

	//sprite creation
	Sprite* newSprite = new Sprite(this,m_Sprites.size(),spriteSize,spriteData,pixelSize, frameCount, stateCount, animSpeed);

	//add the sprite to the sprite list
	m_Sprites.push_back(newSprite);
	return newSprite;
}

Sprite* SpriteManager::GetPlayerSprite()
{
	if(!m_PlayerSprite)
		m_PlayerSprite = CREATE_PARAMETRIZED_SPRITE((*this), MainCharacterSprite, 32, 64, 4, 8, 8);

	return m_PlayerSprite;
}