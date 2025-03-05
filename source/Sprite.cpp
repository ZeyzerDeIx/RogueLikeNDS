#include "Sprite.h"

using namespace std;

Sprite::Sprite(
	SpriteManager* manager,
	int id,
	SpriteSize size, SpriteColorFormat format,
	u16* memoryLocation,
	u16* data,
	Vector2i pixelSize) :

	m_manager(manager),
	m_id(id),
	m_pixelSize(pixelSize),
	m_size(size),
	m_format(format),
	m_memoryLocation(memoryLocation),
	m_data(data),
	
	// Initialize animation-related members
	m_currentFrame(0),
	m_frameCount(1),
	m_currentState(0),
	m_stateCount(1),
	m_animSpeed(1),
	m_counter(0)
{
}

Sprite::~Sprite()
{
	oamFreeGfx(&oamMain, m_memoryLocation);
}

void Sprite::update()
{
	if (m_animSpeed == 0) return;
	if (++m_counter >= m_animSpeed)
	{
		skipFrame();
		m_counter = 0;
	}
}

void Sprite::display(int x, int y, bool zoomed)
{
	float scale = zoomed ? 0.5f : 1.f;
	oamRotateScale(&oamMain, m_id, 0, 256.f * scale, 256.f * scale);
	oamSet(&oamMain, m_id, x, y, 0, m_id, m_size, m_format, m_memoryLocation, 0, zoomed, false, false, false, false);
}

void Sprite::enableAnim(int frameCount, int stateCount, int animSpeed)
{
	m_frameCount = frameCount;
	m_stateCount = stateCount;
	m_animSpeed = animSpeed;
}

void Sprite::skipFrame(int num)
{
	// Modulo ensures that we do not set a non-existent frame
	m_currentFrame = (m_currentFrame + num) % m_frameCount;
	updateOffset();
}

void Sprite::setState(int state)
{
	// Modulo ensures that we do not set a non-existent state
	m_currentState = state % m_stateCount;
	updateOffset();
}

void Sprite::setFrame(int frame)
{
	// Ensure frame does not exceed allowed range
	m_currentFrame = std::min(frame, m_frameCount - 1);
	updateOffset();
}

void Sprite::updateOffset()
{
	// Raw pixel size of a single sprite frame
	int rawPixelSize = m_pixelSize.x * m_pixelSize.y;

	// Compute the offset of the frame to load
	int offset = reinterpret_cast<int>(m_data) +
		(m_currentFrame + m_currentState * m_frameCount) * (rawPixelSize >> 1);

	// Load the desired sprite frame
	dmaCopy(reinterpret_cast<u16*>(offset), m_memoryLocation, rawPixelSize >> 1);

	/* Note: The division by 2 (>>1) is essential, although its necessity is unclear.
	*  Similarly, the int and u16* conversions are required for correct behavior. */
}

const Vector2i& Sprite::getPixelSize()
{
	return m_pixelSize;
}
