#include <nds.h>
#include <stdio.h>
#include "TileSet.h"

// --- Données de démonstration ---
// Remplace ces tableaux par les données générées par ton outil (grit, gfx2gba, Usenti, etc.)

// 📌 Constantes pour la configuration du background
#define BACKGROUND_ID  2
#define BG_TYPE        BgType_Rotation
#define BG_SIZE        BgSize_R_256x256
#define CHARBLOCK      0
#define MAPBLOCK       1


int main(void) {
	// Initialisation du DS en mode 2D (mode 0 pour les backgrounds tilemaps)
	videoSetMode(MODE_2_2D);
	// Affecte VRAM_A au background principal
	vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();
	
	// Initialise le background 0 en mode tilemap 256x256 (32x32 tuiles)
	// Les paramètres : (numéro, type, taille, map offset, tile offset)
	int bg0 = bgInit(BACKGROUND_ID, BG_TYPE, BG_SIZE, CHARBLOCK, MAPBLOCK);
	
	// Chargement des données en VRAM :
	// Charger le tileset en VRAM
	dmaCopy(TileSetTiles, bgGetGfxPtr(bg0), TileSetTilesLen);

	// Charger la palette
	dmaCopy(TileSetPal, BG_PALETTE, TileSetPalLen);

	swiWaitForVBlank();

	u8 *mapPtr = (u8*)bgGetMapPtr(bg0);
	static u8 tileMap[32*24] = 
{
	0 ,1 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	16,17,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,0 ,0 ,1 ,2 ,3 ,4 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 
};

	//DC_InvalidateAll();
//
	//u8 i = 0;
	//for(i=0 ; i < 32 ; i++)
	//{
	//	mapPtr[i] = i%8;
	//	printf("%d : ", mapPtr[i]);
	//}
//
	//DC_FlushAll();
	//dmaCopy(tileMap, mapPtr, 16 * 24);
	//dmaCopy(tileMap, mapPtr, sizeof(tileMap)>>1);

	//mapPtr[0] = 0;
	//mapPtr[1] = 1;
	//mapPtr[32] = 16;
	//mapPtr[33] = 17;
	
	int x = 0, y = 0;
	int cx = 0, cy = 0;

	int angle = 10;
	int scale = 1 << 8;

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

		if (keys_held & KEY_X)
			swiFastCopy(tileMap, mapPtr, sizeof(tileMap)>>2);

		bgSetCenter(bg0, cx, cy);
		bgSetRotateScale(bg0, angle, scale, scale);
		bgSetScroll(bg0, x, y);
	}
	
	return 0;
}
