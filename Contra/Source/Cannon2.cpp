#include "Cannon2.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"

#include <iostream>
using namespace std;

extern CTexture* GameTexture;
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
#define CANNON_RED_BULLET_SPEED	0.08f
#define CANNON_ANIMATE_RATE 15
#define ANIMATION_CANNON2 3
#define ANIMATION_CANNON 4
#define CANNON_RANGE_ATTACK 120
#define CANNON_RANGE_ATTACK_APPEAR 140
#define CANNON_RED_TIME_WAIT		1800
#define CANNON_RED_MAX_ATTACK_COUNT	3
#define CANNON_RED_TIME_DIE	1000
#define CANNON_REDDIE_ANIMATION_COUNT 3
#define CANNON_RED_ATTACK_RATE		3
#define CANNON2_HP 10
#define Pi 3.14259
#define OFFSET 4

CCannon2::CCannon2(int Type, float pos_x, float pos_y, int width, int height) :CEnemy(Type, pos_x, pos_y, width, height)
{
	width = CANNON_RED_WIDTH;
	height = CANNON_RED_HEIGHT;
	_Facing = FACING_RIGHT;
	_HP = CANNON2_HP;
}
void CCannon2::LoadResource()
{
}
void CCannon2::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING) ProcessDying(CANNON_RED_TIME_DIE, CANNON_REDDIE_ANIMATION_COUNT, CANNON_RED_ATTACK_RATE);
}

void CCannon2::ProcessAlive(float delta)
{
	if (!is_InRange && (_X - Bill->_X) <= CANNON_RANGE_ATTACK_APPEAR && (_X - Bill->_X) > 0)
	{
		_AnimateIndex = 0;
		_state = CANNON2_APPEAR;
		is_InRange = true;
	}
	if (is_InRange && (_X - Bill->_X) < -CANNON_RANGE_ATTACK)
	{
		_state = CANNON2_CLOSING;
		is_InRange = false;
	}
	ProcessAttack(CANNON_RED_MAX_ATTACK_COUNT, CANNON_RED_TIME_WAIT, CANNON_RED_ATTACK_RATE, false);
}
void CCannon2::ProcessAttack(int Max_Attack_Count, int Time_Wait, int Attack_Rate, bool isAnimation)
{
	DWORD now = GetTickCount();
	if (now - last_animation > 1000 / 7)
	{
		GetNextAnimation();
		last_animation = now;
	}
	if (_Time_Remain > 0)
	{
		if (now - _Last_Time > 30)//FramRate
		{
			//GetNextAnimation();
			_Time_Remain -= (now - _Last_Time);
			_Last_Time = now;
		}
	}
	else
	{
		if (_Attack_Count < Max_Attack_Count)
		{
			if (now - _Last_Time > 1000 / Attack_Rate)
			{
				_Last_Time = now;
				if (isAnimation) GetNextAnimation(); // Nếu enemy có animation khi attack
				Attack();
				_Attack_Count++;
			}
		}
		else
		{
			_Time_Remain = Time_Wait;
			_Attack_Count = 0;
		}
	}
}
void CCannon2::GetNextAnimation()
{
	if (_state == CANNON2_APPEAR)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_CANNON2)
		{
			_state = CANNON2_LEFT;
			last_state = GetTickCount();
		}
	}
	if (_state == CANNON2_LEFT || _state == CANNON2_LEFT_UP || _state == CANNON2_LEFT_LEFT_UP)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_CANNON2)
		{
			_AnimateIndex = 0;
		}
	}
	if (_state == CANNON2_CLOSING)
	{
		_AnimateIndex--;
		if (_AnimateIndex <= -2)
		{
			_state = CANNON2_CLOSED;
		}
	}
}

void CCannon2::Attack()
{
	CheckTarget();
	Pool_Enemy_Bullet->Load(this);
	PlaySound(Enemy_Attack);
}

D3DXVECTOR3 CCannon2::CheckTarget()
{
	D3DXVECTOR3 p;
	float angle = Find_Angle();
	float y = sin(angle);

	if ((_X - Bill->_X) <= CANNON_RANGE_ATTACK && (_X - Bill->_X)>0)
	{
		if (y > sin(11 * Pi / 12) && y < sin(3 * Pi / 4))
		{
			_state = CANNON2_LEFT_UP;
			p.y = sin(8 * Pi / 9)*CANNON_RED_BULLET_SPEED;
			p.x = cos(8 * Pi / 9)*CANNON_RED_BULLET_SPEED;
		}
		else if (y > sin(3 * Pi / 4) && y < sin(Pi / 2))
		{
			_state = CANNON2_LEFT_LEFT_UP;
			p.y = sin(Pi / 3)*CANNON_RED_BULLET_SPEED;
			p.x = -cos(Pi / 3)*CANNON_RED_BULLET_SPEED;
		}
		else if (y < sin(11 * Pi / 12) && y>sin(-11 * Pi / 12))
		{
			_state = CANNON2_LEFT;
			p.y = 0;
			p.x = -1 * CANNON_RED_BULLET_SPEED;
		}
		else
		{
			_state = CANNON2_LEFT;
			p.y = 0;
			p.x = -1 * CANNON_RED_BULLET_SPEED;
		}
		p.z = 0.0f;
		return p;
	}
}
D3DXVECTOR2 CCannon2::Find_XY()
{
	D3DXVECTOR2 result;
	if (_state == CANNON2_LEFT_UP)
	{
		result.x = _X - OFFSET;
		result.y = _Y + CANNON_RED_HEIGHT / 2 + OFFSET;
	}
	if (_state == CANNON2_LEFT_LEFT_UP)
	{
		result.x = _X - OFFSET;
		result.y = _Y + CANNON_RED_HEIGHT + OFFSET;
	}
	if (_state == CANNON2_LEFT)
	{
		result.x = _X - OFFSET;
		result.y = _Y + CANNON_RED_HEIGHT / 2 - OFFSET;
	}
	return result;
}
CCannon2::~CCannon2()
{
}

