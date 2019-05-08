#include "PoolBullet.h"
#include "GameObject.h"
#include "Resources.h"
#include "Ship.h"
#include "FlyingCapsule.h"

extern vector<CGameObject *> list_Obj_Handle;
extern CShip * Ship;

CPoolBullet::CPoolBullet()
{
	_Bullet_Type = BASIC;
	SetLimitByType();
	AddBullet();
}

void CPoolBullet::Update(float delta)
{
	CheckHit();
	switch (_Bullet_Type)
	{
	case BASIC:
		for each (CBasicBullet* bullet in listBasic)
		{
			bullet->Update(delta);
		}
		break;
	case R:
		break;
	case M:
		for each (CMBullet* bullet in listM)
		{
			bullet->Update(delta);
		}
		break;
	case L:
		for each (CLBullet* bullet in listL)
		{
			bullet->Update(delta);
		}
		break;
	case F:
		for each (CFBullet* bullet in listF)
		{
			bullet->Update(delta);
		}
		break;
	case S:
		for each (CS_Bullet* bullet in listS)
		{
			bullet->Update(delta);
		}
		break;
	default:
		break;
	}
}

void CPoolBullet::Render()
{
	for each (CPlayerBullet* bullet in GetListBullet())
	{
		bullet->Render();
	}
}

void CPoolBullet::Load()
{
	switch (_Bullet_Type)
	{
	case BASIC:
		if (CheckNum() != -1)
		{
			listBasic.at(CheckNum())->Fire();
			PlaySound(Base_Bullet);
		}
		break;
	case F:
		if (CheckNum() != -1)
		{
			listF.at(CheckNum())->Fire();
			PlaySound(FBullet);
		}
		break;
	case M:
		if (CheckNum() != -1)
		{
			listM.at(CheckNum())->Fire();
			PlaySound(MBullet);
		}
		break;
	case L:
		//if (CheckNum() != -1) listL.at(CheckNum())->Fire();
		listL.at(0)->Fire();
		PlaySound(LBullet);
		break;
	case S:
		int NumOfBulletInListS = NumOfBulletInList();
		if (NumOfBulletInListS >= 5)
			NumOfBulletInListS = 5;
		for (int i = 0; i < NumOfBulletInListS; i++)
		if (CheckNum() != -1)
		{
			listS.at(CheckNum())->Fire(i);
			PlaySound(SBullet);
		}
		break;
	}
}

void CPoolBullet::AddBullet()
{
	for (int i = 0; i < LIMIT_BASE; i++)
	{
		CBasicBullet* bullet = new CBasicBullet(OBJ_PLAYER_BULLET, 50, 50, 1);
		listBasic.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_F; i++)
	{
		CFBullet* bullet = new CFBullet(OBJ_PLAYER_BULLET, 50, 50, 3);
		listF.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_M; i++)
	{
		CMBullet* bullet = new CMBullet(OBJ_PLAYER_BULLET, 50, 50, 2);
		listM.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_L; i++)
	{
		CLBullet* bullet = new CLBullet(OBJ_PLAYER_BULLET, 50, 50, 2);
		listL.push_back(bullet);
	}
	for (int i = 0; i < LIMIT_S; i++)
	{
		CS_Bullet* bullet = new CS_Bullet(OBJ_PLAYER_BULLET, 50, 50, 2);
		listS.push_back(bullet);
	}
}

int CPoolBullet::CheckNum()
{
	int index = -1;
	for (int i = 0; i < _Limit; i++)
	{
		if (GetListBullet().at(i)->_Live_State == DESTROYED) return i;
	}
	return index;
}

int CPoolBullet::NumOfBulletInList()
{
	int kq = 0;
	for each (CS_Bullet* bullet in listS)
	{
		if (bullet->_Live_State == DESTROYED)
			kq++;
	}
	return kq;
}

void CPoolBullet::ChangeType(BULLET_TYPE Type)
{
	DestroyBullet();//Set tất cả viên đạn đang có thành destroyed
	_Bullet_Type = Type;
	SetLimitByType();
}

void CPoolBullet::DestroyBullet()
{
	for each (CPlayerBullet* bullet in GetListBullet())
	{
		bullet->Destroyed();
	}
}

vector<CBaseBullet*> CPoolBullet::GetListBullet()
{
	vector<CBaseBullet*> temp;
	switch (_Bullet_Type)
	{
	case BASIC:
		return listBasic;
		break;
	case R:
		return listBasic;
		break;
	case M:
		return listM;
		break;
	case L:
		return listL;
		break;
	case F:
		return listF;
		break;
	case S:
		return listS;
		break;
	default:
		break;
	}
	return temp;
}

void CPoolBullet::CheckHit()
{
	for each (CBaseBullet* bullet in GetListBullet())
	{
		for each (CGameObject* obj in list_Obj_Handle)
		{
			if ((obj->_Type >= OBJ_SOLDIER && obj->_Type <= OBJ_ROCK) || obj->_Type == OBJ_HIDE_RIFLEMAN
				|| obj->_Type == OBJ_FALCON || obj->_Type == OBJ_FLYING_CAPSULE)
			{
				COLLISION_INFO co = bullet->CollidedWith(obj);
				if (co._IsCollided)
				{
					if (obj->_Type == OBJ_CANNON1 || obj->_Type == OBJ_CANNON2 || obj->_Type == OBJ_ARTILLERY)
						PlaySound(Attack_Cannon);
					obj->TakeDamaged(bullet->_Damage);
					bullet->Destroyed();
					PlaySound(Destroy_Enemy);
				}
			}
			/*if (obj->_Type == OBJ_FLYING_CAPSULE)
			{
			COLLISION_INFO co = bullet->CollidedWith(obj);
			if (co._IsCollided)
			{
			obj->Destroyed();
			bullet->Destroyed();
			}
			}*/

			for each (CSmallSpaceShip *obj in Ship->list_SmallShip)
			{
				COLLISION_INFO co = bullet->CollidedWith(obj);
				if (co._IsCollided)
				{
					obj->TakeDamaged(bullet->_Damage);
					bullet->Destroyed();
				}
			}

			if (Ship->state != DISAPEAR)
			{
				COLLISION_INFO co = bullet->CollidedWith(Ship);
				if (co._IsCollided)
				{
					Ship->TakeDamaged(bullet->_Damage);
					bullet->Destroyed();
					PlaySound(Attack_Cannon);
				}
			}
		}
	}
}

CPoolBullet::~CPoolBullet()
{

}