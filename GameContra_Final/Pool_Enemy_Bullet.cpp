#include "Pool_Enemy_Bullet.h"
#include "Small_Bullet.h"
#include "Large_Bullet.h"
#include "MoonBullet.h"
#include "DBullet.h"
#include "Bom.h"

#define LIMIT_SMALL_BULLET	10
#define LIMIT_LARGE_BULLET	10
#define LIMIT_MOON_BULLET	7
#define LIMIT_D_BULLET		2
#define LIMIT_BOM_BULLET	5

CPool_Enemy_Bullet::CPool_Enemy_Bullet()
{
	AddBullet();
}

vector<CEnemy_Bullet*> CPool_Enemy_Bullet::GetListByType(int Type)
{
	switch (Type)
	{
	case OBJ_BOSS_STAGE1_GUN:
	case OBJ_BOSS_STAGE3_SHIP:
	case OBJ_ARTILLERY:
		return _List_Large;
	case OBJ_SOLDIER:
		return _List_Small;
	case OBJ_BOSS_STAGE2_HAND:
	case OBJ_BOSS_STAGE2_MOUTH:
		return _List_Moon;
	case OBJ_DIVER:
		return _List_D;
	case OBJ_BOM:
		return _List_Bom;
	default:
		return _List_Small;
		break;
	}
}

void CPool_Enemy_Bullet::Load(CEnemy* Source)
{
	int i = CheckNum(Source->_Type);
	if (i != -1)
	{
		D3DXVECTOR3 Vector = Source->CheckTarget();
		D3DXVECTOR2 Position = Source->Find_XY();
		GetListByType(Source->_Type).at(i)->Fire(Position.x, Position.y, Vector.x, Vector.y, Vector.z);
	}
}

void CPool_Enemy_Bullet::Load(int Type, float X, float Y, float Vx, float Vy, float Gravity)
{
	int i = CheckNum(Type);
	if (i != -1)
	{
		GetListByType(Type).at(i)->Fire(X, Y, Vx, Vy, Gravity);
	}
}

int CPool_Enemy_Bullet::CheckNum(int Type)
{
	int index = -1;
	for (int i = 0; i < GetListByType(Type).size(); i++)
	{
		if (GetListByType(Type).at(i)->_Live_State == DESTROYED) return i;
	}
	return index;
}

void CPool_Enemy_Bullet::AddBullet()
{
	for (int i = 0; i < LIMIT_SMALL_BULLET; i++)
	{
		CSmall_Bullet* bullet = new CSmall_Bullet();
		_List_Small.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_LARGE_BULLET; i++)
	{
		CLarge_Bullet* bullet = new CLarge_Bullet();
		_List_Large.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_MOON_BULLET; i++)
	{
		CMoon_Bullet* bullet = new CMoon_Bullet();
		_List_Moon.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_D_BULLET; i++)
	{
		CDBullet* bullet = new CDBullet();
		_List_D.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_BOM_BULLET; i++)
	{
		CBom* bom = new CBom();
		_List_Bom.push_back(bom);
	}
}

void CPool_Enemy_Bullet::Update(float delta)
{
	for each (CEnemy_Bullet* bullet in _List_Small)
	{
		bullet->Update(delta);
	}
	for each (CEnemy_Bullet* bullet in _List_Large)
	{
		bullet->Update(delta);
	}
	for each (CEnemy_Bullet* bullet in _List_Moon)
	{
		bullet->Update(delta);
	}
	for each (CEnemy_Bullet* bullet in _List_D)
	{
		bullet->Update(delta);
	}
	for each (CEnemy_Bullet* bullet in _List_Bom)
	{
		bullet->Update(delta);
	}
}

void CPool_Enemy_Bullet::Render()
{
	for each (CEnemy_Bullet* bullet in _List_Small)
	{
		bullet->Render();
	}
	for each (CEnemy_Bullet* bullet in _List_Large)
	{
		bullet->Render();
	}
	for each (CEnemy_Bullet* bullet in _List_Moon)
	{
		bullet->Render();
	}
	for each (CEnemy_Bullet* bullet in _List_D)
	{
		bullet->Render();
	}
	for each (CEnemy_Bullet* bullet in _List_Bom)
	{
		bullet->Render();
	}
}

CPool_Enemy_Bullet::~CPool_Enemy_Bullet()
{

}