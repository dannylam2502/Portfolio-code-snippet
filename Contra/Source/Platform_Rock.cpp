#include "Platform_Rock.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern CBill* Bill;

#define ANIMATION_PLATFORM_ROCK	1 
#define PLATFORM_ROCK_ANIMATE_RATE	 4
#define PLATFORM_ROCK_SPEED		0.05f

CPlatform_Rock::CPlatform_Rock(int Type, int _x, int _y, int width, int height, int range) :CPlatform(Type, _x, _y, width, height)
{
	width = PLATFORM_WIDTH;
	height = PLATFORM_HEIGHT;
	x = _X;
	range_platformrock = range;
}

void CPlatform_Rock::Update(float dt)
{
	_X += _Vx * dt;
	if (_X > x + range_platformrock)
	{
		_Vx = -PLATFORM_ROCK_SPEED;
	}
	if (_X <= x)
	{
		_Vx = PLATFORM_ROCK_SPEED;
	}
}
void CPlatform_Rock::Render()
{
	GameTexture->Render(GetRect(), this, G_ViewPort);
}
void CPlatform_Rock::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_PLATFORM_ROCK) _AnimateIndex = 0;
}

BOX CPlatform_Rock::GetBox()
{
	return BOX(_X + 5, _Y + 23, _Width - 15, _Height - 26);
}

void CPlatform_Rock::Destroyed()
{

}
void CPlatform_Rock::Attack()
{

}
D3DXVECTOR3 CPlatform_Rock::CheckTarget()
{
	D3DXVECTOR3 result;
	return result;
}
D3DXVECTOR2 CPlatform_Rock::Find_XY()
{
	D3DXVECTOR2 result;
	return result;
}
CPlatform_Rock::~CPlatform_Rock()
{
}