#include "Large_Bullet.h"
#include "Texture.h"

extern CTexture* GameTexture;

#define LARGE_BULLET_VELOCITY	0.07f
#define LARGE_BULLET_WIDTH		5
#define LARGE_BULLET_HEIGHT		5
#define TIME_DIE				1000
#define NUM_ANIMATION_DIE		3
#define DIE_ANIMATION_RATE		5


CLarge_Bullet::CLarge_Bullet(int Type, float X, float Y, int Width, int Height) :
CEnemy_Bullet(Type, X, Y, Width, Height)
{
	_Width = LARGE_BULLET_WIDTH;
	_Height = LARGE_BULLET_HEIGHT;
}

CLarge_Bullet::CLarge_Bullet() :
CLarge_Bullet(OBJ_LARGE_BULLET, 0, 0, LARGE_BULLET_WIDTH, LARGE_BULLET_HEIGHT)
{

}

void CLarge_Bullet::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) ProcessDying(TIME_DIE, NUM_ANIMATION_DIE, DIE_ANIMATION_RATE);
	if (_Live_State == ALIVE)
	{
		UpdateCollision();
		UpdatePosition(delta);
		CheckDestroyed();
	}
}

void CLarge_Bullet::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) GameTexture->Render(GetDyingRect(), this, G_ViewPort);
	if(_Live_State == ALIVE) GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CLarge_Bullet::Fire(float X, float Y, float Vx, float Vy)
{
	ChangeState(ALIVE);
	_X = X;
	_Y = Y;
	_Vx = Vx;
	_Vy = Vy;
}
RECT CLarge_Bullet::GetDyingRect()
{
	RECT srect;
	srect = { 0 + _Animation_Die_Count * 40, 320, 40 + _Animation_Die_Count *40, 360 };
	return srect;
}