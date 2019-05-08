#ifndef _BASIC_BULLET_H
#define _BASIC_BULLET_H

#include "PlayerBullet.h"

class CBasicBullet : public CPlayerBullet
{
public:
	CBasicBullet(int Type, float X, float Y, int damage);
	CBasicBullet();
	~CBasicBullet();
	void Update(float delta);
	void Render();
	void Fire();
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 10, 0, 10 + _AnimateIndex * 10, 10 };
		return srect;
	}

private:

};
#endif