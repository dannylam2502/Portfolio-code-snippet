#ifndef _MAIN_BOSS_STAGE3_H_
#define _MAIN_BOSS_STAGE3_H_

#include "Enemy.h"
#include "Left_Hand_Boss_Stage2.h"
#include "Right_Hand_Boss_Stage2.h"
#include "Eye_Boss_Stage2.h"
#include "MouthBoss.h"

enum BOSS_STAGE3_STATE {WAITING, APPEAR};

class CMainBoss_Stage3 : public CEnemy
{
protected:
public:
	CLeft_Hand_Boss* _Left_Hand;
	CRight_Hand_Boss* _Right_Hand;
	CEye_Boss_Stage2* _Left_Eye;
	CEye_Boss_Stage2* _Right_Eye;
	CMouth_Boss*	_Mouth;
	BOSS_STAGE3_STATE _State;

	void ChangeBossState(BOSS_STAGE3_STATE state);
	void Update(float delta);
	void Render();
	RECT GetRect();

	CMainBoss_Stage3();
	~CMainBoss_Stage3();

};

#endif