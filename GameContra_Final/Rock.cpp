#include "Rock.h"
#include "Texture.h"
#include "Resources.h"
#include "GameObject.h"
#include "Pool_Enemy_Bullet.h"
#include "Bill.h"
#include <iostream>
using namespace std;

extern vector<CGameObject *> list_Obj_Handle;
extern CTexture* GameTexture;

extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

#define ROCK_ANIMATE_RATE	17
#define	ROCK_APPEAR_ANIMATE_RATE	1
#define ANIMATION_ROCK		2
#define ROCK_RANGE			150
#define ROCK_SPEED_X		0.02f
#define	ROCK_SPEED_Y		0.07f
#define ROCK_TIME_WAIT		100
#define ROCK_TIME_DIE		1000
#define ROCK_RANGE_JUMP		25
#define ROCK_DIE_AINATION_COUNT	3
#define ROCK_HP			20
#define DISTANCE_ROCK_OBJ	5

CRock::CRock(int Type, int _x, int _y, int width, int height) :CEnemy(OBJ_ROCK, _x, _y, width, height)
{
	_HP = ROCK_HP;
	i = 0;
	xR = _x;
	yR = _y;
	isRJump = false;
	_Vx = ROCK_SPEED_X;
	width = ROCK_WIDTH;
	height = ROCK_HEIGHT;
}

void CRock::Update(float dt)
{
	if (_Live_State == DESTROYED)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / ROCK_APPEAR_ANIMATE_RATE)
		{
			_Last_Time = now;
			Reset_Rock();
			_Live_State = ALIVE;
		}
	}
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING)
		ProcessDying(ROCK_TIME_DIE, ROCK_DIE_AINATION_COUNT, ROCK_ANIMATE_RATE);
}
void CRock::ProcessAlive(float dt)
{
	_X += _Vx * dt;
	_Y += _Vy * dt;

	if (i < ROCK_TIME_WAIT)
	{
		if (_X >= xR + 1)
			_Vx = -ROCK_SPEED_X;
		if (_X <= xR - 1)
			_Vx = ROCK_SPEED_X;
	}
	else
	{
		if (!isRJump)
		{
			_X = xR;
			_Vy = -ROCK_SPEED_Y;
		}
	}
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / ROCK_ANIMATE_RATE)
	{
		_Last_Time = now;
		if (_Y < yR)
		{
			_Vx = 0;
			GetNextAnimation();
		}
	}
	if (_Y <= G_ViewPort._Y)
	{
		Reset_Rock();
	}
	if (_Y >= OBJ_y + ROCK_RANGE_JUMP && isRJump)
	{
		_Vy = -ROCK_SPEED_Y;
	}
	UpdateCollision();
	i++;
}
void CRock::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_ROCK) _AnimateIndex = 0;
}
void CRock::UpdateCollision()
{
#pragma region Collide With Object
	for each (CGameObject* obj in list_Obj_Handle)
	{
#pragma region Collide With Platform
		if (obj->_Type == OBJ_PLATFORM)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				if (this->_Y >= obj->_Y && this->_Vy < 0)
				{
					range2 = yR - obj->_Y;
					if (co.time > 0.0f && co.time < 1.0f)
					{
						//this->_X += this->_Vx * co.time;
						//this->_Y += this->_Vy * co.time;
						//this->Land(obj);
					}
					else
					{
						if (range2 >= range1 + DISTANCE_ROCK_OBJ)
						{
							this->FixPositionCollid(obj, COLLIDED_BOT);
							isRJump = false;
							this->Land(obj);
						}
					}
				}
			}
		}
	}
}

BOX CRock::GetBox()
{
	return BOX(_X + 5, _Y + 5, _Width - 10, _Height - 10);
}

void CRock::Land(CGameObject* obj)
{
	if (_Y < obj->_Y + 10 && isRJump == false)
	{
		_Vy = ROCK_SPEED_Y;
		isRJump = true;
	}
	OBJ_y = obj->_Y;
	range1 = yR - obj->_Y;
}
void CRock::Reset_Rock()
{
	range1 = range2 = 0;
	_X = xR;
	_Y = yR;
	_HP = 20;
	isRJump = false;
	OBJ_y = 0;
	_Vx = ROCK_SPEED_X;
	_Vy = 0;
	i = 0;
}
void CRock::Destroyed()
{

}
void CRock::Attack()
{

}
D3DXVECTOR3 CRock::CheckTarget()
{
	D3DXVECTOR3 result;
	return result;
}
D3DXVECTOR2 CRock::Find_XY()
{
	D3DXVECTOR2 result;
	return result;
}
CRock::~CRock()
{
}
