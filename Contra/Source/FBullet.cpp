#include "BasicBullet.h"
#include "Texture.h"
#include "Bill.h"
#include "PoolBullet.h"
#include "Resources.h"

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPoolBullet* PoolBullet;
#define F_VELOCITY 0.1f
#define F_WIDTH 10
#define F_HEIGHT 10
#define X_OFFSET 25
#define Y_OFFSET 35

CFBullet::CFBullet(int Type, float X, float Y, int damage) :
CPlayerBullet(OBJ_PLAYER_BULLET, X, Y, 1)
{
	_Width = F_WIDTH;
	_Height = F_HEIGHT;
	_X_Center = _Y_Center = 0;
	_Angle_W = 0.0f;
}

void CFBullet::Update(float delta)
{
	if (_Live_State == ALIVE)
	{
		if (_Directon.x != 0 && Bill->_X >= Bill->_X_Last)
		{
			_X_Center += (Bill->_Vx * cos(_Angle) + _Vx) * delta;
			_Y_Center += _Vy * delta;
		}
		else
		{
			_X_Center += _Vx * delta;
			_Y_Center += _Vy * delta;
		}

		_X = _X_Center - 20 * sin(_Angle_W);
		_Y = _Y_Center - 15 * cos(_Angle_W);
		_Angle_W += 0.35f;
		if (_X > G_ViewPort._X + G_ViewPort._Width || _X < G_ViewPort._X || _Y < G_ViewPort._Y || _Y > G_ViewPort._Y + G_ViewPort._Height)
		{
			Destroyed();
		}
	}
}
void CFBullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);


}
void CFBullet::Fire()
{
	if (_Live_State == DESTROYED)
	{
		_Live_State = ALIVE;

		_Directon = Bill->_direction;
		_Angle = GetAngle(_Directon);
		_Vx = F_VELOCITY * cos(_Angle);
		_Vy = F_VELOCITY * sin(_Angle);

		switch (_Directon.x)
		{
		case 1: _X_Center = Bill->_X + X_OFFSET; break;
		case -1: _X_Center = Bill->_X - 5.0f; break;
		case 0:
			_Vx = 0;
			if (_Facing == FACING_RIGHT) _X_Center = Bill->_X + X_OFFSET / 2 + _Facing * 2.0f;
			else _X_Center = Bill->_X + X_OFFSET / 2 - 5.0f;
			break;
		default:
			break;
		}
		switch (_Directon.y)
		{
		case 1: _Y_Center = Bill->_Y + Y_OFFSET; break;
		case -1: _Y_Center = Bill->_Y; break;
		case 0: _Vy = 0; _Y_Center = Bill->_Y + Bill->_Height / 2 + 2; break;
		default:
			break;
		}
		if (Bill->_State_Head == JUMP)
		{
			_Y_Center = Bill->_Y + Y_OFFSET / 6;
		}
		//if (_Vx > 0) _Vx = F_VELOCITY;
		//if (_Vx < 0) _Vx = -F_VELOCITY;
		//if (_Vy > 0) _Vy = F_VELOCITY;
		//if (_Vy < 0) _Vy = -F_VELOCITY;
		if (Bill->_State_Head == LIE)
		{
			_Y = Bill->_Y + Y_OFFSET / 6;
		}
	}
	//_Vy = 3.0f;
}

CFBullet::~CFBullet()
{
}