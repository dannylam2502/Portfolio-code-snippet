#include "PlayerBullet.h"

CPlayerBullet::CPlayerBullet(int Type, float X, float Y, int damage) :
CBaseBullet(Type,X,Y,damage)
{

}
CPlayerBullet::~CPlayerBullet()
{

}
void CPlayerBullet::Update(float delta)
{

}
void CPlayerBullet::Render()
{

}
void CPlayerBullet::Fire()
{
	
}
void CPlayerBullet::Fire(int i)
{

}
void CPlayerBullet::Destroyed()
{
	_Live_State = DESTROYED;
	_X = _Y = 0;
}