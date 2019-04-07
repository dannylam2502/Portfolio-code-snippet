#pragma once

#include "Platform.h"

#define PLATFORM_WIDTH 32
#define PLATFORM_HEIGHT 27
class CPlatform_Rock : public CPlatform
{
public:
	CPlatform_Rock(int Type, int _x, int _y, int width, int height, int range);
	~CPlatform_Rock();

	int x;
	int range_platformrock;

	void Update(float dt);
	void Render();
	void GetNextAnimation();

	BOX GetBox();
	RECT GetRect()
	{
		RECT srect;
		srect = { 197, 167, 229, 198 };
		return srect;
	}
	void Destroyed();
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
};
