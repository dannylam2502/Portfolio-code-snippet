#ifndef _M_BULLET_H
#define _M_BULLET_H

#include "PlayerBullet.h"

class CMBullet : public CPlayerBullet
{
public:
	CMBullet(int Type, float X, float Y, int damage);
	CMBullet();
	~CMBullet();
	void Update(float delta);
	void Render();
	void Fire();
	RECT GetRect()
	{
		RECT srect;
		//srect = { 0 + _AnimateIndex * 10, 20, 10 + _AnimateIndex * 10, 30 };
		srect = { 0, 20, 10, 30 };
		return srect;
	}

private:

};
#endif