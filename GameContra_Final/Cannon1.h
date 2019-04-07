#pragma once
#include "Enemy.h"
using namespace std;

#define CANNON_WIDTH			32
#define CANNON_HEIGHT		32

enum STATE_CANNON1
{
	CANNON_CLOSE,
	CANNON_OPEN,
	CANNON_RIGHT,
	CANNON_LEFT,
	CANNON_UP,
	CANNON_DOWN,
	CANNON_RIGHT_UP,
	CANNON_LEFT_UP,
	CANNON_RIGHT_RIGHT_UP,
	CANNON_LEFT_LEFT_UP,
	CANNON_RIGHT_DOWN,
	CANNON_LEFT_DOWN,
	CANNON_RIGHT_RIGHT_DOWN,
	CANNON_LEFT_LEFT_DOWN
};
class CCannon1 :
	public CEnemy
{
public:
	DWORD _Last_Check;
	DWORD last_animation;
	bool _is_state;
	int _Range_X;
	int _Range_Y;
	CCannon1(int Type, float _x, float _y, int width, int height);
	void LoadResource();
	void Update(float dt);
	void ProcessAttack(int Max_Attack_Count, int Time_Wait, int Attack_Rate, bool isAnimation);
	~CCannon1();
	void TakeDamaged(int);
	STATE_CANNON1 _state;
	void GetNextAnimation();
	RECT GetRect()
	{
		RECT srect;
		switch (_state)
		{
		case CANNON_CLOSE:
			srect = { 96 + _AnimateIndex *CANNON_WIDTH, 64, 128 + _AnimateIndex *CANNON_WIDTH, 96 };
			break;
		case CANNON_OPEN:
			srect = { 96 + _AnimateIndex *CANNON_WIDTH, 32, 128 + _AnimateIndex *CANNON_WIDTH, 64 };
			break;
		case CANNON_LEFT:
			srect = { 192 + _AnimateIndex *CANNON_WIDTH, 64, 224 + _AnimateIndex *CANNON_WIDTH, 96 };
			break;
		case CANNON_UP:
			srect = { 96 + _AnimateIndex *CANNON_WIDTH, 0, 128 + _AnimateIndex *CANNON_WIDTH, 32 };
			break;
		case CANNON_DOWN:
			srect = { 96 + _AnimateIndex *CANNON_WIDTH, 96, 128 + _AnimateIndex *CANNON_WIDTH, 128 };
			break;
		case CANNON_LEFT_LEFT_UP:
			srect = { 192 + _AnimateIndex *CANNON_WIDTH, 0, 224 + _AnimateIndex *CANNON_WIDTH, 32 };
			break;
		case CANNON_LEFT_UP:
			srect = { 192 + _AnimateIndex *CANNON_WIDTH, 32, 224 + _AnimateIndex *CANNON_WIDTH, 64 };
			break;
		case CANNON_LEFT_LEFT_DOWN:
			srect = { 96 + _AnimateIndex *CANNON_WIDTH, 128, 128 + _AnimateIndex *CANNON_WIDTH, 160 };
			break;
		case CANNON_LEFT_DOWN:
			srect = { 192 + _AnimateIndex *CANNON_WIDTH, 96, 224 + _AnimateIndex *CANNON_WIDTH, 128 };
			break;
			//
		case CANNON_RIGHT:
			srect = { 0 + _AnimateIndex *CANNON_WIDTH, 64, 32 + _AnimateIndex *CANNON_WIDTH, 96 };
			break;
		case CANNON_RIGHT_UP:
			srect = { 0 + _AnimateIndex *CANNON_WIDTH, 32, 32 + _AnimateIndex *CANNON_WIDTH, 64 };
			break;
		case CANNON_RIGHT_RIGHT_UP:
			srect = { 0 + _AnimateIndex *CANNON_WIDTH, 0, 32 + _AnimateIndex *CANNON_WIDTH, 32 };
			break;
		case CANNON_RIGHT_DOWN:
			srect = { 0 + _AnimateIndex *CANNON_WIDTH, 96, 32 + _AnimateIndex *CANNON_WIDTH, 128 };
			break;
		case CANNON_RIGHT_RIGHT_DOWN:
			srect = { 0 + _AnimateIndex *CANNON_WIDTH, 128, 32 + _AnimateIndex *CANNON_WIDTH, 160 };
			break;
		default:
			break;
		}
		return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 208 + _Animation_Die_Count * 30, 216, 238 + _Animation_Die_Count * 30, 246 };
		return srect;
	}
	void Destroyed();
	void ProcessAlive(float delta);
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
};