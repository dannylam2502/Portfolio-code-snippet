#include "MapAnimation.h"
#include "Texture.h"

#define MAP_ANIMATE_RATE		8

CMapAnimation::CMapAnimation(int type, float x, float y,int width,int height,int _map_sequence,int _animation,int _stage) 
:CGameObject(OBJ_MAP_ANIMATION,x,y,width,height)
{
	map_sequence = _map_sequence;
	animation = _animation;
	stage = _stage;
}


CMapAnimation::~CMapAnimation()
{
}

void CMapAnimation::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= animation) _AnimateIndex = 0;
}

void CMapAnimation::Update(float dt)
{
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / MAP_ANIMATE_RATE)
	{
		_Last_Time = now;
		GetNextAnimation();
	}
}

void CMapAnimation::Render(LPDIRECT3DTEXTURE9 _Animation,LPD3DXSPRITE spritehandler)
{
	CTexture::Render(GetRect(), _X, _Y, _Animation, spritehandler);
}