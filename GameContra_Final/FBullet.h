#ifndef _FBULLET_H_
#define _FBULLET_H_

#include "PlayerBullet.h"
class CFBullet : public CPlayerBullet
{
public:
	float _X_Center;
	float _Y_Center;
	float _Angle_W;

	CFBullet(int Type, float X, float Y, int damage);
	CFBullet();
	~CFBullet();
	void Update(float delta);
	void Render();
	void Fire();
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 10, 10, 10 + _AnimateIndex * 10, 20 };
		return srect;
	}

private:

};
#endif