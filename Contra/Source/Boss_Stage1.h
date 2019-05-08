#ifndef _BOSS_STAGE1_H_
#define _BOSS_STAGE1_H_

#include "Enemy.h"
#include "Gun.h"

class CBoss_Stage1 : public CEnemy
{
protected:
public:
	CGun* Gun_1;
	CGun* Gun_2;

	CBoss_Stage1();
	~CBoss_Stage1();

	int _Time_Delay;
	bool isGoBoss;

	void Update(float delta);
	void Render();
	void UpdateCollision();
	void ProcessAlive(float delta);
	RECT GetDyingRect();
	void ChangeState(LIVE_STATE state);

	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex * 30, 10, 30 + _AnimateIndex * 30, 50 };
		return srect;
	}
};
#endif