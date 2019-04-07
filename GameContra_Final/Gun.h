#ifndef _GUN_H_
#define _GUN_H_

#include "Enemy.h"

enum GUN_STATE {PUSH, POP};
class CGun : public CEnemy
{
protected:
public:

	CGun();
	~CGun();

	GUN_STATE _Gun_State; // State of gun
	DWORD _Last_Pop;//con trol animate pop
	DWORD _Last_Push;
	bool _IsGunTwo;

	void Update(float delta);
	void Attack();
	void ProcessAlive(float delta);
	void Render();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
	RECT GetDyingRect();
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 20, 0, 20 + _AnimateIndex * 20, 10 };
		if (_Live_State == DESTROYED)
		{
			if (_IsGunTwo) srect = { 180, 0, 230, 20 };
			else srect = { 140, 0, 170, 20 };
		}
		return srect;
	}
};
#endif