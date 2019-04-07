#ifndef _RIGHT_HAND_BOSS_STAGE2_H_
#define _RIGHT_HAND_BOSS_STAGE2_H_

#include "Enemy.h"
#include "Boss_Stone.h"
#include <vector>
using namespace std;

enum STATE_RIGHT_HAND {RIGHT_SWING, RIGHT_APPEAR,RIGHT_SPIN, RIGHT_HUNG, RIGHT_FINISH_MOVE, RIGHT_LOCK_TARGET, RIGHT_BEFORE_SWING };

class CRight_Hand_Boss : public CEnemy
{
protected:
public:
	vector<CBoss_Stone*> _List_Stone;
	float _R; //Ban kinh duong tron
	STATE_RIGHT_HAND _State_Right;
	void ProcessStone();
	void ProcessState();
	void ProcessAlive(float delta);
	void UpdateCollision();

	void ChangeState(STATE_RIGHT_HAND state);
	DWORD _Last_State;

	void Swing();
	void Appear();
	void Spin();
	void Hung();
	void Finish();
	void LockTarget();
	void Prepare();
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();

	int _Time_Wait;

	void Update(float delta);
	void Render();
	void Create();
	CRight_Hand_Boss();
	CRight_Hand_Boss(int Type, float X, float Y, float Width, float Height);
	~CRight_Hand_Boss();
};

#endif