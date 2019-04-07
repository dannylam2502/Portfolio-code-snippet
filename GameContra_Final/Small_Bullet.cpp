#include "Small_Bullet.h"
#include "Texture.h"

extern CTexture* GameTexture;

#define SMALL_BULLET_VELOCITY	0.07f
#define SMALL_BULLET_WIDTH		5
#define SMALL_BULLET_HEIGHT		5
CSmall_Bullet::CSmall_Bullet(int Type, float X, float Y, int Width, int Height) :
CEnemy_Bullet(Type, X, Y, Width, Height)
{
	_Width = SMALL_BULLET_WIDTH;
	_Height = SMALL_BULLET_HEIGHT;
}

CSmall_Bullet::CSmall_Bullet() :
CEnemy_Bullet(OBJ_ENEMY_BULLET, 0, 0, SMALL_BULLET_WIDTH, SMALL_BULLET_HEIGHT)
{

}

void CSmall_Bullet::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	UpdateCollision();
	UpdatePosition(delta);
	CheckDestroyed();
}

void CSmall_Bullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CSmall_Bullet::Fire(float X, float Y, float Vx, float Vy)
{
	_Live_State = ALIVE;
	_X = X;
	_Y = Y;
	_Vx = Vx;
	_Vy = Vy;
}

//void CSmall_Bullet::Fire(CEnemy* Source)
//{
//	_X = Source->_X;
//	_Y = Source->_Y;
//	_Live_State = ALIVE;
//	D3DXVECTOR2 p = CheckTarget();
//	CheckDirection();
//	_Vx = SMALL_BULLET_VELOCITY * p.x;
//	_Vy = SMALL_BULLET_VELOCITY * p.y;
//
//}