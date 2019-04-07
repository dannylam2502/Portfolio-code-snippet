#ifndef _LARGE_BULLET_H_
#define _LARGE_BULLET_H_

#include "EnemyBullet.h"

class CLarge_Bullet : public CEnemy_Bullet
{
protected:
public:
	float _Angle;

	CLarge_Bullet(int Type, float X, float Y, int Width, int Height);
	CLarge_Bullet();

	void Update(float delta);
	void Render();
	void Fire(float X, float Y, float Vx, float Vy);

	~CLarge_Bullet();

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 10, 30, 10 + _AnimateIndex * 10, 40 };
		return srect;
	}
	RECT GetDyingRect();
};

#endif