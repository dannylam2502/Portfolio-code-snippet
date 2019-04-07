#ifndef __BILL_H__
#define __BILL_H__

#include "GameObject.h"
using namespace std;

enum STATE { STAND, MOVE, LIE, REBORN, JUMP, MOVE_TOP, MOVE_BOT, UP, DOWN, NONE, DIE };
enum ACTION { REST, SHOOT };
class CBill : public CGameObject
{
protected:
public:

	CBill();
	~CBill();
	STATE _State_Head;
	STATE _State_Leg;
	//STATE _State;
	ACTION _Action;

	int _Life_Num;

	int _Animation_Rate;
	int _AnimateIndex_Head;
	int _AnimateIndex_Leg;

	bool _Is_Control;
	bool _Is_Swim;
	bool _Is_Invul;
	bool _Is_Flash;

	POINT _direction;
	DWORD _Time_Shooting;
	int _Time_Die;
	int _Time_Invul;
	DWORD _Last_Invul_Time;
	DWORD _Last_Shoot;
	DWORD _Last_Flash;

	CGameObject* landing_obj;

	float last_landing_x;
	float last_landing_y;

	void Damaged(int damage, int x, int y);
	void Render();
	void LoadResource();
	void Update(float dt);
	void UpdateCollision();
	void TriggerControl(bool Up, bool Down, bool Left, bool Right);
	void Jump();
	void Rest();
	void Lie();
	void Die(float Vx);
	void Move(bool Up, bool Down);
	void StandUp();
	void Shoot();
	void Land(CGameObject* obj);
	void ProcessDying();
	void ProcessReborn();
	void ProcessInvul();
	void ProcessFacing();

	BOX GetBox();
	RECT GetRectHead();
	RECT GetRectLeg()
	{
		RECT srect;
		switch (_State_Leg)
		{
		case STAND:
			srect = { 13, 0, 25 + 13, 36 };
			break;
		case MOVE:
			srect = { 14 + _AnimateIndex_Leg * 25, 0, 25 + 14 + _AnimateIndex_Leg * 25, 36 + 0 };
			break;
		default:
			srect = { 0, 0, 0, 0 };
			break;
		}
		return srect;
	}
	void GetNextHeadAnimation(int max)
	{
		_AnimateIndex_Head++;
		if (_AnimateIndex_Head >= max) _AnimateIndex_Head = 0;
	}
	void GetNextHeadAnimation();
	void GetNextLegAnimation(int max)
	{
		_AnimateIndex_Leg++;
		if (_AnimateIndex_Leg >= max) _AnimateIndex_Leg = 0;
	}
	void GetNextAnimation();

	void SetStateHead(STATE state);
	void SetStateLeg(STATE state);

	POINT GetDirection(bool Up, bool Down, bool Left, bool Right);
};

#endif