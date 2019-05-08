#include "DBullet.h"
#include "Texture.h"
#include "Pool_Enemy_Bullet.h"
#include "Diver.h"

extern CTexture* GameTexture;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

#define LARGE_BULLET_VELOCITY	0.07f
#define LARGE_BULLET_WIDTH		5
#define LARGE_BULLET_HEIGHT		5
#define DIVER_ATTACK_RANGE		50
#define TIME_DIE				1000
#define NUM_ANIMATION_DIE		3
#define DIE_ANIMATION_RATE		5

bool isDestroy;
CDBullet::CDBullet(int Type, float X, float Y, int Width, int Height) :CEnemy_Bullet(Type, X, Y, Width,Height)
{
	isDestroy = false;
	_Width = LARGE_BULLET_WIDTH;
	_Height = LARGE_BULLET_HEIGHT;
}
CDBullet:: CDBullet() :
CDBullet(OBJ_ENEMY_BULLET, 0, 0, LARGE_BULLET_WIDTH, LARGE_BULLET_HEIGHT)
{

}
void CDBullet::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) ProcessDying(TIME_DIE, NUM_ANIMATION_DIE, DIE_ANIMATION_RATE);
	if (_Live_State == ALIVE)
	{
		UpdateCollision();
		UpdatePosition(delta);
		CheckDestroyed();
		if (_Y - _Start_Pos > 100)
		{
			Pool_Enemy_Bullet->Load(OBJ_ARTILLERY, _X, _Y, -0.05f, 0.07f, -0.0002f);
			Pool_Enemy_Bullet->Load(OBJ_ARTILLERY, _X, _Y, 0.05f, 0.07f, -0.0002f);
			Pool_Enemy_Bullet->Load(OBJ_ARTILLERY, _X, _Y, 0.0f, 0.1f, -0.0002f);
			TakeDamaged(2);//Cho nó nổ
		}
	}
}

void CDBullet::Render()
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == DYING) GameTexture->Render(GetDyingRect(), this, G_ViewPort);
	if (_Live_State == ALIVE) GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CDBullet::Fire(float X, float Y, float Vx, float Vy)
{
	_Live_State = ALIVE;
	_X = X;
	_Y = Y;
	_Vx = Vx;
	_Vy = Vy;
	_Start_Pos = _Y;
}

RECT CDBullet::GetDyingRect()
{
	RECT srect;
	srect = { 0 + _Animation_Die_Count * 30, 290, 30 + _Animation_Die_Count * 30, 320 };
	return srect;
}


CDBullet::~CDBullet()
{

}
