#pragma once
#include "Enemy.h"
using namespace std;

#define ARTILLERY_WIDTH			34	
#define ARTILLERY_HEIGHT		32

class CArtillery:public CEnemy
{
public:
	CArtillery(int Type, float _x, float _y, int width, int height);
	void Update(float dt);
	void GetNextAnimation();
	RECT GetRect()
	{
		RECT srect;
		srect = { 197 + _AnimateIndex*ARTILLERY_WIDTH, 123, 231 + _AnimateIndex*ARTILLERY_WIDTH, 155 };
			return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 197 + _Animation_Die_Count * 30, 0, 227 + _Animation_Die_Count * 30, 30 };
		return srect;
	}
	//need code
	void Attack();
	void ProcessAlive(float dt);
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
	~CArtillery();
};

