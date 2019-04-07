#include "MouthBoss.h"
#include "Texture.h"
#include "Pool_Enemy_Bullet.h"
#include "PoolBullet.h"
#include "ExploreAnimation.h"
#include "Resources.h"

#define MOUTH_ANIMATE_RATE	7
#define MOUTH_HP			100
#define NUM_ANIMATION		3
#define TIME_OPEN			4000
#define TIME_CLOSE			4000
#define BULLET_SPEED		0.1f
#define X_OFFSET			7

#define TIME_DIE			1000
#define ANIMATION_DIE_NUM	3
#define ANIMATION_DIE_RATE	1

extern CTexture* GameTexture;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
extern CPoolBullet* PoolBullet;
extern CExploreAnimation* Animation;

CMouth_Boss::CMouth_Boss() : CEnemy(OBJ_BOSS_STAGE2_MOUTH, 100, 2080, 50, 50)
{
	_State = MOUTH_CLOSED;
	_HP = MOUTH_HP;
}


void CMouth_Boss::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(delta);
	if (_Live_State == DYING)
		ProcessDying(TIME_DIE, ANIMATION_DIE_NUM, ANIMATION_DIE_RATE);
}

void CMouth_Boss::ProcessAlive(float delta)
{
	ProcessState();
	UpdateCollision();
}

void CMouth_Boss::UpdateCollision()
{
	if (_State != MOUTH_OPENED) return;
	for each (CBaseBullet* bullet in PoolBullet->GetListBullet())
	{
		COLLISION_INFO co;
		co = bullet->CollidedWith(this);
		if (co._IsCollided)
		{
			this->TakeDamaged(bullet->_Damage);
			bullet->Destroyed();
		}
	}
}

void CMouth_Boss::ProcessState()
{
	DWORD now = GetTickCount();
	switch (_State)
	{
	case MOUTH_OPENED:
		if (now - _Last_Time >= 30)//fps
		{
			if (_Time_Open > 0)
			{
				_Time_Open -= (now - _Last_Time);
			}
			else
			{
				ChangeState(MOUTH_CLOSING);
			}
			_Last_Time = now;
		}
		break;
	case MOUTH_OPENING:
		if (now - _Last_Time > 1000 / MOUTH_ANIMATE_RATE)
		{
			if (_AnimateIndex < NUM_ANIMATION)
			{
				_AnimateIndex++;
			}
			else
			{
				ChangeState(MOUTH_OPENED);
			}
			_Last_Time = now;
		}
		break;
	case MOUTH_CLOSED:
		if (now - _Last_Time >= 30)
		{
			if (_Time_Close > 0)
			{
				_Time_Close -= (now - _Last_Time);
			}
			else
			{
				ChangeState(MOUTH_OPENING);
			}
			_Last_Time = now;
		}
		break;
	case MOUTH_CLOSING:
		if (now - _Last_Time > 1000 / MOUTH_ANIMATE_RATE)
		{
			if (_AnimateIndex > 0)
			{
				_AnimateIndex--;
			}
			else
			{
				ChangeState(MOUTH_CLOSED);
			}
			_Last_Time = now;
		}
		break;
	}
}

BOX CMouth_Boss::GetBox()
{
	return BOX(_X + 15, _Y, _Width - 30, _Height);
}

void CMouth_Boss::ChangeState(MOUTH_STATE state)
{
	_State = state;
	switch (_State)
	{
	case MOUTH_OPENED:
		_AnimateIndex = 2;
		_Time_Open = TIME_OPEN;
		Attack();
		break;
	case MOUTH_OPENING:
		_AnimateIndex = 0;
		break;
	case MOUTH_CLOSED:
		_AnimateIndex = 0;
		_Time_Close = TIME_CLOSE;
		break;
	case MOUTH_CLOSING:
		_AnimateIndex = 2;
		break;
	default:
		break;
	}
}

void CMouth_Boss::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
	{
		PlaySound(Destroy_Boss, 10);
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
		for (int i = 0; i < 6; i++)
		{
			Animation->DrawAnimation(110, 2100 - 50 * i);
		}
	}
}

RECT CMouth_Boss::GetRect()
{
	RECT srect;
	srect = { 270 + _AnimateIndex * 50, 200, 320 + _AnimateIndex * 50, 250 };
	return srect;
}

RECT CMouth_Boss::GetDyingRect()
{
	RECT srect;
	srect = { 90 + _Animation_Die_Count * 30, 60, 120 + _Animation_Die_Count * 30, 90 };
	return srect;
}

void CMouth_Boss::Attack()
{
	Pool_Enemy_Bullet->Load(_Type, _X + _Width / 2 - X_OFFSET, _Y, BULLET_SPEED *cos(-M_PI_2 + M_PI / 6), BULLET_SPEED *sin(-M_PI_2 + M_PI / 6), 0.0f);
	Pool_Enemy_Bullet->Load(_Type, _X + _Width / 2 - X_OFFSET, _Y, BULLET_SPEED *cos(-M_PI_2 - M_PI / 6), BULLET_SPEED *sin(-M_PI_2 - M_PI / 6), 0.0f);
	Pool_Enemy_Bullet->Load(_Type, _X + _Width / 2 - X_OFFSET, _Y, BULLET_SPEED *cos(-M_PI_2), BULLET_SPEED *sin(-M_PI_2), 0.0f);
}

CMouth_Boss::~CMouth_Boss()
{

}