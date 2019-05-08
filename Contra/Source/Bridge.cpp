#include "Bridge.h"
#include "Texture.h"
#include "Bill.h"
#include "Resources.h"

extern CTexture* GameTexture;
extern CBill* Bill;

#define ANIMATION_BRIDGE		3
#define ANIMATION_DESTROY	5
#define TIME_DESTROY		2000


CBridge::CBridge() :CGameObject(OBJ_BRIDGE, 0, 0, 32, 16)
{

}

CBridge::CBridge(int type, float x, float y, int width, int height, int _SeqcOnBri) : CGameObject(OBJ_BRIDGE, x, y, width, height)
{
	SequenceOnBrige = _SeqcOnBri;
	temp_x = x;
	temp_y = y;
	_Live_State = ALIVE;
}


CBridge::~CBridge()
{
}

void CBridge::Update(float dt)
{
	DWORD now = GetTickCount();
	_Y = temp_y - 16;
	if (Bill->_X >= _X && _Live_State == ALIVE)
	{
		_AnimateIndex = 0;
		_Live_State = DYING;
		PlaySound(Destroy_Bridge,10);

		//_last_TimeExplovise = GetTickCount();
	}

	if (now - _Last_Time > 1000 / 5)
	{
		_Last_Time = now;
		GetNextAnimation();
	}

	if (_Live_State == DYING)
	{
		_X = temp_x - 8;
	}

	/*if (_Live_State == DESTROYED)
	{
	_X = temp_x;
	}*/
}

void CBridge::Render()
{
	if (_Live_State != DESTROYED)
	{

		if (_Live_State != DYING)
			GameTexture->Render(GetBottomRect(), this, G_ViewPort);
		GameTexture->Render(GetRect(), this, G_ViewPort);
	}
}

void CBridge::GetNextAnimation()
{
	if (_Live_State == ALIVE)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_BRIDGE) _AnimateIndex = 0;
	}

	if (_Live_State == DYING)
	{
		_AnimateIndex++;
		if (_AnimateIndex >= ANIMATION_DESTROY) _Live_State = DESTROYED;;
	}
}
