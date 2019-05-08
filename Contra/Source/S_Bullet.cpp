#include "S_Bullet.h"
#include "Texture.h"
#include "Bill.h"
#include "PoolBullet.h"

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPoolBullet* PoolBullet;

#define BASE_VELOCITY 0.15f
#define BASE_WIDTH 10
#define BASE_HEIGHT 10
#define BASE_ANIMATE_RATE 50
#define X_OFFSET 5
#define Y_OFFSET 7

CS_Bullet::CS_Bullet(int Type, float X, float Y, int damage) :
CPlayerBullet(OBJ_PLAYER_BULLET, X, Y, 1)
{
	_Width = BASE_WIDTH;
	_Height = BASE_HEIGHT;
}

void CS_Bullet::Update(float delta)
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
void CS_Bullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);


}
void CS_Bullet::Fire(int NumOfBullet)
{
	if (_Live_State == DESTROYED)
	{
		_Live_State = ALIVE;
		_Directon = Bill->_direction;
		_Angle = GetAngle(_Directon);
		/*switch (_Directon.x)
		{
		case 1:
		switch (NumOfBullet)
		{
		case 0:
		break;
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
		case 4:
		break;
		}
		break;
		case -1: _Vx = -BASE_VELOCITY; _X = Bill->_X - X_OFFSET; break;
		case 0: _Vx = 0; _X = Bill->_X + Bill->_Width / 2; break;
		default:
		break;
		}
		switch (_Directon.y)
		{
		case 1: _Vy = BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height; break;
		case -1: _Vy = -BASE_VELOCITY; _Y = Bill->_Y; break;
		case 0: _Vy = 0; _Y = Bill->_Y + Bill->_Height / 2; break;
		default:
		break;
		}*/

		if (Bill->_State_Head == LIE)
		{
			_Y = Bill->_Y + Y_OFFSET;
		}

		if (_Directon.x == 0)
		{
			switch (NumOfBullet)
			{
			case 0:
				_Vx = 0;
				_Vy = _Directon.y * BASE_VELOCITY;
				break;
			case 1:
				_Vx = 1.0 / 8 * BASE_VELOCITY;
				_Vy = _Directon.y *0.985F * BASE_VELOCITY;
				break;
			case 2:
				_Vx = -1.0 / 8 * BASE_VELOCITY;
				_Vy = _Directon.y *0.985F * BASE_VELOCITY;
				break;
			case 3:
				_Vx = 1.0 / 4 * BASE_VELOCITY;
				_Vy = _Directon.y *0.96F * BASE_VELOCITY;
				break;
			case 4:
				_Vx = -1.0 / 4 * BASE_VELOCITY;;
				_Vy = _Directon.y *0.96F * BASE_VELOCITY;
				break;
			}

			if (Bill->_State_Head == JUMP)
			{
				_X = Bill->_X + 7;
				_Y = Bill->_Y;
			}
			else
			{
				_X = Bill->_X + 7;
				_Y = Bill->_Y + 40;
			}
		}

		if (_Directon.y == 0)
		{
			switch (NumOfBullet)
			{
			case 0:
				_Vx = _Directon.x*BASE_VELOCITY;
				_Vy = 0;
				break;
			case 1:
				_Vx = _Directon.x*0.985F*BASE_VELOCITY;
				_Vy = 1.0 / 8 * BASE_VELOCITY;
				break;
			case 2:
				_Vx = _Directon.x*0.985f*BASE_VELOCITY;
				_Vy = -1.0 / 8 * BASE_VELOCITY;
				break;
			case 3:
				_Vx = _Directon.x*0.96f*BASE_VELOCITY;
				_Vy = 1.0 / 4 * BASE_VELOCITY;
				break;
			case 4:
				_Vx = _Directon.x*0.96f*BASE_VELOCITY;
				_Vy = -1.0 / 4 * BASE_VELOCITY;
				break;
			}

			if (Bill->_Facing == FACING_RIGHT)
			{
				_Y = Bill->_Y + Bill->_Height / 2;
				_X = Bill->_X + Bill->_Width + X_OFFSET;
			}
			else
			{
				_X = Bill->_X - X_OFFSET;
				_Y = Bill->_Y + Bill->_Height / 2;
			}
		}

		if (_Directon.x != 0 && _Directon.y != 0)
		{
			switch (NumOfBullet)
			{
			case 0:
				_Vx = _Directon.x*BASE_VELOCITY;
				_Vy = _Directon.y*BASE_VELOCITY;
				break;
			case 1:
				_Vx = _Directon.x*1.1f*BASE_VELOCITY;
				_Vy = _Directon.y*0.85f*BASE_VELOCITY;
				break;
			case 2:
				_Vx = _Directon.x*0.85f*BASE_VELOCITY;
				_Vy = _Directon.y*1.1f*BASE_VELOCITY;
				break;
			case 3:
				_Vx = _Directon.x*1.15f*BASE_VELOCITY;
				_Vy = _Directon.y*0.7f*BASE_VELOCITY;
				break;
			case 4:
				_Vx = _Directon.x*0.7f*BASE_VELOCITY;
				_Vy = _Directon.y*1.15f*BASE_VELOCITY;
				break;
			}

			if (_Directon.x == 1 && _Directon.y == 1)
			{
				_X = Bill->_X + 15;
				_Y = Bill->_Y + Bill->_Height - 7;
			}

			if (_Directon.x == 1 && _Directon.y == -1)
			{
				_X = Bill->_X + 18;
				_Y = Bill->_Y + 3;
			}

			if (_Directon.x == -1 && _Directon.y == 1)
			{
				_X = Bill->_X;
				_Y = Bill->_Y + Bill->_Height - 7;
			}

			if (_Directon.x == -1 && _Directon.y == -1)
			{
				_X = Bill->_X - 7;
				_Y = Bill->_Y + 3;
			}
		}
		if (Bill->_State_Head == LIE)
		{
			if (Bill->_Facing == FACING_RIGHT)
			{
				_Y = Bill->_Y + 5.0f;
				_X = Bill->_X + 35;
			}
			else
			{
				_Y = Bill->_Y + 5.0f;
				_X = Bill->_X - 5;
			}
		}
		/*if (_Directon.x == 1 && _Directon.y == -1)
		{
		switch (NumOfBullet)
		{
		case 0:
		_Vx = BASE_VELOCITY; _X = Bill->_X + Bill->_Width + X_OFFSET;
		_Vy = -BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height / 2;
		break;
		case 1:
		_Vx = 1.1*BASE_VELOCITY; _X = Bill->_X + Bill->_Width + X_OFFSET;
		_Vy = -0.85*BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height / 2;
		break;
		case 2:
		_Vx = 0.85*BASE_VELOCITY; _X = Bill->_X + Bill->_Width + X_OFFSET;
		_Vy = -1.1*BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height / 2;
		break;
		case 3:
		_Vx = 1.15*BASE_VELOCITY; _X = Bill->_X + Bill->_Width + X_OFFSET;
		_Vy = -0.7*BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height / 2;
		break;
		case 4:
		_Vx = 0.7*BASE_VELOCITY; _X = Bill->_X + Bill->_Width + X_OFFSET;
		_Vy = -1.15*BASE_VELOCITY; _Y = Bill->_Y + Bill->_Height / 2;
		break;
		}
		}*/

	}
	//_Vy = 3.0f;
}

CS_Bullet::~CS_Bullet()
{
}