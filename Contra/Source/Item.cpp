#include "Item.h"
#include "Texture.h"
#include "PoolBullet.h"
#include "Bill.h"
#include "Resources.h"

#define ITEM_VELOCITY		0.04f
#define ITEM_AMPLITUDE		1
#define ITEM_BOOST			0.2f
#define ITEM_VY_LIMIT		0.15f
#define ITEM_GRAVITY		-0.0003f;
#define ITEM_ANIMATE_RATE	7
#define ITEM_INVUL_TIME		30000

extern CTexture* GameTexture;
extern vector<CGameObject *> list_Obj_Handle;
extern CPoolBullet* PoolBullet;
extern CBill* Bill;

CItem::CItem(int Type, float X, float Y, int Width, int Height) :
CGameObject(Type, X, Y, Width, Height)
{
	_Item_Type = Item_R;
	_Facing = FACING_RIGHT;
	_Live_State = DESTROYED;
	_Gravity = ITEM_GRAVITY;
}
CItem::CItem() : CGameObject(OBJ_ITEM, 100, 70, 15, 5)
{
	_Item_Type = Item_R;
	_Facing = FACING_RIGHT;
	_Live_State = DESTROYED;
	_Gravity = ITEM_GRAVITY;
}
void CItem::Update(float dt)
{
	UpdateCollision();
	if (_Live_State == ALIVE)
	{
		if (_landed == false)
		{
			UpdatePosition(dt);
			if (_Vy < -ITEM_VY_LIMIT)
			{
				_Vy = -ITEM_VY_LIMIT;
			}
			_landed = CheckLanded();
		}
	}
	if (_Item_Type == Item_SSS)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 1000 / ITEM_ANIMATE_RATE)
		{
			_Last_Time = now;
			_AnimateIndex++;
			if (_AnimateIndex >= 3)
			{
				_AnimateIndex = 0;
			}
		}
	}
}
void CItem::Render()
{
	if (_Live_State != DESTROYED)
	{
		GameTexture->Render(GetRect(), this, G_ViewPort);
	}

}

void CItem::Appear(float X, float Y)
{
	_X = X;
	_Y = Y;
	_Live_State = ALIVE;
	_Vx = ITEM_VELOCITY;
	_Vy = ITEM_BOOST;
}
bool CItem::CheckLanded()
{
	for each (CGameObject * obj in list_Obj_Handle)
	{
		if (obj->_Type == OBJ_PLATFORM)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				if (_Y >= obj->_Y + obj->_Height / 2 && _Vy < 0)
				{
					FixPositionCollid(obj, COLLIDED_BOT);
					return true;
				}
			}
		}
	}
	return false;
}

void CItem::UpdateCollision()
{
	if (_Live_State == DESTROYED) return;
	COLLISION_INFO co = Bill->CollidedWith(this);
	if (co._IsCollided)
	{
		PlaySound(Eat_Item);
		switch (_Item_Type)
		{
		case Item_R:
			break;
		case Item_L:
			PoolBullet->ChangeType(L);
			break;
		case Item_S:
			PoolBullet->ChangeType(S);
			break;
		case Item_M:
			PoolBullet->ChangeType(M);
			break;
		case Item_F:
			PoolBullet->ChangeType(F);
			break;
		case Item_B:
			Bill->_Time_Invul = ITEM_INVUL_TIME;
			Bill->_Last_Invul_Time = GetTickCount();
			break;
		case Item_SSS:
			for each (CGameObject* obj in list_Obj_Handle)
			{
				if (obj->_Type >= OBJ_SOLDIER && obj->_Type <= OBJ_DIVER)
				{
					obj->TakeDamaged(100); //die for sure
				}
			}
			break;
		default:
			break;
		}
		_Live_State = DESTROYED;
	}

}
CItem::~CItem(){}