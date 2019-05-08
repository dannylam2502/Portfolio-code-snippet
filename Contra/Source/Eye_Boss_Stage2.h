#ifndef _EYE_BOSS_H_
#define _EYE_BOSS_H_

#include "Enemy.h"

class CEye_Boss_Stage2 : public CEnemy
{
protected:
public:

	CEye_Boss_Stage2();
	CEye_Boss_Stage2(int Type, float X, float Y, int Width, int Height);

	void Update(float delta);
	void Render();

	~CEye_Boss_Stage2();

	RECT GetRect()
	{
		RECT srect;
		srect = { 100 + _AnimateIndex * 10, 0, 110 + _AnimateIndex * 10, 20 };
		return srect;
	}
};

#endif