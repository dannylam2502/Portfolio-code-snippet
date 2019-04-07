#include "Rifleman.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
#define ANIMATION_RIFLEMAN		2
#define RIFLEMAN_ATTACK_RATE		3
#define RIFLEMAN_RANGE_ATTACK		200
#define RIFLEMAN_RANGE_CHANGE		20
#define RIFLEMAN_BULLET_SPEED	0.08f
#define RIFLEMAN_TIME_WAIT		1800
#define RIFLEMAN_MAX_ATTACK_COUNT	3
#define pi			3.14
#define RIFLEMAN_TIME_DIE	1000
#define RIFLEMAN_DIE_ANIMATION_COUNT 3
#define OFSET 5
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

CRifleman::CRifleman(int Type, float pos_x, float pos_y, int width, int height) :CEnemy(Type, pos_x, pos_y, width, height)
{

}

void CRifleman::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING) ProcessDying(RIFLEMAN_TIME_DIE, RIFLEMAN_DIE_ANIMATION_COUNT, RIFLEMAN_ATTACK_RATE);
}

void CRifleman::ProcessAlive(float delta)
{
	ProcessAttack(RIFLEMAN_MAX_ATTACK_COUNT, RIFLEMAN_TIME_WAIT, RIFLEMAN_ATTACK_RATE, true);
}
void CRifleman::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_RIFLEMAN) _AnimateIndex = 0;
}

void CRifleman::Attack()
{
	CheckDirection();
	Pool_Enemy_Bullet->Load(this);
}

D3DXVECTOR3 CRifleman::CheckTarget()
{
	float a = _X - Bill->_X;
	float b = _Y - Bill->_Y;
	if (a > 0 && b > 10)
	{
		_state = RIFLEMAN_DOWN;
		_Facing = FACING_LEFT;
	}
	else if (a > 0 && b < -10)
	{
		_state = RIFLEMAN_UP;
		_Facing = FACING_LEFT;
	}
	else if (a>0 && b >= -10 && b <= 10)
	{
		_state = RIFLEMAN_LEFT;
		_Facing = FACING_LEFT;
	}
	else if (a < 0 && b > 10)
	{
		_state = RIFLEMAN_DOWN;
		_Facing = FACING_RIGHT;
	}
	else if (a < 0 && b < -10)
	{
		_state = RIFLEMAN_UP;
		_Facing = FACING_RIGHT;
	}
	else if (a<0 && b >= -10 && b <= 10)
	{
		_state = RIFLEMAN_LEFT;
		_Facing = FACING_RIGHT;
	}
	float angle = Find_Angle();
	D3DXVECTOR3 result;
	result.x = RIFLEMAN_BULLET_SPEED * cos(angle);
	result.y = RIFLEMAN_BULLET_SPEED * sin(angle);
	result.z = 0.0f;
	return result;
}
D3DXVECTOR2 CRifleman::Find_XY()
{
	CheckDirection();
	D3DXVECTOR2 result;
	if (_Direction.x > 0)
	{
		switch (_state)
		{
		case RIFLEMAN_LEFT:
			result.x = _X + RIFLEMAN_WIDTH - OFSET;
			result.y = _Y + RIFLEMAN_HEIGHT / 2 + OFSET;
			break;
		case RIFLEMAN_UP:
			result.x = _X + RIFLEMAN_WIDTH / 2 + OFSET;
			result.y = _Y + RIFLEMAN_HEIGHT + OFSET;
			break;
		case RIFLEMAN_DOWN:
			result.x = _X + RIFLEMAN_WIDTH - OFSET;
			result.y = _Y + OFSET;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (_state)
		{
		case RIFLEMAN_LEFT:
			result.x = _X - OFSET;
			result.y = _Y + RIFLEMAN_HEIGHT / 2 + OFSET;
			break;
			break;
		case RIFLEMAN_UP:
			result.x = _X - OFSET;
			result.y = _Y + RIFLEMAN_HEIGHT;

			break;
		case RIFLEMAN_DOWN:
			result.x = _X - OFSET;
			result.y = _Y + OFSET;
			break;
		default:
			break;
		}
	}
	return result;
}
CRifleman::~CRifleman()
{
}

