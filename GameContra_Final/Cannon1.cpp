#include "Cannon1.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"
#include <Math.h>
#include "PoolBullet.h"
#include "WorldMap.h"


#include <iostream>
using namespace std;
extern CTexture* GameTexture;
#define ANIMATION_CANNON1				3
#define CANNON_ANIMATE_RATE				10
#define CANNON_RANGE_ATTACK1		120
#define CANNON_RANGE_ATTACK2		90
#define CANNON_RANGE_CHANGE				15
#define pi 3.14
#define OFSET 4

#define CANNON_TIME_WAIT		1800
#define CANNON_TIME_WAIT2		1000
#define CANNON_TIME_DIE	1000
#define CANNON_DIE_ANIMATION_COUNT 3
#define CANNON_MAX_ATTACK_COUNT	1
#define CANNON_MAX_ATTACK_COUNT2	3
#define CANNON_ATTACK_RATE		3
#define CANNON_HP 10;
#define HP_CANNON_CLOSE 100000
#define CANNON_WHITE_BULLET_SPEED 0.08f
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
extern CPoolBullet* Pool_Bullet;
extern STAGE stage;

CCannon1::CCannon1(int Type, float _x, float _y, int width, int height) :CEnemy(Type, _x, _y, width, height)
{
	_HP = CANNON_HP;
	_Range_X = CANNON_RANGE_ATTACK1;
	_Range_Y = CANNON_RANGE_ATTACK2;
}
void CCannon1::LoadResource()
{
}
void CCannon1::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING) ProcessDying(CANNON_TIME_DIE, CANNON_DIE_ANIMATION_COUNT, CANNON_ATTACK_RATE);
}
void CCannon1::ProcessAlive(float delta)
{
	if (this->_X > 2000)
	{
		ProcessAttack(CANNON_MAX_ATTACK_COUNT2, CANNON_TIME_WAIT, CANNON_ATTACK_RATE, false);
	}
	else
		ProcessAttack(CANNON_MAX_ATTACK_COUNT, CANNON_TIME_WAIT2, CANNON_ATTACK_RATE, false);
}
void CCannon1::ProcessAttack(int Max_Attack_Count, int Time_Wait, int Attack_Rate, bool isAnimation)
{
	if (stage == STAGE2)
	{
		_Range_X = 256;
		_Range_Y = 90;
	}
	else
	{
		_Range_X = 100;
		_Range_Y = 200;
	}
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
				if (isAnimation)
				{
					GetNextAnimation(); // Nếu enemy có animation khi attack
					PlaySound(Enemy_Attack);
				}
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
void CCannon1::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_CANNON1) _AnimateIndex = 0;
}
void CCannon1::Destroyed()
{

}

void CCannon1::Attack()
{
	CheckTarget();
	Pool_Enemy_Bullet->Load(this);
	PlaySound(Enemy_Attack);
}

