#ifndef _SMALL_BULLET_H_
#define _SMALL_BULLET_H_

#include "EnemyBullet.h"

class CSmall_Bullet : public CEnemy_Bullet
{
protected:
public:
	float _Angle;

	CSmall_Bullet(int Type, float X, float Y, int Width, int Height);
	CSmall_Bullet();

	void Update(float delta);
	void Render();
	void Fire(float X, float Y, float Vx, float Vy);

	~CSmall_Bullet();

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 10, 0, 10 + _AnimateIndex * 10, 10 };
		return srect;
	}
};

#endif