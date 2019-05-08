#include "Gun.h"
#include "Texture.h"
#include "Pool_Enemy_Bullet.h"
#include "Bill.h"

#define GUN_X_POSITION				3215.0f
#define GUN_Y_POSITION				101.0f
#define GUN_WIDTH					10
#define GUN_HEIGHT					10
#define GUN_ANIMATE					2
#define GUN_POP_RATE				1
#define GUN_PUSH_RATE				10
#define GUN_BULLET_SPEED			0.05f
#define GUN_BULLET_GRAVITY			-0.0002f
#define GUN_HP						15
#define GUN_TIME_DIE				1000
#define GUN_DIE_ANIMATION_COUNT		3
#define GUN_DIE_ANIMATE_RATE		7

extern CTexture* GameTexture;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
extern CBill* Bill;

CGun::CGun() : CEnemy(OBJ_BOSS_STAGE1_GUN, GUN_X_POSITION, GUN_Y_POSITION, GUN_WIDTH, GUN_HEIGHT)
{
	_Facing = FACING_RIGHT;
	_Gun_State = POP;
	_Live_State = ALIVE;
	_HP = GUN_HP;
}

void CGun::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(delta);
	if (_Live_State == DYING)
		ProcessDying(GUN_TIME_DIE, GUN_DIE_ANIMATION_COUNT, GUN_DIE_ANIMATE_RATE);
}

void CGun::ProcessAlive(float delta)
{
	DWORD now = GetTickCount();
	if (_Gun_State == POP)
	{
		if (now - _Last_Time > 1000 / GUN_POP_RATE)
		{
			_Last_Time = now;
			_AnimateIndex++;
			if (_AnimateIndex >= GUN_ANIMATE)
			{
				_AnimateIndex = 0;
			}
			_Gun_State = PUSH;
			Attack();
		}
	}
	else
	{
		if (now - _Last_Time > 1000 / GUN_PUSH_RATE)
		{
			_Last_Time = now;
			_AnimateIndex++;
			if (_AnimateIndex >= GUN_ANIMATE)
			{
				_AnimateIndex = 0;
			}
			_Gun_State = POP;
		}
	}
}

void CGun::Attack()
{
	Pool_Enemy_Bullet->Load(this);
}
D3DXVECTOR2 CGun::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X;
	result.y = _Y + _Height / 2;
	return result;
}

D3DXVECTOR3 CGun::CheckTarget()
{
	D3DXVECTOR3 p(0, 0, 0);
	if (_Live_State == DESTROYED || abs(_X - Bill->_X) > G_ViewPort._Width) return p;
	int c = rand() % 3;
	p.x = -GUN_BULLET_SPEED * 0.5f*(c + 1.0f);
	p.y = GUN_BULLET_SPEED * 1.3f;
	p.z = GUN_BULLET_GRAVITY;
	return p;
}

RECT CGun::GetDyingRect()
{
	RECT srect;
	srect = { 90 + _Animation_Die_Count * 30, 60, 120 + _Animation_Die_Count * 30, 90 };
	return srect;
}

void CGun::Render()
{
	if (_Live_State == DESTROYED) GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == ALIVE) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}

CGun::~CGun()
{

}