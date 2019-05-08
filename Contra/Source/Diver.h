#pragma once
#include "Enemy.h"
using namespace std;

#define DIVER_WIDTH			17	
#define DIVER_HEIGHT		31

class CDiver :public CEnemy
{
public:
	CDiver(int Type, int _x, int _y, int width, int height);
	void Update(float dt);
	void GetNextAnimation();
	RECT GetRect()
	{
		RECT srect;
		srect = { 197 + _AnimateIndex*DIVER_WIDTH, 32, 214 + _AnimateIndex*DIVER_WIDTH, 63 };
		return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 197 + _Animation_Die_Count * 30, 0, 227 + _Animation_Die_Count * 30, 30 };
		return srect;
	}
	void TakeDamaged(int damage);
	//need code
	void Attack();
	void ProcessAlive(float delta);
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
	void CreateBullet();
	~CDiver();
};

