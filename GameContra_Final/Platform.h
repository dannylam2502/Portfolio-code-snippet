#pragma once

#include "GameObject.h"

class CPlatform : public CGameObject
{
public:
	CPlatform(int Type,int pos_x,int pos_y,int width,int height);
	~CPlatform();
	void Render();
	RECT GetRect()
	{
		RECT srect;
		srect = { 0, 190, 30, 210 };
		return srect;
	}
};

