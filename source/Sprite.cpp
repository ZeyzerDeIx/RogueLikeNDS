#include "Sprite.h"

using namespace std;
#define W 0
#define H 1

Sprite::Sprite(SpriteManager* manager, int id, SpriteSize size, SpriteColorFormat format, u16* memoryLocation, u16* data, int w, int h)
{
	m_manager = manager;
	m_id = id;
	m_pixelSize[W] = w;
	m_pixelSize[H] = h;
	m_size = size;
	m_format = format;
	m_memoryLocation = memoryLocation;
	m_data = data;
	m_currentFrame = 0;
	m_currentState = 0;
	m_counter = 0;
}
Sprite::~Sprite() {}

void Sprite::update()
{
	if(m_animSpeed == 0) return;
	if (++m_counter >= m_animSpeed) skipFrame(), m_counter = 0;
}

void Sprite::display(int x, int y, bool zoomed)
{
	oamRotateScale(&oamMain, m_id, 0, 256.f*(zoomed?0.5f:1.f), 256.f*(zoomed?0.5f:1.f));
	oamSet(&oamMain, m_id, x, y, 0, m_id, m_size, m_format, m_memoryLocation,0,zoomed,false,false,false,false);
}

void Sprite::enableAnim(int frameCount, int stateCount, int animSpeed)
{
	m_frameCount = frameCount;
	m_stateCount = stateCount;
	m_animSpeed = animSpeed;
}

void Sprite::skipFrame(int num)
{
	//si l'animation dépasse le nombre total de frame on la réinitialise
	if(m_currentFrame >= m_frameCount-1) m_currentFrame = 0;
	else m_currentFrame++;
	updateOffset();
}

void Sprite::setState(int state)
{
	if(state == m_currentState) return;
	//le modulo assure qu'on ne mette pas un état qui n'existe pas
	m_currentState = state%m_stateCount;
	//debug.end().log("State",m_currentState);
	updateOffset();
}

void Sprite::setFrame(int frame)
{
	if(frame == m_currentFrame) return;
	m_currentFrame = frame<m_frameCount?frame:(m_frameCount-1);
	//debug.end().log("State",m_currentState);
	updateOffset();
}

void Sprite::updateOffset()
{
	//taille brut en pixel d'une image
	int pxlSzBrut = m_pixelSize[H]*m_pixelSize[W];
	//calcul du décalage entre le début de l'image et l'image à charger
	int offset = (int)m_data + (m_currentFrame + m_currentState*m_frameCount)*pxlSzBrut/2;
	//chargement de l'image voulue
	dmaCopy((u16*)offset, m_memoryLocation, pxlSzBrut/2);


	/*Note: Dans les 2 lignes au dessus, il y a un /2, je ne saisie pas pourquoi mais il est primordial... même chose pour les convertions en int et u16*, je capte pas...*/
}

int Sprite::getPixelSize(bool xOrY)
{
	return m_pixelSize[xOrY];
}