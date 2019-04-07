#ifndef _BASE_ANIMATION_H_
#define _BASE_ANIMATION_H_

#include "GameObject.h"

class CBaseAnimation : public CGameObject
{
protected:
public:

	CBaseAnimation();
	CBaseAnimation(int Type, float X, float Y, int Width, int Height);
	void Update(float delta);
	void Render();
	RECT GetRect();
	~CBaseAnimation();
};

#endif