#ifndef _LEFT_HAND_BOSS_STAGE2_H_
#define _LEFT_HAND_BOSS_STAGE2_H_

#include "Enemy.h"
#include "Boss_Stone.h"
#include <vector>
using namespace std;

enum STATE_LEFT_HAND { LEFT_SWING, LEFT_APPEAR, LEFT_SPIN, LEFT_HUNG, LEFT_FINISH_MOVE, LEFT_LOCK_TARGET, LEFT_BEFORE_SWING };

class CLeft_Hand_Boss : public CEnemy
{
protected:
public:
	vector<CBoss_Stone*> _List_Stone;
	float _R; //Ban kinh duong tron
	STATE_LEFT_HAND _State_Left;
	void ProcessStone();
	void ProcessState();
	void ProcessAlive(float delta);
	void UpdateCollision();

	void ChangeState(STATE_LEFT_HAND state);
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
	CLeft_Hand_Boss();
	CLeft_Hand_Boss(int Type, float X, float Y, float Width, float Height);
	~CLeft_Hand_Boss();
};

#endif