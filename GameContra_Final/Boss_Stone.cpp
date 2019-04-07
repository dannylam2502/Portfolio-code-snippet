#include "Boss_Stone.h"
#include "Texture.h"

#define STONE_SPEED 0.05f
#define STONE_HP	30

#define TIME_WAIT 900

#define TIME_DIE	1000
#define ANIMATION_DIE_NUM	3
#define ANIMATION_DIE_RATE	5

extern CTexture* GameTexture;

CBoss_Stone::CBoss_Stone() : CEnemy(OBJ_BOSS_STAGE2_STONE, 100, 100, STONE_WIDTH, STONE_HEIGHT)
{
	_X_Center = _X + _Width / 2;
	_Y_Center = _Y + _Height / 2;
	_Angle = 0.0f;
	_Angle_Rate = ANGLE_W_RATE;
	_Angle_Max = ANGLE_MAX;
	_Angle_Min = ANGLE_MIN;
	_IsWait = false;
	_Time_Wait = 0;
	_Live_State = DESTROYED;
	_IsSpecial = false;
	_HP = STONE_HP;
}

CBoss_Stone::CBoss_Stone(int Type, float X, float Y, int Width, int Height)
: CEnemy(Type, X, Y, Width, Height)
{

}

void CBoss_Stone::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(delta);
	if (_Live_State == DYING)
		ProcessDying(TIME_DIE, ANIMATION_DIE_NUM, ANIMATION_DIE_RATE);
}


void CBoss_Stone::ProcessAlive(float delta)
{
	_X = _X_Center - STONE_WIDTH / 2;
	_Y = _Y_Center - STONE_HEIGHT / 2;
	if (_IsWait)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 30)
		{
			_Last_Time = now;
			_Time_Wait -= 30;
		}
		if (_Time_Wait < 0)
		{
			_IsWait = false;
		}
	}
	else
	{
		ProcessAngle();
	}
}
void CBoss_Stone::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}

void CBoss_Stone::ProcessAngle()
{
	if (_IsWait)
	{
		_Angle = _Last_Angle;
	}
	else
	{
		CheckIsUp();
		if (_IsUp)
		{
			_Angle += _Angle_Rate;
		}
		else
		{
			_Angle -= _Angle_Rate;

		}
	}
}

void CBoss_Stone::CheckIsUp()
{
	if (_Angle > _Angle_Max)
	{
		_IsUp = false;
	}
	if (_Angle < _Angle_Min)
	{
		_IsUp = true;
	}
	//if (_Angle > _Angle_Max || _Angle < _Angle_Min)
	//{
	//	_Angle_Rate = 0.0f;
	//}
	//else
	//{
	//	_Angle_Rate = _Last_Angle_Rate;
	//}
}

void CBoss_Stone::Wait()
{
	_IsWait = true;
	_Time_Wait = TIME_WAIT;
}

void CBoss_Stone::Wait(int Time)
{
	_IsWait = true;
	_Time_Wait = Time;
}

RECT CBoss_Stone::GetDyingRect()
{
	RECT srect;
	srect = { 90 + _Animation_Die_Count * 30, 60, 120 + _Animation_Die_Count * 30, 90 };
	return srect;
}

CBoss_Stone::~CBoss_Stone()
{

}