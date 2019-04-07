#include "SmallSpaceShip.h"
#include "Texture.h"
#include "Bill.h"

#define ANIMATION_SMALLSPACESHIP		3
#define SMALLSPACESHIP_ANIMATE_RATE		10
#define SMALLSPACESHIP_SPEED			0.1
#define SMALLSPACESHIP_LEFT				0
#define SMALLSPACESHIP_RIGHT			1

#define SMALLSHIP_DIE_TIME				500
#define SMALLSHIP_DIE_ANIMATION_COUNT	3
#define SMALLSHIP_DIE_ANIMATION_RATE	7


extern CTexture * GameTexture;
extern CBill* Bill;

CSmallSpaceShip::CSmallSpaceShip()
{
}

CSmallSpaceShip::CSmallSpaceShip(int type, float x, float y, int width, int height, int _direction) : CEnemy(type, x, y, width, height)
{
	_HP = 1;
	_Live_State = ALIVE;
	direction = _direction;
	//state = SMALLSHIP_DESTROYED;
}

CSmallSpaceShip::~CSmallSpaceShip()
{
}

void CSmallSpaceShip::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_SMALLSPACESHIP) _AnimateIndex = 0;
}

void CSmallSpaceShip::Update(float dt)
{
	if (_Live_State == ALIVE)
	{
		_X += _Vx*dt;
		_Y += _Vy*dt;

		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / SMALLSPACESHIP_ANIMATE_RATE)
		{
			_Last_Time = now;
			GetNextAnimation();
		}

		if (direction == SMALLSPACESHIP_LEFT)
		{
			if (_X > G_ViewPort._X + 16 && _Y > 64)
				_Vx = -SMALLSPACESHIP_SPEED;
			if (_X <= G_ViewPort._X + 16)
			{
				_Vy = -SMALLSPACESHIP_SPEED;
				_Vx = 0;
			}
			if (_Y < 64)
			{
				_Vy = 0;
				_Vx = SMALLSPACESHIP_SPEED;
			}
		}
		else
		{
			if (_X < G_ViewPort._X + G_ViewPort._Width - 44 && _Y > 64)
				_Vx = SMALLSPACESHIP_SPEED;
			if (_X >= G_ViewPort._X + G_ViewPort._Width - 44)
			{
				_Vy = -SMALLSPACESHIP_SPEED;
				_Vx = 0;
			}
			if (_Y < 64)
			{
				_Vy = 0;
				_Vx = -SMALLSPACESHIP_SPEED;
			}
		}
	}

	if (_Live_State == DYING)
	{
		_Vx = _Vy = 0;
		ProcessDying(SMALLSHIP_DIE_TIME, SMALLSHIP_DIE_ANIMATION_COUNT, SMALLSHIP_DIE_ANIMATION_RATE);
	}

	if (_X > G_ViewPort._X + G_ViewPort._Width || _X < G_ViewPort._X - 28)
		_Live_State = DESTROYED;
}

void CSmallSpaceShip::Render()
{
	if (_Live_State == ALIVE)
		GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}
