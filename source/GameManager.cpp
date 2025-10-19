#include "GameManager.h"

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::initAll()
{
	std::srand(std::time({}));
	videoSetMode(MODE_5_2D);

	// Memory mapping
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_MAIN_BG_0x06060000);
	
	bgInit(BG::ID, BG::TYPE, BG::SIZE, 0, 1);
}