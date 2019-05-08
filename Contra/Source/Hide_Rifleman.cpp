#include "Hide_Rifleman.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
#define ANIMATION_RIFLEMAN		2
#define RIFLEMAN_ATTACK_RATE		10
#define RIFLEMAN_RANGE_ATTACK		200
#define RIFLEMAN_BULLET_SPEED	0.08f
#define RIFLEMAN_TIME_WAIT		3000
#define RIFLEMAN_MAX_ATTACK_COUNT	1
#define RIFLEMAN_BULLET_SPEED	0.08f
#define RIFLEMAN_TIME_DIE	1000
#define RIFLEMAN_DIE_ANIMATION_COUNT 3
#define OFSET 2
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

CHide_Rifleman::CHide_Rifleman(int Type, float pos_x, float pos_y, int width, int height) :CEnemy(Type, pos_x, pos_y, width, height)
{
	_HP = 1;
}

void CHide_Rifleman::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING)
		ProcessDying(RIFLEMAN_TIME_DIE, RIFLEMAN_DIE_ANIMATION_COUNT, RIFLEMAN_ATTACK_RATE);

}

void CHide_Rifleman::ProcessAlive(float delta)
{
	if (_state == RIFLEMAN_HIDE)
	{
		_HP = 100000;
	}
	else _HP = 1;
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / 7)
	{
		_Last_Time = now;
		GetNextAnimation();
	}
	if (now - last_state > 2000 && _state == RIFLEMAN_HIDE)
	{
		last_state = now;
		_state = RIFLEMAN_PEAR;
	}
	if (now - last_state > 3000 && _state == RIFLEMAN_SHOOT)
	{
		last_state = now;
		_state = RIFLEMAN_HIDING;
	}
	if (_state == RIFLEMAN_SHOOT)
	{
		ProcessAttack(RIFLEMAN_MAX_ATTACK_COUNT, RIFLEMAN_TIME_WAIT, RIFLEMAN_ATTACK_RATE, false);
	}
}
void CHide_Rifleman::GetNextAnimation()
{
	if (_state == RIFLEMAN_PEAR)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= 2)
		{
			last_state = GetTickCount();
			_state = RIFLEMAN_SHOOT;
		}
	}
	if (_state == RIFLEMAN_SHOOT)
	{
		_AnimateIndex = 2;
	}
	if (_state == RIFLEMAN_HIDING)
	{
		_AnimateIndex--;
		if (_AnimateIndex <= 0)
		{
			_AnimateIndex = 2;
			_state = RIFLEMAN_HIDE;
		}
	}
	if (_state == RIFLEMAN_HIDE)
	{
		_AnimateIndex = 0;
	}
}

void CHide_Rifleman::Attack()
{
	CheckDirection();
	//if (_state == RIFLEMAN_SHOOT)
	Pool_Enemy_Bullet->Load(this);
}

D3DXVECTOR3 CHide_Rifleman::CheckTarget()
{
	D3DXVECTOR3 result;
	if (_X - Bill->_X > 0)
	{
		_Facing = FACING_LEFT;
		result.x = -1 * RIFLEMAN_BULLET_SPEED;
		result.y = 0;
		result.z = 0.0f;
	}
	else
	{
		_Facing = FACING_RIGHT;
		result.x = 1 * RIFLEMAN_BULLET_SPEED;
		result.y = 0;
		result.z = 0.0f;
	}
	return result;
}
D3DXVECTOR2 CHide_Rifleman::Find_XY()
{
	CheckDirection();
	D3DXVECTOR2 result;
	if (_X - Bill->_X > 0)
	{
		result.x = _X;
		result.y = _Y + HIDE_RIFLEMAN_HEIGHT - 2;
	}
	else
	{
		result.x = _X + HIDE_RIFLEMAN_WIDTH;
		result.y = _Y + HIDE_RIFLEMAN_HEIGHT - 2;
	}
	return result;
}
CHide_Rifleman::~CHide_Rifleman()
{
}

