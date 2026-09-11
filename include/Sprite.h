#pragma once

#include "pch.h"
class SpriteManager;

class Sprite
{
public:
	Sprite(SpriteManager* manager,
		int id,
		SpriteSize spriteSize,
		u16* data,
		Vector2i pixelSize = {32, 32},
		int frameCount = 1,
		int stateCount = 1,
		int animSpeed = 1
	);
	~Sprite();

	void Update(float speedFactor = 1);
	void Display(Vector2i pos = {0,0}, bool zoomed = false);
	void SkipFrame(int num = 1);
	void SetState(int state);
	void SetFrame(int frame);

	int GetState();

	[[nodiscard]] const Vector2i& getPixelSize() const;

private:
	SpriteManager* m_Manager;
	Vector2i m_PixelSize;
	SpriteSize m_SpriteSize;
	u16* m_Data;
	short m_Id;

	// Animation-related members

	// Currently displayed frame (x)
	short m_CurrentFrame;
	// Total number of animation frames
	short const m_FrameCount;
	// Currently displayed state (y)
	short m_CurrentState;
	// Total number of animation states
	short const m_StateCount;
	// Number of updates before switching frames
	short m_AnimSpeed;
	// Counts updates
	short m_Counter;
	/** @brief Memory offset to jump to the next frame using u16* pointer arithmetic.
	 *  
	 * Calculation: (Width * Height) / 4.
	 * - Division by 2 converts pixels to bytes (4bpp mode: 1 pixel = 0.5 byte).
	 * - Division by 2 accounts for u16* pointer arithmetic (incrementing by 1 jumps 2 bytes).
	 */
	short const m_FrameMemoryOffset;
};