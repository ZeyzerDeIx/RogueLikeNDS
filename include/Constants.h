#pragma once

// All of those are in pixel
constexpr int SCREEN_SIZE_W = 256;
constexpr int SCREEN_SIZE_H = 192;


namespace BG
{
	constexpr int SIZE_W = 512;
	constexpr int SIZE_H = 512;
	constexpr int ID = 2;

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

	constexpr BgSize getSize(int width, int height, bool ext_rotation, bool rotation_mode, bool bitmap_mode, bool bitdepth_16)
	{
		if (ext_rotation)
		{ // Extended Rotation backgrounds
			if (width == 128 && height == 128)   return BgSize_ER_128x128;
			if (width == 256 && height == 256)   return BgSize_ER_256x256;
			if (width == 512 && height == 512)   return BgSize_ER_512x512;
			if (width == 1024 && height == 1024) return BgSize_ER_1024x1024;
		}
		else if (rotation_mode)
		{ // Classic Rotation backgrounds
			if (width == 128 && height == 128)   return BgSize_R_128x128;
			if (width == 256 && height == 256)   return BgSize_R_256x256;
			if (width == 512 && height == 512)   return BgSize_R_512x512;
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
				if (width == 128 && height == 128)  return BgSize_B8_128x128;
				if (width == 256 && height == 256)  return BgSize_B8_256x256;
				if (width == 512 && height == 256)  return BgSize_B8_512x256;
				if (width == 512 && height == 512)  return BgSize_B8_512x512;
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

	constexpr BgType getType(bool ext_rotation, bool rotation_mode, bool bitmap_mode, bool bitdepth_16, bool text_mode)
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

	constexpr BgSize SIZE = getSize(SIZE_W, SIZE_W, EXT_ROTATION, ROTATION_MODE, BITMAP_MODE, BITDEPTH_16);

	constexpr BgType TYPE = getType(EXT_ROTATION, ROTATION_MODE, BITMAP_MODE, BITDEPTH_16, TEXT_MODE);
}

namespace TILESET
{
	constexpr int COUNT_W = 5;
	constexpr int COUNT_H = 4;
}
namespace TILE // Tiles like visible on the tileset sprite
{
	constexpr int SIZE = 16;

	constexpr int COUNT_W = BG::SIZE_W / SIZE;
	constexpr int COUNT_H = BG::SIZE_H / SIZE;

	constexpr int FLIP_H = 10;
	constexpr int FLIP_V = 11;
}
namespace SUB_TILE // Tiles like considered by the NDS
{
	constexpr int SIZE = 8;
	constexpr int COUNT_W = BG::SIZE_W / SIZE;
	constexpr int COUNT_H = BG::SIZE_H / SIZE;
}
namespace META_TILE // Tiles at a gameplay level, composed of 4 Tile
{
	constexpr int SIZE = 2 * TILE::SIZE;

	constexpr int COUNT_W = BG::SIZE_W / SIZE;
	constexpr int COUNT_H = BG::SIZE_H / SIZE;

	enum class Type { Path, Wall, Void };
	namespace CORNER {enum { TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT };}
}

namespace DIRECTION
{
	constexpr u8 NONE      = 0b0000'0000;
	constexpr u8 TOP       = 0b0000'0001;
	constexpr u8 BOT       = 0b0000'0010;
	constexpr u8 LEFT      = 0b0000'0100;
	constexpr u8 RIGHT     = 0b0000'1000;
	constexpr u8 TOP_LEFT  = 0b0001'0000;
	constexpr u8 TOP_RIGHT = 0b0010'0000;
	constexpr u8 BOT_LEFT  = 0b0100'0000;
	constexpr u8 BOT_RIGHT = 0b1000'0000;
}

namespace ENTITY
{
	namespace ANIMATION
	{
		enum DIRECTION
		{
			TOP,
			BOT,
			LEFT,
			RIGHT,
			TOP_MOVING,
			BOT_MOVING,
			LEFT_MOVING,
			RIGHT_MOVING
		};
		// Offset between moving states and idle states.
		constexpr int MOVING_STATE_OFFSET = 4;
		constexpr bool isMovingState(int state)
		{
			return state > DIRECTION::RIGHT;
		}
	}
}

#define PALETTE_SIZE 16