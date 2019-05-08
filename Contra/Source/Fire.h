#pragma once

#include "GameObject.h"

using namespace std;

#define FIRE_WIDTH			16
#define FIRE_HEIGHT			16

class CFire :
	public CGameObject
{
public:
	CFire* fire;
	DWORD _Last_Check;
	int x,fire_x;
	CFire(int Type, float pos_x, float pos_y, int width, int height);
	void Render();
	void Update(float dt);

	RECT GetRect()
	{
		RECT srect;
		srect = { 197 + _AnimateIndex *FIRE_WIDTH, 198, 213 + _AnimateIndex * FIRE_WIDTH, 214 };
		return srect;
	}
	void GetNextAnimation();
	~CFire();
};

