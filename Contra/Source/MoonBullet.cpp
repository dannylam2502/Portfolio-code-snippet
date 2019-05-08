#include "MoonBullet.h"
#include "Texture.h"

extern CTexture* GameTexture;

#define MOON_BULLET_VELOCITY		0.03f
#define MOON_BULLET_WIDTH			10
#define MOON_BULLET_HEIGHT			10
#define MOON_BULLET_ANIMATE_RATE	7
#define NUM_ANIMATION				4
CMoon_Bullet::CMoon_Bullet(int Type, float X, float Y, int Width, int Height) :
CEnemy_Bullet(Type, X, Y, Width, Height)
{
	_Width = MOON_BULLET_WIDTH;
	_Height = MOON_BULLET_HEIGHT;
}

CMoon_Bullet::CMoon_Bullet() :
CEnemy_Bullet(OBJ_ENEMY_BULLET, 0, 0, MOON_BULLET_WIDTH, MOON_BULLET_HEIGHT)
{

}

void CMoon_Bullet::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	UpdateCollision();
	UpdatePosition(delta);
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / MOON_BULLET_ANIMATE_RATE)
	{
		_Last_Time = now;
		_AnimateIndex++;
		if (_AnimateIndex >= NUM_ANIMATION)
		{
			_AnimateIndex = 0;
		}
	}
	CheckDestroyed();
}

void CMoon_Bullet::Render()
{
	if (_Live_State == DESTROYED) return;
	GameTexture->Render(GetRect(), this, G_ViewPort);
}

void CMoon_Bullet::Fire(float X, float Y, float Vx, float Vy)
{
	_Live_State = ALIVE;
	_X = X;
	_Y = Y;
	_Vx = Vx;
	_Vy = Vy;
}

