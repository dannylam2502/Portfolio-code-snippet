#ifndef _MOUTH_BOSS_H_
#define _MOUTH_BOSS_H_

#include "Enemy.h"

enum MOUTH_STATE {MOUTH_OPENED, MOUTH_OPENING, MOUTH_CLOSED, MOUTH_CLOSING};

class CMouth_Boss : public CEnemy
{
protected:
public:
	MOUTH_STATE _State;

	void Update(float delta);
	void Render();
	void Attack();
	void ProcessState();
	void ProcessAlive(float delta);
	void UpdateCollision();

	void ChangeState(MOUTH_STATE state);
	int _Time_Close;
	int _Time_Open;
	BOX GetBox();
	RECT GetRect();
	RECT GetDyingRect();

	CMouth_Boss();
	~CMouth_Boss();
};
#endif