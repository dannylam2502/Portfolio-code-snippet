#pragma once
#include "Enemy.h"

#define SMALLSPACESHIP_WIDTH	28
#define SMALLSPACESHIP_HEIGHT	20

//enum STATE_SMALLSHIP { SMALLSHIP_ALIVE, SMALLSHIP_DESTROYED };

class CSmallSpaceShip :
	public CEnemy
{
public:


	int direction;
	//STATE_SMALLSHIP state;

	CSmallSpaceShip();
	CSmallSpaceShip(int type, float x, float y, int width, int height, int _direction);

	~CSmallSpaceShip();

	void Render();
	void Update(float dt);
	void SetPosition(float x, float y)
	{
		_X = x;
		_Y = y;
	}

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex *SMALLSPACESHIP_WIDTH, 166, 28 + _AnimateIndex * SMALLSPACESHIP_WIDTH, 186 };
		return srect;
	}

	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 150 + _Animation_Die_Count * 30, 60, 180 + _Animation_Die_Count * 30, 90 };
		return srect;
	}

	void GetNextAnimation();
};