D3DXVECTOR3 CCannon1::CheckTarget()
{
	D3DXVECTOR3 p;
	float angle = Find_Angle();
	float radian_y = sin(angle);

	if (((_X - Bill->_X)<_Range_X && (_X - Bill->_X) > -_Range_X) &&
		((_Y - Bill->_Y)<_Range_Y && (_Y - Bill->_Y) > -_Range_Y))
	{
		if (_Direction.x > 0)
		{
			if (radian_y > sin(9 * pi / 12) && radian_y < sin(7 * pi / 12))
			{
				p.y = sin(8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = -cos(8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_RIGHT_RIGHT_UP;
			}
			else
			if (radian_y < sin(-9 * pi / 12) && radian_y > sin(-7 * pi / 12))
			{
				p.y = sin(-8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = -cos(-8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_RIGHT_RIGHT_DOWN;
			}
			else
			if (radian_y > sin(11 * pi / 12) && radian_y < sin(9 * pi / 12))
			{
				p.y = sin(10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = -cos(10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_RIGHT_UP;
			}
			else
			if (radian_y < sin(-11 * pi / 12) && radian_y > sin(-9 * pi / 12))
			{
				p.y = sin(-10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = -cos(-10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_RIGHT_DOWN;
			}
			else
			if (radian_y < sin(11 * pi / 12) && radian_y>sin(-11 * pi / 12))
			{
				_state = CANNON_RIGHT;
				p.x = 1 * CANNON_WHITE_BULLET_SPEED;
				p.y = 0;
			}
			else
			if (/*radian_y > sin(-5 * pi / 12) && */radian_y < sin(-7 * pi / 12))
			{
				_state = CANNON_DOWN;
				p.x = 0;
				p.y = -1 * CANNON_WHITE_BULLET_SPEED;
			}
			else
			if (radian_y > sin(7 * pi / 12) /*&& radian_y > sin(5 * pi / 12)*/)
			{
				_state = CANNON_UP;
				p.x = 0;
				p.y = 1 * CANNON_WHITE_BULLET_SPEED;
			}
		}
		else
		{
			if (radian_y > sin(9 * pi / 12) && radian_y < sin(7 * pi / 12))
			{
				p.y = sin(8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = cos(8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_LEFT_LEFT_UP;
			}
			else
			if (radian_y < sin(-9 * pi / 12) && radian_y > sin(-7 * pi / 12))
			{
				p.y = sin(-8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = cos(-8 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_LEFT_LEFT_DOWN;
			}
			else
			if (radian_y > sin(11 * pi / 12) && radian_y < sin(9 * pi / 12))
			{
				p.y = sin(8 * pi / 9)*CANNON_WHITE_BULLET_SPEED;
				p.x = cos(8 * pi / 9)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_LEFT_UP;
			}
			else
			if (radian_y < sin(-11 * pi / 12) && radian_y > sin(-9 * pi / 12))
			{
				p.y = sin(-10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				p.x = cos(-10 * pi / 12)*CANNON_WHITE_BULLET_SPEED;
				_state = CANNON_LEFT_DOWN;
			}
			else
			if (radian_y < sin(11 * pi / 12) && radian_y>sin(-11 * pi / 12))
			{
				_state = CANNON_LEFT;
				p.y = 0;
				p.x = -1 * CANNON_WHITE_BULLET_SPEED;
			}
		}
		p.z = 0.0f;
		return p;
	}
	else
	{
		_state = CANNON_CLOSE;
	}
}

void CCannon1::TakeDamaged(int damage)
{
	if (_state == CANNON_CLOSE) return;
	_HP -= damage;
	if (_HP <= 0)
	{
		ChangeState(DYING);
	}
}

D3DXVECTOR2 CCannon1::Find_XY()
{
	CheckDirection();
	D3DXVECTOR2 result;
	if (_Direction.x > 0)
	{
		switch (_state)
		{
		case CANNON_RIGHT:
			result.x = _X + CANNON_WIDTH - OFSET;
			result.y = _Y + CANNON_HEIGHT / 2 - OFSET;
			break;
		case CANNON_RIGHT_UP:
			result.x = _X + CANNON_WIDTH - OFSET;
			result.y = _Y + CANNON_HEIGHT / 2 + OFSET;
			break;
		case CANNON_RIGHT_DOWN:
			result.x = _X + CANNON_WIDTH - OFSET;
			result.y = _Y + OFSET;
			break;
		case CANNON_RIGHT_RIGHT_UP:
			result.x = _X + CANNON_WIDTH / 2 + OFSET;
			result.y = _Y + CANNON_HEIGHT - OFSET;
			break;
		case CANNON_RIGHT_RIGHT_DOWN:
			result.x = _X + CANNON_WIDTH / 2;
			result.y = _Y;
			break;
		case CANNON_UP:
			result.x = _X + CANNON_WIDTH / 3;
			result.y = _Y + CANNON_HEIGHT;
			break;
		case CANNON_DOWN:
			result.x = _X + CANNON_WIDTH / 3;
			result.y = _Y;
			break;
		}
	}
	else
	{
		switch (_state)
		{
		case CANNON_LEFT:
			result.x = _X;
			result.y = _Y + CANNON_HEIGHT / 2 - OFSET;
			break;
		case CANNON_LEFT_UP:
			result.x = _X;
			result.y = _Y + CANNON_HEIGHT / 2;
			break;
		case CANNON_LEFT_DOWN:
			result.x = _X;
			result.y = _Y + OFSET;
			break;
		case CANNON_LEFT_LEFT_UP:
			result.x = _X + CANNON_WIDTH / 5 - OFSET;
			result.y = _Y + CANNON_HEIGHT - OFSET;
			break;
		case CANNON_LEFT_LEFT_DOWN:
			result.x = _X + OFSET;
			result.y = _Y;
			break;
		}
	}
	return result;
}
CCannon1::~CCannon1()
{
}