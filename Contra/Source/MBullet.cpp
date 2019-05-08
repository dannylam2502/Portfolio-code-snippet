#include "MBullet.h"
#include "Texture.h"
#include "Bill.h"
#include "PoolBullet.h"

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPoolBullet* PoolBullet;
#define M_VELOCITY 0.2f
#define M_WIDTH 10
#define M_HEIGHT 10
#define M_ANIMATE_RATE 50
#define X_OFFSET 25
#define Y_OFFSET 35

CMBullet::CMBullet(int Type, float X, float Y, int damage) :
CPlayerBullet(OBJ_PLAYER_BULLET, X, Y, 1)
{
	_Width = M_WIDTH;
	_Height = M_HEIGHT;
}

void CMBullet::Update(float delta)
{
	if (_Live_State == ALIVE)
	{
		if (_Directon.x != 0 && Bill->_X >= Bill->_X_Last)
		{
			_X += (Bill->_Vx * cos(_Angle) + _Vx) * delta;
			_Y += _Vy * delta;
		}
		else
		{
			_X += _Vx * delta;
			_Y += _Vy * delta;
		}
		if (_X > G_ViewPort._X + G_ViewPort._Width || _X < G_ViewPort._X || _Y < G_ViewPort._Y || _Y > G_ViewPort._Y + G_ViewPort._Height)
		{
			Destroyed();
		}
		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / M_ANIMATE_RATE)
		{
			_Last_Time = now;
			_AnimateIndex++;
			if (_AnimateIndex >= 2)
			{
				_AnimateIndex = 0;
			}
		}

	}
}
void CMBullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);


}
void CMBullet::Fire()
{
	if (_Live_State == DESTROYED)
	{
		_Live_State = ALIVE;

		_Directon = Bill->_direction;
		_Angle = GetAngle(_Directon);
		_Vx = M_VELOCITY * cos(_Angle);
		_Vy = M_VELOCITY * sin(_Angle);

		switch (_Directon.x)
		{
		case 1: _X = Bill->_X + X_OFFSET; break;
		case -1: _X = Bill->_X - 5.0f; break;
		case 0:
			_Vx = 0;
			if (_Facing == FACING_RIGHT) _X = Bill->_X + X_OFFSET / 2 + _Facing * 2.0f;
			else _X = Bill->_X + X_OFFSET / 2 - 5.0f;
			break;
		default:
			break;
		}
		switch (_Directon.y)
		{
		case 1: _Y = Bill->_Y + Y_OFFSET; break;
		case -1: _Y = Bill->_Y; break;
		case 0: _Vy = 0; _Y = Bill->_Y + Bill->_Height / 2 + 2; break;
		default:
			break;
		}
		if (Bill->_State_Head == JUMP)
		{
			_Y = Bill->_Y + Y_OFFSET / 2;
		}
		//if (_Vx > 0) _Vx = M_VELOCITY;
		//if (_Vx < 0) _Vx = -M_VELOCITY;
		//if (_Vy > 0) _Vy = M_VELOCITY;
		//if (_Vy < 0) _Vy = -M_VELOCITY;
		if (Bill->_State_Head == LIE)
		{
			_Y = Bill->_Y + Y_OFFSET / 6;
		}

	}
	//_Vy = 3.0f;
}

CMBullet::~CMBullet()
{
}