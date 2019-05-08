#pragma once
#include "Enemy.h"
using namespace std;

#define HIDE_RIFLEMAN_WIDTH		24
#define HIDE_RIFLEMAN_HEIGHT		17

enum STATE_HIDE_RIFLEMAN
{
	RIFLEMAN_HIDE,
	RIFLEMAN_HIDING,
	RIFLEMAN_PEAR,
	RIFLEMAN_SHOOT,
};
class CHide_Rifleman :
	public CEnemy
{
public:
	DWORD _Last_Check, last_state;
	CHide_Rifleman(int Type, float pos_x, float pos_y, int width, int height);
	void Update(float dt);
	~CHide_Rifleman();
	STATE_HIDE_RIFLEMAN _state;
	void GetNextAnimation();
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex *HIDE_RIFLEMAN_WIDTH, 341, 24 + _AnimateIndex *HIDE_RIFLEMAN_WIDTH, 366 };
		return srect;
	}
	void ProcessAlive(float delta);
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
};
