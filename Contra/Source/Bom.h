#ifndef _BOM_H_
#define _BOM_H_

#include "EnemyBullet.h"

#define BOM_WIDTH	14
#define BOM_HEIGHT	14

class CBom : public CEnemy_Bullet
{
protected:
public:
	float _Angle;

	CBom(int Type, float X, float Y, int Width, int Height);
	CBom();

	void Update(float delta);
	void Render();
	void Fire(float X, float Y, float Vx, float Vy);
	void GetNextAnimation();

	~CBom();

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 14, 241, 14 + _AnimateIndex * 14, 255 };
		return srect;
	}
	RECT GetDyingRect();
};

#endif

