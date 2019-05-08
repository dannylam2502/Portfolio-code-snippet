#include "LifeNum.h"
#include "Bill.h"
#include "Texture.h"

extern CBill* Bill;
extern CTexture* GameTexture;

CLifeNum::CLifeNum() : CGameObject(OBJ_LIFE_NUM, 0, 0, 0, 0)
{

}

void CLifeNum::Update(float delta)
{
	_X = G_ViewPort._X + 15;
	_Y = G_ViewPort._Y + G_ViewPort._Height - 30;
}

void CLifeNum::Render()
{
	for (int i = 0; i < Bill->_Life_Num - 1 && i < 4; i++)
	{
		GameTexture->Render(GetRect(), this, G_ViewPort, _X + i * 15, _Y);
	}
}

RECT CLifeNum::GetRect()
{
	RECT srect;
	srect = { 40, 0, 50, 20 };
	return srect;
}

CLifeNum::~CLifeNum()
{

}