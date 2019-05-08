#include "Enemy.h"
#include "Bill.h"
#include <math.h>
#include "Global.h"
#include "Texture.h"
#include "Resources.h"

#define GUN_BULLET_VX	0.05f
#define GUN_BULLET_VY	0.05f

extern CTexture* GameTexture;
extern CBill* Bill;

CEnemy::CEnemy(int Type, float X, float Y, int Width, int Height)
:CGameObject(Type, X, Y, Width, Height)
{
	_Damage = 1;
	_Attack_Count = 0;
	_Animation_Die_Count = -1;
	_Time_Remain = -1;
	_HP = 1;
}

CEnemy::CEnemy() : CGameObject(OBJ_SOLDIER, 50, 50, 32, 32)
{

}

void CEnemy::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(delta);
}

void CEnemy::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)	
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}

void CEnemy::Destroyed()
{
	_Live_State = DESTROYED;
}

void CEnemy::ProcessDying(int Time_Die, int Animation_Count, int Animation_Rate)
{
	DWORD now = GetTickCount();
	if (_Animation_Die_Count < Animation_Count)
	{
		if (now - _Last_Check > 1000 / Animation_Rate)
		{
			_Last_Check = now;
			_Animation_Die_Count++;
		}
	}
	else
	{
		ChangeState(DESTROYED);
	}
}

void CEnemy::ChangeState(LIVE_STATE state)
{
	if (_Live_State == state) return;
	_Live_State = state;
	if (_Live_State == DYING)
	{
		_AnimateIndex = 0;
		_Animation_Die_Count = -1;
		_Time_Remain = -1;
	}
}

RECT CEnemy::GetDyingRect()
{
	RECT srect = { 210 + _Animation_Die_Count * 40, 260, 250 + _Animation_Die_Count * 40, 300 };
	return srect;
}

RECT CEnemy::GetRect()
{
	RECT srect = { 0, 0, 0, 0 };
	return srect;
}

void CEnemy::Attack()
{

}

void CEnemy::TakeDamaged(int damage)
{
	_HP -= damage;
	if (_HP <= 0)
	{
		ChangeState(DYING);
	}
}

void CEnemy::CheckDirection()
{
	if (_X > Bill->_X) _Direction.x = -1;
	else _Direction.x = 1;
	if (_Y > Bill->_Y) _Direction.y = -1;
	else _Direction.y = 1;
}

D3DXVECTOR3 CEnemy::CheckTarget()
{
	CheckDirection();
	D3DXVECTOR3 p;
	float angle = Find_Angle();
	p.x = _Vx * cos(angle);
	p.y = _Vy * cos(angle);
	p.z = 0.0f;

	return p;
}

void CEnemy::ProcessAttack(int Max_Attack_Count, int Time_Wait, int Attack_Rate, bool isAnimation)
{
	DWORD now = GetTickCount();
	if (_Time_Remain > 0)
	{
		if (now - _Last_Time > 30)//FramRate
		{
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
					GetNextAnimation();
					PlaySound(Enemy_Attack);
				}// Nếu enemy có animation khi attack
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

D3DXVECTOR2 CEnemy::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X + _Width / 2;
	result.y = _Y + _Height / 2;
	return result;
}

float CEnemy::Find_Angle()
{
	float angle = 0.0f;
	CheckDirection();

	float a = _X - Bill->_X;
	float b = _Y - Bill->_Y;

	angle = atan2(b, a);

	/*if (_Direction.x == -1 && _Direction.y == 1) return angle + M_PI;
	if (_Direction.x == -1 && _Direction.y == -1) return angle + M_PI;
	if (_Direction.x == 1 && _Direction.y == -1) return  M_PI + angle;*/

	return float(angle + M_PI);
}

void CEnemy::GetNextAnimation()
{

}
void CEnemy::ProcessAlive(float delta)
{

}
CEnemy::~CEnemy(){};