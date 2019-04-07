#ifndef _BOSS_STONE_H_
#define _BOSS_STONE_H_

#include "Enemy.h"

#define STONE_WIDTH 15
#define STONE_HEIGHT 15
#define ANGLE_MAX 2.1f
#define ANGLE_MIN -2.1f
#define ANGLE_W_RATE		0.08f//0.06

class CBoss_Stone : public CEnemy
{
protected:
public:

	float _X_Center;
	float _Y_Center;
	float _Angle;
	float _Last_Angle;

	float _Angle_Rate;
	float _Last_Angle_Rate;
	float _Angle_Max;
	float _Angle_Min;

	bool _IsWait;
	bool _IsSpecial;
	int _Time_Wait;

	bool _IsUp;
	void CheckIsUp();

	void ProcessAngle();
	void ProcessAlive(float);

	void Wait();
	void Wait(int Time);

	CBoss_Stone();
	CBoss_Stone(int Type, float X, float Y, int Width, int Height);
	~CBoss_Stone();

	void Update(float delta);
	void Render();

	RECT GetRect()
	{
		RECT srect;
		if (_IsSpecial)
		{
			srect = { 20, 50, 40, 70 };
		}
		else
			srect = { 0, 50, 20, 70 };
		return srect;
	}
	RECT GetDyingRect();
};

#endif