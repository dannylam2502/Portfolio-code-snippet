#pragma once
#include "Enemy.h"
using namespace std;

#define TANK_WIDTH		98	
#define TANK_HEIGHT		65
enum STATE_TANK
{
	TANK_RIGHT,
	TANK_RIGHT_DOWN,
	TANK_DOWN
};
class CTank : public CEnemy
{
public:
	bool IsMove = true;
	DWORD _Last_Check;
	DWORD _Last_Animation;
	CTank();
	CTank(int type, float x, float y, int width, int height);
	void Render();
	void Update(float dt);
	void GetNextAnimation();
	STATE_TANK _state;
	RECT GetRect()
	{
		RECT srect;
		switch (_state)
		{
		case TANK_RIGHT:
			srect = { 0 + _AnimateIndex*TANK_WIDTH, 0, 98 + _AnimateIndex*TANK_WIDTH, 65 };
			return srect;
		case TANK_RIGHT_DOWN:
			srect = { 0 + _AnimateIndex*TANK_WIDTH, 67, 98 + _AnimateIndex*TANK_WIDTH, 132 };
			return srect;
		case TANK_DOWN:
			srect = { 0 + _AnimateIndex*TANK_WIDTH, 135, 98 + _AnimateIndex*TANK_WIDTH, 200 };
			return srect;
		default:
			break;
		}

	}

	RECT GetDyingRect()
	{
		RECT result;
		result = { 0 + _Animation_Die_Count*TANK_WIDTH, 527, 98 + _Animation_Die_Count*TANK_WIDTH, 592 };
		return result;
	}
	//need code
	void Attack();
	void ProcessAlive(float delta);
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
	~CTank();
};
