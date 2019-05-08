#include "GameObject.h"
#include "SweptAABB.h"
#include "Texture.h"

extern LPDIRECT3DDEVICE9 G_Device;
extern LPD3DXSPRITE G_SpriteHandler;
extern CTexture* GameTexture;
//Trọng lực
#define GRAVITY -0.00035f

CGameObject::CGameObject(int Type, float X, float Y, int Width, int Height)
{
	_Type = Type;
	_X = X;
	_Y = Y;
	_X_Last = _X;
	_Y_Last = _Y;
	_Vx = 0;
	_Vy = 0;
	_Gravity = GRAVITY;

	_Width = Width;
	_Height = Height;
	_Facing = FACING_LEFT;
	_AnimateIndex = 0;
	_HP = 1;
	_Attack_Count = 0;
	_Animation_Die_Count = -1;
	_Time_Remain = -1;
	_Render_Color = D3DCOLOR_XRGB(255, 255, 255);

	D3DXCreateSprite(G_Device, &_SpriteHandler);
}


void CGameObject::LoadResource()
{

}


void CGameObject::Update(float dt)
{

}

void CGameObject::Render()
{
}

CGameObject::~CGameObject()
{
	_SpriteHandler->Release();
}

BOX CGameObject::GetBox()
{
	BOX result(_X, _Y, _Width, _Height, _Vx, _Vy);
	return result;
}

COLLISION_INFO CGameObject::CollidedWith(CGameObject *o)
{
	COLLISION_INFO result;
	result.time = 0.0f;
	if (_Live_State == DESTROYED || o->_Live_State == DESTROYED || _Live_State == DYING || o->_Live_State == DYING)
	{
		result._IsCollided = false;
		return result;
	}
	BOX b1 = this->GetBox();
	BOX b2 = o->GetBox();
	result._IsCollided = AABB(b1, b2, result.x, result.y);
	if (result._IsCollided) return result;

	b1 = CalculateVelocity(b1, b2);
	if (AABBCheck(GetSweptBroadphaseBOX(b1), b2))
	{
		result.time = SweptAABB(b1, b2, result.x, result.y);
		if (result.time < 1.0f)
		{
			result._IsCollided = true;
			return result;
		}
		else
		{
			result._IsCollided = false;
			return result;
		}
	}
	else
	{
		result._IsCollided = false;
		return result;
	}

	return result;
}

void CGameObject::UpdatePosition(float Delta)
{
	/*_X_Last = _X;
	_Y_Last = _Y;*/

	_Vy += _Gravity * Delta;
	_X += _Vx * Delta;
	_Y += _Vy * Delta;
}

void CGameObject::UpdateCollision()
{

}

void CGameObject::Destroyed()
{
	_Live_State = DESTROYED;
}

void CGameObject::FixPositionCollid(CGameObject * o, int CollidPos)
{
	BOX b1 = this->GetBox();
	BOX b2 = o->GetBox();
	if (CollidPos == COLLIDED_LEFT)
	{
		//_Vx = 0;
		_X = b2.x + b1.w;
	}
	else if (CollidPos == COLLIDED_RIGHT)
	{
		//_Vx = 0;
		_X = b2.x - b1.w;
	}

	else if (CollidPos == COLLIDED_BOT)
	{
		_Y = b2.y + b2.h;
		_Vy = 0;
	}
	else if (CollidPos == COLLIDED_TOP)
	{
		_Vy = 0;
		_Y = b2.y - b2.h;
	}
}

void CGameObject::ProcessDying(int Time_Die, int Animation_Count, int Animation_Rate)
{
	DWORD now = GetTickCount();
	if (_Animation_Die_Count < Animation_Count)
	{
		if (now - _Last_Check > 1000 / Animation_Rate)
		{
			_Last_Check = now;
			_Animation_Die_Count++;
		}
	}
	else
	{
		ChangeState(DESTROYED);
	}
}

void CGameObject::ChangeState(LIVE_STATE state)
{
	if (_Live_State == state) return;
	_Live_State = state;
	if (_Live_State == DYING)
	{
		_AnimateIndex = 0;
		_Animation_Die_Count = -1;
		_Time_Remain = -1;
	}
}

RECT CGameObject::GetDyingRect()
{
	RECT srect = { 210 + _Animation_Die_Count * 40, 260, 250 + _Animation_Die_Count * 40, 300 };
	return srect;
}

RECT CGameObject::GetRect()
{
	RECT srect = { 0, 0, 0, 0 };
	return srect;
}

void CGameObject::TakeDamaged(int damage)
{
	_HP -= damage;
	if (_HP <= 0)
	{
		ChangeState(DYING);
	}
}

void CGameObject::GetNextAnimation()
{

}
void CGameObject::ProcessAlive(float delta)
{

}