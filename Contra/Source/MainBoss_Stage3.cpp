#include "MainBoss_Stage3.h"
#include "Texture.h"
#include "Bill.h"

extern CBill* Bill;
extern CTexture* GameTexture;

CMainBoss_Stage3::CMainBoss_Stage3() : CEnemy(OBJ_BOSS_STAGE2_SKIN, 16, 2030, 240, 150)
{
	_Right_Hand = new CRight_Hand_Boss();
	_Left_Hand = new CLeft_Hand_Boss();
	_Left_Eye = new CEye_Boss_Stage2(OBJ_BOSS_STAGE2_EYE, 104, 2126, 10, 10);
	_Right_Eye = new CEye_Boss_Stage2(OBJ_BOSS_STAGE2_EYE, 142, 2126, 10, 10);
	_Right_Eye->_Facing = FACING_LEFT;
	_Mouth = new CMouth_Boss();
	_State = WAITING;
}

void CMainBoss_Stage3::Update(float delta)
{
	if (_State == WAITING)
	{
		if (G_ViewPort._Y + G_ViewPort._Width > this->_Y + this->_Height)
		{
			ChangeBossState(APPEAR);
			_Right_Hand->ChangeState(RIGHT_APPEAR);
			_Left_Hand->ChangeState(LEFT_APPEAR);
		}
	}
	if (_State == APPEAR)
	{
		_Right_Eye->Update(delta);
		_Right_Hand->Update(delta);
		_Left_Hand->Update(delta);
		_Left_Eye->Update(delta);
		_Mouth->Update(delta);
		if (_Mouth->_Live_State == DYING)
		{
			for each (CBoss_Stone* stone in _Right_Hand->_List_Stone)
			{
				stone->ChangeState(DYING);
			}
			for each (CBoss_Stone* stone in _Left_Hand->_List_Stone)
			{
				stone->ChangeState(DYING);
			}
		}
	}
}

void CMainBoss_Stage3::Render()
{
	if (_State == WAITING) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);
	_Mouth->Render();
	_Right_Eye->Render();
	_Right_Hand->Render();
	_Left_Hand->Render();
	_Left_Eye->Render();
}

void CMainBoss_Stage3::ChangeBossState(BOSS_STAGE3_STATE state)
{
	if (_State == state) return;
	_State = state;
}

RECT CMainBoss_Stage3::GetRect()
{
	RECT srect;
	srect = { 0, 188, 240, 133 + 188 };
	return srect;
}

CMainBoss_Stage3::~CMainBoss_Stage3()
{

}
