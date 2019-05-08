#ifndef _MOON_BULLET_H_
#define _MOON_BULLET_H_

#include "EnemyBullet.h"

class CMoon_Bullet : public CEnemy_Bullet
{
protected:
public:
	float _Angle;

	CMoon_Bullet(int Type, float X, float Y, int Width, int Height);
	CMoon_Bullet();

	void Update(float delta);
	void Render();
	void Fire(float X, float Y, float Vx, float Vy);

	~CMoon_Bullet();

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 20, 220, 20 + _AnimateIndex * 20, 240 };
		return srect;
	}
};

#endif