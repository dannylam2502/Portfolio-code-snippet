#include "Tank.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

#define ANIMATION_TANK		 2
#define TANK_ANIMATE_RATE	 10
#define TANK_SPEED			0.03f
#define TANK_RANGE			200
#define pi					3.14

#define TIME_STOP					8000
#define TANK_TIME_WAIT				1500
#define TANK_TIME_DIE				1000
#define TANK_DIE_ANIMATION_COUNT	7
#define TANK_MAX_ATTACK_COUNT		3
#define TANK_ATTACK_RATE			2
#define TANK_DYING_RATE				5
#define TANK_HP						85

CTank::CTank(int Type, float _x, float _y, int width, int height) :CEnemy(OBJ_TANK, _x, _y, width, height)
{
	_Live_State = ALIVE;
	_HP = TANK_HP;
}

void CTank::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE)
	{
		ProcessAlive(dt);
	}
	if (_Live_State == DYING)
	{
		_Render_Color = D3DCOLOR_XRGB(255, 255, 255);

		this->_Facing = FACING_RIGHT;
		PlaySound(Destroy_Bridge,10);
		ProcessDying(TANK_TIME_DIE, TANK_DIE_ANIMATION_COUNT, TANK_DYING_RATE);
	}

}
void CTank::ProcessAlive(float delta)
{
	DWORD now = GetTickCount();
	if (IsMove)
	{
		if (_X > G_ViewPort._X + G_ViewPort._Width / 2)
			_Vx = -TANK_SPEED;
		else
		{
			if (_Vx != 0)
				_Last_Check = GetTickCount();
			_Vx = 0;
		}
	}

	if (now - _Last_Check > TIME_STOP)
	{
		if (_Last_Check != 0)
			IsMove = false;
		_Vx = -TANK_SPEED;
	}

	_X += _Vx * delta;
	_Y += _Vy * delta;
	if (_X <= G_ViewPort._X + G_ViewPort._Width / 2)
		ProcessAttack(TANK_MAX_ATTACK_COUNT, TANK_TIME_WAIT, TANK_ATTACK_RATE, false);

	if (now - _Last_Animation > 1000 / TANK_ANIMATE_RATE)
	{
		_Last_Animation = now;
		if (_Vx != 0)
			GetNextAnimation();
	}

	if (_HP > 40 && _HP < 60)
		_Render_Color = D3DCOLOR_XRGB(255, 172, 76);
	if (_HP > 20 && _HP <= 40)
		_Render_Color = D3DCOLOR_XRGB(255, 140, 124);
	if (_HP <= 20)
		_Render_Color = D3DCOLOR_XRGB(228, 68, 52);
}


void CTank::Render()
{
	if (_Live_State == DYING)
	{
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
	}
	if (_Live_State == ALIVE)
		GameTexture->Render(GetRect(), this, G_ViewPort);
}
void CTank::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_TANK) _AnimateIndex = 0;
}

void CTank::Attack()
{
	CheckDirection();
	Pool_Enemy_Bullet->Load(this);
	PlaySound(Enemy_Attack);
}

D3DXVECTOR3 CTank::CheckTarget()
{
	CheckDirection();
	D3DXVECTOR3 result;
	float a = _X - Bill->_X;
	float b = Bill->_Y - (_Y + TANK_HEIGHT);

	float c = sqrt((a*a + b*b));

	//result.y = sin( b / c)*0.1f;
	//result.x = -cos(a / c)*0.1f;
	result.z = 0;

	if (50 <= a && a < 400 && b < -TANK_HEIGHT / 2)
	{
		_state = TANK_RIGHT_DOWN;
		result.x = -cos(pi / 6)*0.1f;
		result.y = sin(-pi / 4 + 30 * pi / 180)*0.1f;
	}
	else
	if (0 < a && a < 50 && b < -TANK_HEIGHT / 2)
	{
		_state = TANK_DOWN;
		result.x = -cos(a / c)*0.1f;
		result.y = sin(b / c)*0.1f;
	}
	else
	{
		_state = TANK_RIGHT;
		result.x = -0.1f;
		result.y = 0;
	}
	return result;
}
D3DXVECTOR2 CTank::Find_XY()
{
	D3DXVECTOR2 result;
	switch (_state)
	{
	case TANK_RIGHT:
		result.x = _X;
		result.y = _Y + 3 * TANK_HEIGHT / 4;
		break;
	case TANK_DOWN:
		result.x = _X;
		result.y = _Y + TANK_HEIGHT / 3;
		break;
	case TANK_RIGHT_DOWN:
		result.x = _X;
		result.y = _Y + 3 * TANK_HEIGHT / 5;
		break;
	default:
		break;
	}
	return result;
}

CTank::~CTank()
{
}
