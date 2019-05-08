#include "BasicBullet.h"
#include "Texture.h"
#include "Bill.h"
#include "PoolBullet.h"
#include "Resources.h"

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPoolBullet* PoolBullet;
#define BASE_VELOCITY 0.2f
#define BASE_WIDTH 5
#define BASE_HEIGHT 5
#define BASE_ANIMATE_RATE 50
#define X_OFFSET 24
#define Y_OFFSET 36

CBasicBullet::CBasicBullet(int Type, float X, float Y, int damage) :
CPlayerBullet(OBJ_PLAYER_BULLET, X, Y, 1)
{
	_Width = BASE_WIDTH;
	_Height = BASE_HEIGHT;
}

void CBasicBullet::Update(float delta)
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
		if (now - _Last_Time > 1000 / BASE_ANIMATE_RATE)
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
void CBasicBullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);


}
void CBasicBullet::Fire()
{
	if (_Live_State == DESTROYED)
	{
		_Live_State = ALIVE;

		_Directon = Bill->_direction;
		_Angle = GetAngle(_Directon);
		_Vx = BASE_VELOCITY * cos(_Angle);
		_Vy = BASE_VELOCITY * sin(_Angle);

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

		//if (_Vx > 0) _Vx = BASE_VELOCITY;
		//if (_Vx < 0) _Vx = -BASE_VELOCITY;
		//if (_Vy > 0) _Vy = BASE_VELOCITY;
		//if (_Vy < 0) _Vy = -BASE_VELOCITY;
		if (Bill->_State_Head == LIE)
		{
			_Y = Bill->_Y + Y_OFFSET / 6;
		}
	}

	//_Vy = 3.0f;
}

CBasicBullet::~CBasicBullet()
{
}