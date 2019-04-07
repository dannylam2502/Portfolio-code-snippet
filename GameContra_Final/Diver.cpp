#include "Diver.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"
#include "PoolBullet.h"
#include "DBullet.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
extern bool isDestroy;
#define ANIMATION_DIVER		 2
#define DIVER_ANIMATE_RATE	2

#define DIVER_TIME_WAIT		2000
#define DIVER_TIME_DIE	1000
#define DIVER_DIE_ANIMATION_COUNT 3
#define DIE_ANIMATE_RATE		5
#define DIVER_MAX_ATTACK_COUNT	2
#define DIVER_ATTACK_RATE		1

#define BULLET_SPEED		0.2f

float gioihan;
CDiver::CDiver(int Type, int _x, int _y, int width, int height) :CEnemy(Type, _x, _y, width, height)
{
	gioihan = _x;
	_AnimateIndex = 1;
	width = DIVER_WIDTH;
	height = DIVER_HEIGHT;
}

void CDiver::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING) ProcessDying(DIVER_TIME_DIE, DIVER_DIE_ANIMATION_COUNT, DIE_ANIMATE_RATE);
}
void CDiver::ProcessAlive(float delta)
{
	if (Bill->_Y > _Y + 70)
	{
		ProcessAttack(DIVER_MAX_ATTACK_COUNT, DIVER_TIME_WAIT, DIVER_ATTACK_RATE, true);
	}
}
void CDiver::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_DIVER) _AnimateIndex = 0;
}
void CDiver::Attack()
{
	CheckDirection();

	//if (_AnimateIndex == 0)
	{
		Pool_Enemy_Bullet->Load(this);
	}

	/*if (Pool_Enemy_Bullet->CheckNum(OBJ_DIVER) != 0)
	{
	Pool_Enemy_Bullet->AddBullet();
	}*/
}

void CDiver::TakeDamaged(int damage)
{
	if (_AnimateIndex == 1) return;
	_HP -= damage;
	if (_HP <= 0)
	{
		ChangeState(DYING);
	}
}

D3DXVECTOR3 CDiver::CheckTarget()
{
	CheckDirection();
	D3DXVECTOR3 result;
	result.x = 0.0f;
	result.y = BULLET_SPEED;
	result.z = 0;
	/*if (isDestroy)
	{
	if (_X > 100)
	{
	result.x = 0.01f;
	result.y = -0.05f;
	result.z = 0;
	}
	if (_X < 100)
	{
	result.x = -0.01f;
	result.y = -0.05f;
	result.z = 0;
	}
	if (_X == 100)
	{
	result.x = 0;
	result.y = -0.05f;
	result.z = 0;
	}
	}
	else
	{
	result.x = 0;
	result.y = 0.1f;
	result.z = 0;
	}*/
	return result;
}
D3DXVECTOR2 CDiver::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X + _Width / 2;
	result.y = _Y + _Height / 2;
	return result;
}
CDiver::~CDiver()
{
}
