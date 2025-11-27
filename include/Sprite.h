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

	void update(float speedFactor = 1);
	void display(Vector2i pos = {0,0}, bool zoomed = false);
	void skipFrame(int num = 1);
	void setState(int state);
	void setFrame(int frame);

	int getState();

	const Vector2i& getPixelSize() const;

private:
	SpriteManager* m_manager;
	Vector2i m_pixelSize;
	SpriteSize m_spriteSize;
	u16* m_data;
	short m_id;

	// Animation-related members

	// Currently displayed frame (x)
	short m_currentFrame;
	// Total number of animation frames
	short const m_frameCount;
	// Currently displayed state (y)
	short m_currentState;
	// Total number of animation states
	short const m_stateCount;
	// Number of updates before switching frames
	short m_animSpeed;
	// Counts updates
	short m_counter;
	/** @brief Memory offset to jump to the next frame using u16* pointer arithmetic.
	 *  
	 * Calculation: (Width * Height) / 4.
	 * - Division by 2 converts pixels to bytes (4bpp mode: 1 pixel = 0.5 byte).
	 * - Division by 2 accounts for u16* pointer arithmetic (incrementing by 1 jumps 2 bytes).
	 */
	short const m_frameMemoryOffset;
};