#include "Fire.h"
#include "Texture.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
#define ANIMATION_FIRE		2
#define FIRE_ANIMATE_RATE		5
#define FIRE_SPEED			0.05f
#define FIRE_RANGE			64
CFire::CFire(int Type, float pos_x, float pos_y, int width, int height) :CGameObject(Type,pos_x,pos_y,width,height)
{
	 _Vx = FIRE_SPEED;
}

void CFire::Render()
{
	GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CFire::Update(float dt)
{
	_X += _Vx * dt;
	_Y += _Vy * dt;

	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / FIRE_ANIMATE_RATE)
	{
		_Last_Time = now;
		GetNextAnimation();
	}
	if (_X >= G_ViewPort._Width - FIRE_RANGE - 15)
	{
		_Vx = -FIRE_SPEED;
	}
	if (_X <= FIRE_RANGE)
	{
		_Vx = FIRE_SPEED;
	}
}
void CFire::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_FIRE) _AnimateIndex = 0;
}

CFire::~CFire()
{
}
