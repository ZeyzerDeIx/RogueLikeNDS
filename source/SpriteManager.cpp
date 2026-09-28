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
	for (Sprite*& m_Sprite : m_Sprites)
		delete m_Sprite;
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
	u16* ramData = new u16[tilesLen / 2];
	u16* vRamData = oamAllocateGfx(&oamMain, spriteSize, SpriteColorFormat_16Color);

    //binary files loading
	AssetManager::LoadBin(name + ".img.bin"s, ramData, tilesLen);
	AssetManager::LoadBin(name + ".pal.bin"s, SPRITE_PALETTE + m_Sprites.size()*PALETTE_SIZE, palLen);

	//sprite creation
	Sprite* newSprite = new Sprite(this,m_Sprites.size(),spriteSize,ramData,vRamData,pixelSize, frameCount, stateCount, animSpeed);

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