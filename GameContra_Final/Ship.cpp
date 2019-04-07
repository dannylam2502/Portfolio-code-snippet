#include "Ship.h"
#include "Texture.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"
#include "Resources.h"

#define ANIMATION_SHIP				12
#define SHIP_ANIMATE_RATE			10
#define SHIP_BULLET_SPEED			0.1f
#define TIME_OPENED					4800
#define TIME_DISAPEAR				5000
#define TIME_SHOOT					800
#define LEFT_OFFSET_X				-11
#define RIGHT_OFFSET_X				36
#define OFFSET_Y					40

#define SHIP_DIE_ANIMATION_RATE		8
#define SHIP_DIE_TIME				3000
#define SHIP_DIE_ANIMATION_COUNT	7

extern CTexture * GameTexture;
extern CBill * Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

CShip::CShip()
{
}

CShip::CShip(int type, int x, int y, int width, int height) : CEnemy(type, x, y, width, height)
{
	_HP = 100;
	_Y = 170;
	_Live_State = ALIVE;
	state = DISAPEAR;
	for (int i = 0; i < 6; i++)
	{
		CSmallSpaceShip *obj = new CSmallSpaceShip(OBJ_BOSS_STAGE3_SMALLSHIP, 0, 0, 28, 20, i % 2);
		obj->_Live_State = ALIVE;
		obj->_Animation_Die_Count = 0;
		list_SmallShip.push_back(obj);
	}
}

CShip::~CShip()
{
}

void CShip::GetNextAnimation()
{
	if (state == APEAR)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_SHIP)
		{
			state = OPENED;
			last_state = GetTickCount();
		}
	}

	if (state == OPENED)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_SHIP)
			_AnimateIndex = 10;
	}

	if (state == CLOSING)
	{
		_AnimateIndex--;
		if (_AnimateIndex <= 2)
		{
			_AnimateIndex = 0;
			state = DISAPEAR;
		}
	}
}

void CShip::Update(float dt)
{
	if (_Live_State == ALIVE)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / SHIP_ANIMATE_RATE)
		{
			_Last_Time = now;
			GetNextAnimation();
		}

		if (now - last_state > TIME_OPENED && state == OPENED)
		{
			last_state = now;
			state = CLOSING;
			_AnimateIndex = 11;
		}

		if (now - last_state > TIME_DISAPEAR && state == DISAPEAR)
		{
			last_state = now;
			state = APEAR;
			/*for each (CSmallSpaceShip *obj in list_SmallShip)
			{
			obj->_Live_State = ALIVE;
			}*/
			this->_X = Bill->_X + 35.0f;
			this->_Y -= 30;
			if (_Y < 110)
				_Y = 170;
		}

		if (now - last_shoot > TIME_SHOOT && state == OPENED)
		{
			last_shoot = now;
			Attack();
			for each(CSmallSpaceShip * obj in list_SmallShip)
			{
				if (obj->direction == sequence % 2 && obj->_Live_State == DESTROYED)
				{
					obj->_Live_State = ALIVE;
					obj->_HP = 1;
					obj->_Animation_Die_Count = 0;
					if (sequence % 2 == 0)
						obj->SetPosition(this->_X + LEFT_OFFSET_X, this->_Y + OFFSET_Y);
					else
						obj->SetPosition(this->_X + RIGHT_OFFSET_X, this->_Y + OFFSET_Y);
					break;
				}
			}
			sequence++;

			/*for (int i = 0; i < 6;i++)
			{
			if (list_SmallShip[i]->state == SMALLSHIP_DESTROYED)
			{
			list_SmallShip[i]->state = SMALLSHIP_ALIVE;
			if (i%2 == 0)
			list_SmallShip[i]->SetPosition(this->_X + LEFT_OFFSET_X, this->_Y + OFFSET_Y);
			else
			list_SmallShip[i]->SetPosition(this->_X + RIGHT_OFFSET_X, this->_Y + OFFSET_Y);
			break;
			}
			}*/
		}
	}

	if (_Live_State == DYING)
	{
		/*if (now - _Last_Time > 1000 / SHIP_DIE_ANIMATION_RATE)
		{
		_Last_Time = now;
		_Animation_Die_Count++;
		}*/
		PlaySound(Destroy_Boss,10);
		for each (CSmallSpaceShip *obj in list_SmallShip)
		{
			obj->_Live_State = DYING;
		}

		ProcessDying(SHIP_DIE_TIME, SHIP_DIE_ANIMATION_COUNT, SHIP_DIE_ANIMATION_RATE);
	}

	for each (CSmallSpaceShip *obj in list_SmallShip)
	{
		obj->Update(dt);
	}
}

void CShip::Attack()
{
	Pool_Enemy_Bullet->Load(this);
}

D3DXVECTOR3 CShip::CheckTarget()
{
	D3DXVECTOR3 result;
	result.x = 0;
	result.y = -1 * SHIP_BULLET_SPEED;
	result.z = 0.0f;
	return result;
}


D3DXVECTOR2 CShip::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X + SHIP_WIDTH / 2 - 5;
	result.y = _Y + SHIP_HEIGHT / 2 + 3;
	return result;
}

void CShip::Render()
{
	if (state != DISAPEAR && _Live_State == ALIVE)
		GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
	{
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
	}

	for each (CSmallSpaceShip *obj in list_SmallShip)
	{
		obj->Render();
	}
}
