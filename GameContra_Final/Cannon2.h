#pragma once
#include "Enemy.h"
#include"Bill.h"
using namespace std;

#define CANNON_RED_WIDTH			32
#define CANNON_RED_HEIGHT		32

enum STATE_CANNON2
{
	CANNON2_APPEAR,
	CANNON2_CLOSE,
	CANNON2_CLOSING,
	CANNON2_OPEN,
	CANNON2_LEFT,
	CANNON2_LEFT_UP,
	CANNON2_LEFT_LEFT_UP,
	CANNON2_CLOSED
};
//enum STATE_CANNON_RED{ OPENING, OPENED, CLOSING, CLOSED, APPEAR, DISAPEAR };
class CCannon2 :
	public CEnemy
{
	bool is_InRange;
public:
	CCannon2(int Type, float pos_x, float pos_y, int width, int height);
	DWORD last_state;
	DWORD last_animation;
	void LoadResource();
	void Update(float dt);
	void GetNextAnimation();
	void ProcessAttack(int Max_Attack_Count, int Time_Wait, int Attack_Rate, bool isAnimation);
	STATE_CANNON2 _state;
	//STATE_CANNON_RED state;
	RECT GetRect()
	{
		RECT srect;
		switch (_state)
		{
		case CANNON2_APPEAR:
			srect = { 0 + _AnimateIndex *CANNON_RED_WIDTH, 180, 32 + _AnimateIndex *CANNON_RED_WIDTH, 212 };
			break;
		case CANNON2_CLOSED:
			srect = { 0, 180, 32, 212 };
			break;
		case CANNON2_CLOSING:
			srect = { 64 + _AnimateIndex *CANNON_RED_WIDTH, 180, 96 + _AnimateIndex *CANNON_RED_WIDTH, 212 };
			break;
			/*	case CANNON2_OPEN:
			srect = { 0 + _AnimateIndex *CANNON_RED_WIDTH, 180, 32 + _AnimateIndex *CANNON_RED_WIDTH, 212 };
			break;*/
		case CANNON2_LEFT:
			srect = { 96, 180 + _AnimateIndex   *CANNON_RED_HEIGHT, 128, 212 + _AnimateIndex   *CANNON_RED_HEIGHT };
			break;
		case CANNON2_LEFT_UP:
			srect = { 128, 180 + _AnimateIndex   *CANNON_RED_HEIGHT, 160, 212 + _AnimateIndex   *CANNON_RED_HEIGHT };
			break;
		case CANNON2_LEFT_LEFT_UP:
			srect = { 160, 180 + _AnimateIndex   *CANNON_RED_HEIGHT, 192, 212 + _AnimateIndex   *CANNON_RED_HEIGHT };
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
	void Attack();
	void ProcessAlive(float delta);
	void ChangeCanonState(STATE_CANNON2 state);
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2  Find_XY();
	~CCannon2();
};

