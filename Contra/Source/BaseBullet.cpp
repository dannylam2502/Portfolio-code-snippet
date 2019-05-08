#include "BaseBullet.h"

CBaseBullet::CBaseBullet(int Type, float X, float Y, int damage) :
CGameObject(Type, X, Y, 0, 0)
{
	_Damage = damage;
	_Live_State = DESTROYED;
	
}
CBaseBullet::~CBaseBullet()
{
}
void CBaseBullet::Update(float delta)
{

}
void CBaseBullet::Render()
{

}

void CBaseBullet::Fire()
{

}

void CBaseBullet::Fire(int i)
{

}

void CBaseBullet::Destroyed()
{

}