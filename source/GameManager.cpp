#include "GameManager.h"

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::initAll(bool debugMode)
{
	std::srand(std::time({}));
	videoSetMode(MODE_5_2D);

	// Memory mapping
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_MAIN_BG_0x06060000);
	
	bgInit(BG::ID, BG::TYPE, BG::SIZE, 0, 1);
	bgSetControlBits(BG::ID, BG_WRAP_ON);

	bool nitroFSInitSuccess = nitroFSInit(NULL);

	if(!debugMode) return;

	//init debug console
	Debug::get();
	std::cout << (nitroFSInitSuccess ? "nitroFSInit succeed" : "nitroFSInit failed") << std::endl;
}