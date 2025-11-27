#include "Sprite.h"

using namespace std;

Sprite::Sprite(
	SpriteManager* manager,
	int id,
	SpriteSize spriteSize,
	u16* data,
	Vector2i pixelSize,
	int frameCount,
	int stateCount,
	int animSpeed) :

	m_manager(manager),
	m_pixelSize(pixelSize),
	m_spriteSize(spriteSize),
	m_data(data),
	m_id(id),
	
	// Initialize animation-related members
	m_currentFrame(0),
	m_frameCount(frameCount),
	m_currentState(0),
	m_stateCount(stateCount),
	m_animSpeed(animSpeed),
	m_counter(0),
	m_frameMemoryOffset(pixelSize.x * pixelSize.y / 4)
{
}

Sprite::~Sprite()
{
	for (int i = 0 ; i < m_frameCount*m_stateCount ; i++)
    	oamFreeGfx(&oamMain, m_data + i*m_frameMemoryOffset);
}

void Sprite::update(float speedFactor)
{
	if (m_animSpeed == 0) return;
	if (++m_counter >= m_animSpeed * speedFactor)
	{
		skipFrame();
		m_counter = 0;
	}
}

void Sprite::display(Vector2i pos, bool zoomed)
{
	float scale = zoomed ? 0.5f : 1.f;
	oamRotateScale(&oamMain, m_id, 0, 256.f * scale, 256.f * scale);
	oamSet(&oamMain,
		   m_id,
		   pos.x, pos.y,
		   0, //priority
		   m_id, // palette_alpha
		   m_spriteSize,
		   SpriteColorFormat_16Color, //systematic
		   m_data + (m_currentFrame + m_currentState * m_frameCount) * m_frameMemoryOffset,
		   0, //affine index
		   zoomed, //sizeDouble
		   false, false, false, false);
}

void Sprite::skipFrame(int num)
{
	// Modulo ensures that we do not set a non-existent frame
	m_currentFrame = (m_currentFrame + num) % m_frameCount;
	//updateOffset();
}

void Sprite::setState(int state)
{
	// Modulo ensures that we do not set a non-existent state
	m_currentState = state % m_stateCount;
	//updateOffset();
}

void Sprite::setFrame(int frame)
{
	// Ensure frame does not exceed allowed range
	m_currentFrame = std::min(frame, m_frameCount - 1);
	//updateOffset();
}

int Sprite::getState() { return m_currentState; }

const Vector2i& Sprite::getPixelSize() const
{
	return m_pixelSize;
}
