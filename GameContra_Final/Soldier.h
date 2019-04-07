#pragma once

#include "Enemy.h"

using namespace std;

#define SOLDIER_WIDTH			16
#define SOLDIER_HEIGHT			32

enum STATE_SOLDIER{ SOLDIER_WALK, SOLDIER_JUMP };
class CSoldier :
	public CEnemy
{
public:
	DWORD _Last_Check;
	CGameObject* landing_obj;
	float last_landing_x;
	float last_landing_y;

	CSoldier();
	CSoldier(int type, float x, float y, int width, int height);
	void ChangeSoldierState(STATE_SOLDIER state);
	void LoadResource();
	void Update(float dt);
	void UpdateCollision();
	void Land(CGameObject*);
	D3DXVECTOR3 CheckTarget();

	STATE_SOLDIER _State;
	RECT GetRect();
	void GetNextAnimation();
	//need code
	BOX GetBox()
	{
		return BOX(_X + 5, _Y, _Width - 10, _Height - 5);
	}
	void ProcessAlive(float delta);
	void Attack();
	~CSoldier();
};
