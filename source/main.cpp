#include <stdio.h>
#include "PCH.h"
#include "TileMap.h"
#include "TileSet.h"


int main(void) {
	// Initialisation du DS en mode 2D (mode 2 pour les backgrounds tilemaps affine)
	videoSetMode(MODE_2_2D);
	// Affecte VRAM_A au background principal
	vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();

	printf("%d\n", BG::TYPE);
	
	// Initialise le background en mode tilemap 256x256 (32x32 tuiles)
	// Les paramètres : (numéro, type, taille, map offset, tile offset)
	int bg0 = bgInit(BG::ID, BG::TYPE, BG::SIZE, 0, 1);
	
	// Chargement des données en VRAM :
	// Charger le tileset en VRAM
	dmaCopy(TileSetTiles, bgGetGfxPtr(bg0), TileSetTilesLen);

	// Charger la palette
	dmaCopy(TileSetPal, BG_PALETTE, TileSetPalLen);

	TileMap tileMap;
	tileMap.flush();

	
	int x = 0, y = 0;
	int cx = 0, cy = 0;

	int angle = 10;
	int scale = 1 << 8;
	#if __cplusplus >= 202002L
   	printf("C++20 enabled\n");
	#endif


	while (1)
	{
		swiWaitForVBlank();

		// Update registers during the vertical blanking period to prevent
		// screen tearing.
		bgUpdate();

		scanKeys();

		u16 keys_held = keysHeld();

		if (keys_held & KEY_UP)
			y++;
		else if (keys_held & KEY_DOWN)
			y--;
		if (keys_held & KEY_LEFT)
			x++;
		else if (keys_held & KEY_RIGHT)
			x--;

		if (keys_held & KEY_L)
			angle -= 1 << 4;
		else if (keys_held & KEY_R)
			angle += 1 << 4;

		if (keys_held & KEY_SELECT)
			scale += 1 << 3;
		else if (keys_held & KEY_START)
			scale -= 1 << 3;

		bgSetCenter(bg0, cx, cy);
		bgSetRotateScale(bg0, angle, scale, scale);
		bgSetScroll(bg0, x, y);
	}
	
	return 0;
}
