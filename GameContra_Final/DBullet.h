#pragma once
#include "EnemyBullet.h"
#include "MBullet.h"
class CDBullet :
	public CEnemy_Bullet
{
protected:

public:
	float _Start_Pos;

	CDBullet(int Type, float X, float Y, int Width, int Height);
	CDBullet();
	~CDBullet();

	void Update(float delta);
	void Render();
	void Fire(float X, float Y, float Vx, float Vy);
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 10, 30, 10 + _AnimateIndex * 10, 40 };
		return srect;
	}
	RECT GetDyingRect();
};

