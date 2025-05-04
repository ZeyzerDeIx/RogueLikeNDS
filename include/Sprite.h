#pragma once

#include "pch.h"
class SpriteManager;

class Sprite
{
public:
	Sprite(SpriteManager* manager, int id, SpriteSize size, SpriteColorFormat format, 
		   u16* memoryLocation, u16* data, Vector2i pixelSize = {32, 32});
	~Sprite();

	void update(float speedFactor = 1);
	void display(Vector2i pos = {0,0}, bool zoomed = false);
	void enableAnim(int frameCount = 1, int stateCount = 1, int animSpeed = 1);
	void skipFrame(int num = 1);
	void setState(int state);
	void setFrame(int frame);

	int getState();

	const Vector2i& getPixelSize() const;

private:
	SpriteManager* m_manager;
	int m_id;
	Vector2i m_pixelSize;
	SpriteSize m_size;
	SpriteColorFormat m_format;
	u16* m_memoryLocation;
	u16* m_data;

	// Animation-related members
	int m_currentFrame; // Currently displayed frame (x)
	int m_frameCount;   // Total number of animation frames
	int m_currentState; // Currently displayed state (y)
	int m_stateCount;   // Total number of animation states
	int m_animSpeed;    // Number of updates before switching frames
	int m_counter;      // Counts updates

	void updateOffset();
};
