#include "EnemyBullet.h"
#include "Bill.h"
#include <vector>

extern CBill* Bill;
extern vector<CGameObject *> list_Obj_Handle;

CEnemy_Bullet::CEnemy_Bullet(int Type, float X, float Y, int Width, int Height)
:CEnemy(Type, X, Y, Width, Height)
{
	_Live_State = DESTROYED;
}

void CEnemy_Bullet::CheckDestroyed()
{
	if (_X > G_ViewPort._X + G_ViewPort._Width || _X < G_ViewPort._X || _Y < G_ViewPort._Y || _Y > G_ViewPort._Y + G_ViewPort._Height)
	{
		_Live_State = DESTROYED;
		_X = 0;
		_Y = 0;
	}
}

void CEnemy_Bullet::Update(float dt)
{

}

void CEnemy_Bullet::Render()
{

}

void CEnemy_Bullet::Fire(float X, float Y, float Vx, float Vy)
{

}
//X , Y: Toa do vien dan luc dau, VX , Vy van toc, Gravity: trong luc
void CEnemy_Bullet::Fire(float X, float Y, float Vx, float Vy, float Gravity)
{
	_Gravity = Gravity;
	Fire(X, Y, Vx, Vy);
}

void CEnemy_Bullet::UpdateCollision()
{
	if (Bill->_Is_Invul) return;
	if (Bill->_Is_Swim && Bill->_State_Head == LIE) return;
	COLLISION_INFO co = this->CollidedWith(Bill);
	if (co._IsCollided)
	{
		//Bill->Die(this->_Vx);
	}
	if (_Type == OBJ_LARGE_BULLET || _Type == OBJ_BOM_BULLET)
	//if (_Type == OBJ_BOM_BULLET)
	{
		for each (CGameObject* obj in list_Obj_Handle)
		{
			if (obj->_Type == OBJ_PLATFORM)
			{
				if (_Y > obj->_Y + obj->_Height / 3)
				{
					COLLISION_INFO co = this->CollidedWith(obj);
					if (co._IsCollided)
					{
						TakeDamaged(2);
						_X -= 12;
					}
				}
			}
			
		}
	}
}

CEnemy_Bullet::~CEnemy_Bullet(){}