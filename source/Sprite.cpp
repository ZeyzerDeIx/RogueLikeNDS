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

	m_Manager(manager),
	m_PixelSize(pixelSize),
	m_SpriteSize(spriteSize),
	m_Data(data),
	m_Id(id),
	
	// Initialize animation-related members
	m_CurrentFrame(0),
	m_FrameCount(frameCount),
	m_CurrentState(0),
	m_StateCount(stateCount),
	m_AnimSpeed(animSpeed),
	m_Counter(0),
	m_FrameMemoryOffset(pixelSize.x * pixelSize.y / 4)
{
}

Sprite::~Sprite()
{
	for (int i = 0 ; i < m_FrameCount*m_StateCount ; i++)
    	oamFreeGfx(&oamMain, m_Data + i*m_FrameMemoryOffset);
}

void Sprite::Update(float speedFactor)
{
	if (m_AnimSpeed == 0) return;
	if (++m_Counter >= m_AnimSpeed * speedFactor)
	{
		SkipFrame();
		m_Counter = 0;
	}
}

void Sprite::Display(Vector2i pos, bool zoomed)
{
	float scale = zoomed ? 0.5f : 1.f;
	oamRotateScale(&oamMain, m_Id, 0, 256.f * scale, 256.f * scale);
	oamSet(&oamMain,
		   m_Id,
		   pos.x, pos.y,
		   0, //priority
		   m_Id, // palette_alpha
		   m_SpriteSize,
		   SpriteColorFormat_16Color, //systematic
		   m_Data + (m_CurrentFrame + m_CurrentState * m_FrameCount) * m_FrameMemoryOffset,
		   0, //affine index
		   zoomed, //sizeDouble
		   false, false, false, false);
}

void Sprite::SkipFrame(int num)
{
	// Modulo ensures that we do not set a non-existent frame
	m_CurrentFrame = (m_CurrentFrame + num) % m_FrameCount;
	//updateOffset();
}

void Sprite::SetState(int state)
{
	// Modulo ensures that we do not set a non-existent state
	m_CurrentState = state % m_StateCount;
	//updateOffset();
}

void Sprite::SetFrame(int frame)
{
	// Ensure frame does not exceed allowed range
	m_CurrentFrame = std::min(frame, m_FrameCount - 1);
	//updateOffset();
}

int Sprite::GetState() { return m_CurrentState; }

const Vector2i& Sprite::getPixelSize() const
{
	return m_PixelSize;
}
