#pragma once
#include "Enemy.h"
#include "GameObject.h"
#include <vector>
using namespace std;

#define ROCK_WIDTH			24
#define ROCK_HEIGHT		27

class CRock : public CEnemy
{
public:
	int i;
	float xR, yR;
	float range1, range2;
	boolean isRJump;
	float OBJ_y;
	CRock(int Type, int _x, int _y, int width, int height);
	void Update(float dt);
	void GetNextAnimation();
	void UpdateCollision();
	void Land(CGameObject* obj);
	RECT GetRect()
	{
		RECT srect;
		srect = { 229 + _AnimateIndex*ROCK_WIDTH, 167, 24 + 229 + _AnimateIndex*ROCK_WIDTH, 198 };
		return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 197 + _Animation_Die_Count * 30, 0, 227 + _Animation_Die_Count * 30, 30 };
		return srect;
	}
	//need code
	BOX GetBox();
	void ProcessAlive(float dt);
	void Destroyed();
	void Attack();
	void Reset_Rock();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
	~CRock();
};

