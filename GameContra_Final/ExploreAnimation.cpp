#include "ExploreAnimation.h"
#include "Texture.h"

#define ANIMATE_RATE	7
#define NUM_ANIMATION	6	

extern CTexture* GameTexture;

CExploreAnimation::CExploreAnimation() :
CGameObject(OBJ_EXPLORE_ANIMATION, 0, 0, 0, 0)
{

}
CExploreAnimation::~CExploreAnimation()
{

}
void CExploreAnimation::Update()
{
	if (_Live_State == DESTROYED) return;
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / ANIMATE_RATE)
	{
		_Last_Time = now;
		if (_AnimateIndex < NUM_ANIMATION)
		{
			_AnimateIndex++;
		}
		else
		{
			//_AnimateIndex = 0;
			ChangeState(DESTROYED);
		}
	}
}

void CExploreAnimation::DrawAnimation(float X, float Y)
{
	ChangeState(ALIVE);
	Update();
	Render(X, Y);
}

void CExploreAnimation::ChangeState(LIVE_STATE state)
{
	if (_Live_State == state) return;
	_Live_State = state;
	_AnimateIndex = 0;
}

void CExploreAnimation::Render(float X, float Y)
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort, X, Y);
}
RECT CExploreAnimation::GetRect()
{
	RECT srect;
	srect = { 0 + _AnimateIndex * 48, 479, 48 + _AnimateIndex * 48, 525 };
	return srect;
}