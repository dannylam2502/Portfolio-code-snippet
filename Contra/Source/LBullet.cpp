#include "LBullet.h"
#include "Texture.h"
#include "Bill.h"
#include "PoolBullet.h"
#include <math.h>
_USE_MATH_DEFINES

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPoolBullet* PoolBullet;
#define BASE_VELOCITY 0.2f
#define BASE_WIDTH 10
#define BASE_HEIGHT 10
#define BASE_ANIMATE_RATE 50
#define X_OFFSET 25
#define Y_OFFSET 35


CLBullet::CLBullet(int Type, float X, float Y, int damage) :
CPlayerBullet(OBJ_PLAYER_BULLET, X, Y, 5)
{
	_Width = BASE_WIDTH;
	_Height = BASE_HEIGHT;
}

void CLBullet::Update(float delta)
{
	if (_Live_State == ALIVE)
	{
		if (_Directon.x != 0 && Bill->_X >= Bill->_X_Last) _X += (Bill->_Vx + _Vx) * delta;
		else _X += _Vx * delta;
		//_X += _Vx * delta;
		_Y += _Vy * delta;
		if (_X > G_ViewPort._X + G_ViewPort._Width || _X < G_ViewPort._X || _Y < G_ViewPort._Y || _Y > G_ViewPort._Y + G_ViewPort._Height)
		{
			Destroyed();
		}
		/*DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / BASE_ANIMATE_RATE)
		{
		_Last_Time = now;
		_AnimateIndex++;
		if (_AnimateIndex >= 2)
		{
		_AnimateIndex = 0;
		}
		}
		*/
	}
}
void CLBullet::Render()
{
	if (_Live_State == DESTROYED) return;
	//GameTexture->Render(GetRect(), this, G_ViewPort, M_PI_2);
	GameTexture->Render(GetRect(), this, G_ViewPort);
}
void CLBullet::Fire()
{
	/*if (_Live_State == DESTROYED)*/
	{
		_Live_State = ALIVE;

		_Directon = Bill->_direction;
		float angle = GetAngle(_Directon);
		_Vx = BASE_VELOCITY * cos(angle);
		_Vy = BASE_VELOCITY * sin(angle);

		if (_Directon.x == 0)
		{
			_Vx = 0;
			if (Bill->_State_Head == JUMP)
			{
				_X = Bill->_X - 2;
				_Y = Bill->_Y;
			}
			else
			{
				_X = Bill->_X - 2;
				_Y = Bill->_Y + 40;
			}
		}

		if (_Directon.y == 0)
		{
			_Vy = 0;
			if (Bill->_Facing == FACING_RIGHT)
			{
				_Y = Bill->_Y + 5;
				_X = Bill->_X + Bill->_Width;
			}
			else
			{
				_X = Bill->_X - X_OFFSET;
				_Y = Bill->_Y + 5;
			}
		}

		if (_Directon.x == 1 && _Directon.y == 1)
		{
			_X = Bill->_X + 15;
			_Y = Bill->_Y + Bill->_Height - 7;
		}

		if (_Directon.x == 1 && _Directon.y == -1)
		{

			if (Bill->_State_Head == JUMP)
			{
				_X = Bill->_X + 15;
				_Y = Bill->_Y - 15;
			}
			else
			{
				_X = Bill->_X + 18;
				_Y = Bill->_Y - 15;
			}
		}

		if (_Directon.x == -1 && _Directon.y == 1)
		{
			_X = Bill->_X - 23;
			_Y = Bill->_Y + Bill->_Height - 7;
		}

		if (_Directon.x == -1 && _Directon.y == -1)
		{
			if (Bill->_State_Head == JUMP)
			{
				_X = Bill->_X - 25;
				_Y = Bill->_Y - 15;
			}
			else
			{
				_X = Bill->_X - 25;
				_Y = Bill->_Y - 15;
			}

		}
		//if (_Vx > 0) _Vx = BASE_VELOCITY;
		//if (_Vx < 0) _Vx = -BASE_VELOCITY;
		//if (_Vy > 0) _Vy = BASE_VELOCITY;
		//if (_Vy < 0) _Vy = -BASE_VELOCITY;
		if (Bill->_State_Head == LIE)
		{
			_Y = Bill->_Y -7;
			if (Bill->_Facing == FACING_RIGHT)
				_X = Bill->_X + 20;
		}

	}
	//_Vy = 3.0f;
}

CLBullet::~CLBullet()
{
}