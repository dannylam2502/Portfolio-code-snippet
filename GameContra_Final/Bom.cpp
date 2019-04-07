#include "Bom.h"
#include "Texture.h"

extern CTexture* GameTexture;

#define BOM_VELOCITY			0.07f
#define BOM_ANIMATION_RATE		10
#define BOM_ANIMATION_COUNT		8
#define TIME_DIE				1000
#define NUM_ANIMATION_DIE		3
#define DIE_ANIMATION_RATE		5

CBom::CBom(int Type, float X, float Y, int Width, int Height) :
CEnemy_Bullet(Type, X, Y, Width, Height)
{
	_Width = BOM_WIDTH;
	_Height = BOM_HEIGHT;
}

CBom::CBom() :
CEnemy_Bullet(OBJ_BOM_BULLET, 0, 0, BOM_WIDTH, BOM_HEIGHT)
{

}

void CBom::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) 
		ProcessDying(TIME_DIE, NUM_ANIMATION_DIE, DIE_ANIMATION_RATE);
	if (_Live_State == ALIVE)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / BOM_ANIMATION_RATE)
		{
			_Last_Time = now;
			GetNextAnimation();
		}

		UpdateCollision();
		UpdatePosition(delta);
		CheckDestroyed();
	}
}

void CBom::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= BOM_ANIMATION_COUNT)
		_AnimateIndex = 0;
}

void CBom::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) GameTexture->Render(GetDyingRect(), this, G_ViewPort);
	if (_Live_State == ALIVE) GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CBom::Fire(float X, float Y, float Vx, float Vy)
{
	ChangeState(ALIVE);
	_X = X;
	_Y = Y;
	_Vx = Vx;
	_Vy = Vy;
}

RECT CBom::GetDyingRect()
{
	//RECT srect;
	//srect = { 0 + _Animation_Die_Count * 14, 241, 14 + _Animation_Die_Count * 14, 255 };
	//return srect;
	RECT srect;
	srect = { 0 + _Animation_Die_Count * 40, 320, 40 + _Animation_Die_Count * 40, 360 };
	return srect;

}