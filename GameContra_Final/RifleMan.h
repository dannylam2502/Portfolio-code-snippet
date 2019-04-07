#pragma once
#include "Enemy.h"
using namespace std;

#define RIFLEMAN_WIDTH		24
#define RIFLEMAN_HEIGHT		32
#define RIFLEMAN_UP_WIDTH		18
#define RIFLEMAN_UP_HEIGHT		39


enum STATE_RIFLEMAN
{
	RIFLEMAN_LEFT,
	RIFLEMAN_UP,
	RIFLEMAN_DOWN,
};
class CRifleman :
	public CEnemy
{
public:
	DWORD _Last_Check;
	CRifleman(int Type, float pos_x, float pos_y, int width, int height);
	void Update(float dt);
	~CRifleman();
	STATE_RIFLEMAN _state;
	void GetNextAnimation();
	RECT GetRect()
	{
		RECT srect;
		switch (_state)
		{
		case RIFLEMAN_LEFT:
			srect = { 76 + _AnimateIndex *RIFLEMAN_WIDTH, 302, 24 + 76 + _AnimateIndex *RIFLEMAN_WIDTH, 32 + 302 };
			break;
		case RIFLEMAN_UP:
			srect = { 152 + _AnimateIndex *RIFLEMAN_UP_WIDTH, 302, 18 + 152 + _AnimateIndex *RIFLEMAN_UP_WIDTH, 39 + 302 };
			break;
		case RIFLEMAN_DOWN:
			srect = { 0, 302, 24, 31 + 302 };
			break;
		default:
			break;
		}
		return srect;
	}
	void ProcessAlive(float delta);
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
};
