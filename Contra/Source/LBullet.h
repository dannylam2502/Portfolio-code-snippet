#ifndef _L_BULLET_H
#define _L_BULLET_H

#include "PlayerBullet.h"

class CLBullet : public CPlayerBullet
{
public:
	CLBullet(int Type, float X, float Y, int damage);
	CLBullet();
	~CLBullet();
	void Update(float delta);
	void Render();
	void Fire();
	RECT GetRect()
	{
		RECT srect;
		if (_Directon.y == 0)
			srect = { 0, 255, 32, 287 };
		if (_Directon.x == 0)
			srect = { 96, 255, 128, 287 };
		if ((_Directon.x == 1 && _Directon.y == 1) || (_Directon.x == -1 && _Directon.y == -1))
			srect = { 32, 255, 64, 287 };
		if ((_Directon.x == 1 && _Directon.y == -1) || (_Directon.x == -1 && _Directon.y == 1))
			srect = { 64, 255, 96, 287 };
		return srect;
	}

private:

};
#endif