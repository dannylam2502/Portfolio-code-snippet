#include "Boss_Stage1.h"
#include "Texture.h"
#include "PoolBullet.h"
#include "Bill.h"
#include "Resources.h"
#include "ExploreAnimation.h"

#define BOSS_STAGE1_X_POSITION		3220
#define BOSS_STAGE1_Y_POSITION		45
#define BOSS_STAGE1_WIDTH			32
#define BOSS_STAGE1_ANIMATE_RATE	7
#define BOSS_STAGE1_HP				50

#define DISTANCE_BETWEEN_GUN		22
#define TIME_DELAY					800
#define TIME_DIE					1000
#define ANIMATION_DIE_COUNT			3
#define ANIMATION_DIE_RATE			1	

extern CTexture* GameTexture;
extern CPoolBullet* PoolBullet;
extern CBill* Bill;
extern CExploreAnimation* Animation;

CBoss_Stage1::CBoss_Stage1() :
CEnemy(OBJ_BOSS_STAGE1, BOSS_STAGE1_X_POSITION, BOSS_STAGE1_Y_POSITION, BOSS_STAGE1_WIDTH, BOSS_STAGE1_WIDTH)
{
	Gun_1 = new CGun();
	Gun_2 = new CGun();
	Gun_2->_X = Gun_1->_X + DISTANCE_BETWEEN_GUN;
	_Facing = FACING_RIGHT;
	_Time_Delay = TIME_DELAY;
	_HP = BOSS_STAGE1_HP;
	isGoBoss = true;
}
void CBoss_Stage1::Update(float delta)
{
	//if (_Live_State == DESTROYED) return;
	//if (_Live_State == ALIVE) ProcessAlive(delta);

	ProcessAlive(delta);
	if (_Live_State == DYING)
	{
		ProcessDying(TIME_DIE, ANIMATION_DIE_COUNT, ANIMATION_DIE_RATE);
	}
}
void CBoss_Stage1::Render()
{
	//if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE)
	{
		GameTexture->Render(GetRect(), this, G_ViewPort);
	}
	//if (_Live_State == DYING)
	else
	{
		PlaySound(Destroy_Boss, 10);
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
		for (int i = 0; i < 6; i++)
		{
			Animation->DrawAnimation(3200 + 50 * i, 50);
		}
	}
	Gun_1->Render();
	Gun_2->Render();
}

void CBoss_Stage1::ProcessAlive(float delta)
{
	Gun_1->Update(delta);
	if (_Time_Delay < 0)
	{
		Gun_2->Update(delta);
	}

	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / BOSS_STAGE1_ANIMATE_RATE)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= 3)
		{
			_AnimateIndex = 0;
		}
		_Time_Delay -= (now - _Last_Time);
		_Last_Time = now;
	}
	UpdateCollision();
}

void CBoss_Stage1::UpdateCollision()
{
	COLLISION_INFO co;
	for each (CBaseBullet* bullet in PoolBullet->GetListBullet())
	{
		co = bullet->CollidedWith(Gun_1);
		if (co._IsCollided)
		{
			Gun_1->TakeDamaged(bullet->_Damage);
			bullet->Destroyed();
		}
		co = bullet->CollidedWith(Gun_2);
		if (co._IsCollided)
		{
			Gun_2->TakeDamaged(bullet->_Damage);
			bullet->Destroyed();
		}
		co = bullet->CollidedWith(this);
		if (co._IsCollided)
		{
			this->TakeDamaged(bullet->_Damage);
			bullet->Destroyed();
		}
	}
	co = Bill->CollidedWith(this);
	if (co._IsCollided)
	{
		int pos = GetCollisionPosition(co);
		if (pos == COLLIDED_RIGHT)
		{
			/*Bill->FixPositionCollid(this, COLLIDED_RIGHT);
			Bill->Face(this);*/
			Bill->_X += co.x;
			Bill->_Y += co.y;
		}
	}
}

void CBoss_Stage1::ChangeState(LIVE_STATE state)
{
	if (_Live_State == state) return;
	_Live_State = state;
	if (_Live_State == DYING)
	{
		_AnimateIndex = 0;
		_Time_Remain = 0;
		Gun_1->ChangeState(DYING);
		Gun_2->ChangeState(DYING);
	}
}

RECT CBoss_Stage1::GetDyingRect()
{
	RECT srect;
	srect = { 90 + _Animation_Die_Count * 30, 60, 120 + _Animation_Die_Count * 30, 90 };
	return srect;
}

CBoss_Stage1::~CBoss_Stage1()
{

}