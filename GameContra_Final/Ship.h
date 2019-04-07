#pragma once
#include "Enemy.h"
#include "SmallSpaceShip.h"
#include <vector>


using namespace std;

#define SHIP_WIDTH	62
#define SHIP_HEIGHT	66


enum STATE_SHIP { APEAR, OPENED, CLOSING, DISAPEAR };

class CShip :
	public CEnemy
{
public:
	CShip();
	CShip(int type, int x, int y, int width, int height);
	~CShip();

	vector<CSmallSpaceShip *> list_SmallShip;
	STATE_SHIP state;
	DWORD last_state, last_shoot;
	int sequence = 0;

	void Render();
	void Update(float dt);
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex *SHIP_WIDTH, 100, 62 + _AnimateIndex * SHIP_WIDTH, 166 };
		return srect;
	}

	RECT GetDyingRect()
	{
		RECT rect;
		rect = { 250 + _Animation_Die_Count * 70, 0, 320 + _Animation_Die_Count * 70, 70 };
		return rect;
	}
	void GetNextAnimation();
};

