#include "Soldier.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"
#include "WorldMap.h"

// extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
// Override Checktarget
// Vào Pool_Enemy_Bullet.cpp GetType 
// Đưa vào hàm Attack Pool_Enemy_Bullet->Load(this);
// Update nhớ Attack
#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern vector<CGameObject *> list_Obj_Handle;
extern STAGE stage;

#define ANIMATION_SOLDIER			3
#define SOLDIER_SPEED				0.07f
#define SOLDIER_JUMP_SPEED			0.12f
#define SOLDIER_ANIMATE_RATE		8
#define SOLDIER_ANIMATE_DIE_RATE	5
#define SOLDIER_ATTACK_RATE			1
#define SOLDIER_TIME_DIE			1000
#define SOLDIER_DIE_AINATION_COUNT	3
#define SOLDIER_HP					1

extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

CSoldier::CSoldier() :CEnemy(OBJ_SOLDIER, 100, 120, SOLDIER_WIDTH, SOLDIER_HEIGHT)
{

}

CSoldier::CSoldier(int type, float x, float y, int width, int height) : CEnemy(OBJ_SOLDIER, x, y, width, height)
{
	_HP = SOLDIER_HP;
}

void CSoldier::LoadResource()
{
}
void CSoldier::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING)
		ProcessDying(SOLDIER_TIME_DIE, SOLDIER_DIE_AINATION_COUNT, SOLDIER_ANIMATE_DIE_RATE);
}

void CSoldier::ProcessAlive(float dt)
{
	UpdatePosition(dt);
	UpdateCollision();
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / SOLDIER_ANIMATE_RATE)
	{
		_Last_Time = now;
		GetNextAnimation();
	}
	if (now - _Last_Check > 1000 / SOLDIER_ATTACK_RATE)
	{
		_Last_Check = now;
		Attack();
	}

	if (landing_obj != NULL)
	{
		ChangeSoldierState(SOLDIER_WALK);
	}
	else
	{
		ChangeSoldierState(SOLDIER_JUMP);
	}

	if (_X != last_landing_x)
	{
		landing_obj = NULL;
	}

	if (_Vx > 0) _Facing = FACING_RIGHT;
	else if (_Vx < 0) _Facing = FACING_LEFT;
	else if (_Vx_Last < 0) _Facing = FACING_LEFT;
	else if (_Vx_Last > 0) _Facing = FACING_RIGHT;
}

void CSoldier::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_SOLDIER) _AnimateIndex = 0;
}

void CSoldier::UpdateCollision()
{
	for each (CGameObject* obj in list_Obj_Handle)
	{
		if (obj->_Type == OBJ_PLATFORM)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				if (this->_Y >= obj->_Y && this->_Vy < 0 && obj->_Type != OBJ_PLATFORM_ROCK)
				{
					if (co.time > 0.0f && co.time < 1.0f)
					{
						this->_X += this->_Vx * co.time;
						this->_Y += this->_Vy * co.time;
						this->Land(obj);
					}
					else
					{
						this->FixPositionCollid(obj, COLLIDED_BOT);
						this->Land(obj);
					}
				}
			}
		}
		if (obj->_Type == OBJ_WATER)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				ChangeState(DESTROYED);
			}
		}
	}
}

void CSoldier::Land(CGameObject* obj)
{
	if (obj != landing_obj)
	{
		landing_obj = obj;
		last_landing_x = _X;
		last_landing_y = _Y;
		if (last_landing_x < obj->_X)
			last_landing_x = obj->_X;
		else if (last_landing_x + _Width > obj->_X + obj->_Width)
			last_landing_x = obj->_X + obj->_Width - _Width;
	}
}

void CSoldier::Attack()
{
	CheckDirection();
	_Vx = _Direction.x * SOLDIER_SPEED;
	if (stage == STAGE2)
	{
		Pool_Enemy_Bullet->Load(this);
	}
}

void CSoldier::ChangeSoldierState(STATE_SOLDIER state)
{
	if (_State == state) return;
	_State = state;
	if (_State == SOLDIER_JUMP)
	{
		_Vy = SOLDIER_JUMP_SPEED;
	}
}

D3DXVECTOR3 CSoldier::CheckTarget()
{
	D3DXVECTOR3 result;
	CheckDirection();
	result.x = 0.1f*_Direction.x;
	result.y = 0.0f;
	result.z = 0;
	return result;
}

RECT CSoldier::GetRect()
{
	RECT srect;
	srect = { 227 + _AnimateIndex *SOLDIER_WIDTH, 302, 243 + _AnimateIndex * SOLDIER_WIDTH, 334 };
	return srect;
}


CSoldier::~CSoldier()
{
}
