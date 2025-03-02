#include <nds.h>
#include <stdio.h>
#include "TileSet.h"

// --- Données de démonstration ---
// Remplace ces tableaux par les données générées par ton outil (grit, gfx2gba, Usenti, etc.)

const u16 tileMap[16 * 16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};




int main(void) {
	// Initialisation du DS en mode 2D (mode 0 pour les backgrounds tilemaps)
	videoSetMode(MODE_0_2D);
	// Affecte VRAM_A au background principal
	vramSetBankA(VRAM_A_MAIN_BG);
	
	// Initialise le background 0 en mode tilemap 256x256 (32x32 tuiles)
	// Les paramètres : (numéro, type, taille, map offset, tile offset)
	int bg0 = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 1, 0);
	
	// Chargement des données en VRAM :
	// Charger le tileset en VRAM
	dmaCopy(TileSetTiles, bgGetGfxPtr(bg0), TileSetTilesLen);

	// Charger la palette
	dmaCopy(TileSetPal, BG_PALETTE, TileSetPalLen);

	// Charger la tilemap (indices des tuiles)
	dmaCopy(tileMap, bgGetMapPtr(bg0), sizeof(tileMap));
	
	while(1) {
		swiWaitForVBlank();
		// Ici, tu peux ajouter du scrolling en modifiant REG_BG0HOFS/REG_BG0VOFS
	}
	
	return 0;
}
