#include "Falcon.h"

#include "Texture.h"
#include "Resources.h"
//#include"GameObject.h"


#include <iostream>
using namespace std;
extern CTexture* GameTexture;


#define ANIMATION_FALCON 7 
#define FALCON_ANIMATE_RATE	 7
#define TIME_DIE			1000
#define DIE_ANIMATION_COUNT	3
#define ANIMATE_DIE_RATE	5

CFalcon::CFalcon(int Type, int _x, int _y, int width, int height, int Item_Type) :CEnemy(Type, _x, _y, width, height)
{
	width = FALCON_WIDTH;
	height = FALCON_HEIGHT;
	_HP = 1;
	switch (Item_Type)
	{
	case 1:
		_Item_Type = Item_B;
		break;
	case 2:
		_Item_Type = Item_F;
		break;
	case 3:
		_Item_Type = Item_L;
		break;
	case 4:
		_Item_Type = Item_M;
		break;
	case 5:
		_Item_Type = Item_R;
		break;
	case 6:
		_Item_Type = Item_S;
		break;
	case 7:
		_Item_Type = Item_SSS;
		break;
	}
	_Item = new CItem();
	_Item->_Item_Type = this->_Item_Type;
}

void CFalcon::Update(float dt)
{
	_Item->Update(dt);
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING)
		ProcessDying(TIME_DIE, DIE_ANIMATION_COUNT, ANIMATE_DIE_RATE);
}

void CFalcon::ProcessAlive(float delta)
{
	DWORD now = GetTickCount();
	if (_Live_State == ALIVE)
	{
		if (now - _Last_Time > 1000 / FALCON_ANIMATE_RATE)
		{
			_Last_Time = now;
			GetNextAnimation();
		}
		if (now - last_state > 2000 && _state == FALCON_OPEN)
		{
			last_state = now;
			_state = FALCON_CLOSING;
			_HP = 1000;
		}
		if (now - last_state > 2000 && _state == FALCON_CLOSE)
		{
			last_state = now;
			_state = FALCON_OPENING;
			_HP = 1;
		}
		//if (_state == FALCON_CLOSING||_state==FALCON_CLOSE)
		//{
		//	
		//}

	}
	/*if (_Live_State == DESTROYED)
	{
		if (now - _Last_Time > 1000 / 4)
		{
			_Last_Time = now;
			_AnimateIndex_Die++;
		}
	}*/
}

void CFalcon::TakeDamaged(int damage)
{
	_HP -= damage;
	if (_HP <= 0)
	{
		_Item->Appear(_X,_Y);
		ChangeState(DYING);
	}
}

void CFalcon::Render()
{
	_Item->Render();
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}
void CFalcon::GetNextAnimation()
{
	if (_state == FALCON_OPENING)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= 4)
		{
			last_state = GetTickCount();
			_state = FALCON_OPEN;
		}
	}
	if (_state == FALCON_OPEN)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= 7)
		{
			_AnimateIndex = 5;
		}
	}
	if (_state == FALCON_CLOSING)
	{
		_AnimateIndex--;
		if (_AnimateIndex <= 2)
		{
			_AnimateIndex = 4;
			_state = FALCON_CLOSE;
		}
	}
	if (_state == FALCON_CLOSE)
	{
		_AnimateIndex = 0;
	}
}

void CFalcon::Destroyed()
{
	_Live_State = DESTROYED;
}
void CFalcon::Appear(float X, float Y)
{
	_X = X;
	_Y = Y;
	_Live_State = ALIVE;
}
CFalcon::~CFalcon()
{
}