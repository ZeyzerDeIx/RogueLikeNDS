#pragma once

// All of those are in pixel
constexpr int SCREEN_SIZE_W = 256;
constexpr int SCREEN_SIZE_H = 192;

constexpr int BACKGROUND_SIZE_W = 256;
constexpr int BACKGROUND_SIZE_H = 256;

constexpr int SUB_TILE_SIZE = 8;
constexpr int TILE_SIZE = 16;

constexpr int TILE_COUNT_W = BACKGROUND_SIZE_W / TILE_SIZE;
constexpr int TILE_COUNT_H = BACKGROUND_SIZE_H / TILE_SIZE;
constexpr int SUB_TILE_COUNT_W = BACKGROUND_SIZE_W / SUB_TILE_SIZE;
constexpr int SUB_TILE_COUNT_H = BACKGROUND_SIZE_H / SUB_TILE_SIZE;

constexpr int BACKGROUND_ID = 2;

// True → Use extended rotation mode (ER)
constexpr bool EXT_ROTATION = false;
// True → Use bitmap mode (B8/B16), False → Tile mode (T)
constexpr bool BITMAP_MODE = false;
// True → 16-bit bitmap mode, False → 8-bit bitmap mode
constexpr bool BITDEPTH_16 = false;
// True → Use Text mode (BgType_Text4bpp or BgType_Text8bpp)
constexpr bool TEXT_MODE = false;
// True → Rotation background (not extended)
constexpr bool ROTATION_MODE = true;


constexpr BgSize getBgSize(int width, int height, bool ext_rotation, bool rotation_mode, bool bitmap_mode, bool bitdepth_16)
{
	if (ext_rotation)
	{ // Extended Rotation backgrounds
		if (width == 128 && height == 128) return BgSize_ER_128x128;
		if (width == 256 && height == 256) return BgSize_ER_256x256;
		if (width == 512 && height == 512) return BgSize_ER_512x512;
		if (width == 1024 && height == 1024) return BgSize_ER_1024x1024;
	}
	else if (rotation_mode)
	{ // Classic Rotation backgrounds
		if (width == 128 && height == 128) return BgSize_R_128x128;
		if (width == 256 && height == 256) return BgSize_R_256x256;
		if (width == 512 && height == 512) return BgSize_R_512x512;
		if (width == 1024 && height == 1024) return BgSize_R_1024x1024;
	}
	else if (bitmap_mode)
	{ // Bitmap backgrounds (8-bit and 16-bit)
		if (bitdepth_16)
		{
			if (width == 128 && height == 128) return BgSize_B16_128x128;
			if (width == 256 && height == 256) return BgSize_B16_256x256;
			if (width == 512 && height == 256) return BgSize_B16_512x256;
			if (width == 512 && height == 512) return BgSize_B16_512x512;
		}
		else
		{
			if (width == 128 && height == 128) return BgSize_B8_128x128;
			if (width == 256 && height == 256) return BgSize_B8_256x256;
			if (width == 512 && height == 256) return BgSize_B8_512x256;
			if (width == 512 && height == 512) return BgSize_B8_512x512;
			if (width == 1024 && height == 512) return BgSize_B8_1024x512;
			if (width == 512 && height == 1024) return BgSize_B8_512x1024;
		}
	}
	else
	{ // Normal tile-based backgrounds
		if (width == 256 && height == 256) return BgSize_T_256x256;
		if (width == 512 && height == 256) return BgSize_T_512x256;
		if (width == 256 && height == 512) return BgSize_T_256x512;
		if (width == 512 && height == 512) return BgSize_T_512x512;
	}
	return static_cast<BgSize>(-1); // Invalid size
}


constexpr BgType getBgType(bool ext_rotation, bool rotation_mode, bool bitmap_mode, bool bitdepth_16, bool text_mode)
{
	if (bitmap_mode)
		return bitdepth_16 ? BgType_Bmp16 : BgType_Bmp8;
	if (ext_rotation)
		return BgType_ExRotation;
	if (rotation_mode)
		return BgType_Rotation;
	if (text_mode)
		return bitdepth_16 ? BgType_Text8bpp : BgType_Text4bpp;
	return static_cast<BgType>(-1); // Invalid value
}


constexpr BgSize BG_SIZE = getBgSize(BACKGROUND_SIZE_W, BACKGROUND_SIZE_W, EXT_ROTATION, ROTATION_MODE, BITMAP_MODE, BITDEPTH_16);

constexpr BgType BG_TYPE = getBgType(EXT_ROTATION, ROTATION_MODE, BITMAP_MODE, BITDEPTH_16, TEXT_MODE);
