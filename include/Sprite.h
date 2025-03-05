#pragma once

#include "PCH.h"
class SpriteManager;

class Sprite
{
	public:
		Sprite(SpriteManager* manager, int id, SpriteSize size, SpriteColorFormat format, u16* memoryLocation, u16* data, Vector2i pixelSize = {32, 32});
		~Sprite();
		void update();
		void display(int x = 0, int y = 0, bool zoomed = false);
		void enableAnim(int frameCount = 1, int stateCount = 1, int animSpeed = 1);
		void skipFrame(int num = 1);
		void setFrame(int frame);
		void setState(int state);
		const Vector2i&  getPixelSize();
	private:
		SpriteManager* m_manager;
		int m_id;
		Vector2i m_pixelSize;
		SpriteSize m_size;
		SpriteColorFormat m_format;
		u16* m_memoryLocation;
		u16* m_data;
		//partie animation
		int m_currentFrame; //frame acutellement affichée (x)
		int m_frameCount;   //nombre max de frame de l'animation
		int m_currentState; //état actuellement affiché (y)
		int m_stateCount;   //nombre max d'état de l'animation
		int m_animSpeed;    //nombre d'update entre chaque passage de frame
		int m_counter;      //compte les update

		void updateOffset();
};