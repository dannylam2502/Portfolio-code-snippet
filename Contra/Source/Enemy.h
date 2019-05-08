#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "GameObject.h"

class CEnemy : public CGameObject
{
protected:
public:
	CEnemy();
	CEnemy(int Type, float X, float Y, int Width, int Height);

	int _Damage;
	POINT _Direction;
	int _Attack_Count;
	int _Time_Remain;
	int _Animation_Die_Count;
	DWORD _Last_Check;

	virtual void GetNextAnimation();
	virtual void Update(float delta);
	virtual void Render();
	virtual void Destroyed();
	virtual void Attack();
	virtual void TakeDamaged(int damage);
	virtual void ChangeState(LIVE_STATE state);
	void CheckDirection();
	virtual D3DXVECTOR3 CheckTarget();
	virtual D3DXVECTOR2 Find_XY();
	virtual float Find_Angle();
	virtual void ProcessAttack(int Max_Attack_Count,int Time_Wait,int Attack_Rate, bool isAnimation);
	virtual void ProcessDying(int Time_Die, int Animation_Count, int Animation_Rate);
	virtual void ProcessAlive(float delta);
	virtual RECT GetRect();
	virtual RECT GetDyingRect();

	~CEnemy();
};
#endif