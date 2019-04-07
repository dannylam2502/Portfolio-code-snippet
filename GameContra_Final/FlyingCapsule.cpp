#include "FlyingCapsule.h"
#include "Texture.h"
#include "Bill.h"
#include "WorldMap.h"

#define FLYING_CAPSULE_VELOCITY		0.1f
#define FLYING_CAPSULE_AMPLITUDE	2.0f
#define FLYING_CAPSULE_ANIMATE		7
#define FLYING_CAPSULE_ANGLE_W		0.07f

#define TIME_DIE			1000
#define DIE_ANIMATION_COUNT	3
#define ANIMATE_DIE_RATE	5

extern CTexture* GameTexture;
extern CBill* Bill;
extern STAGE stage;

CFlyingCapsule::CFlyingCapsule(int Type, float X, float Y, int Width, int Height, int Item_type) :
CGameObject(Type, X, Y, Width, Height)
{
	_Gravity = 0;
	_Vx = FLYING_CAPSULE_VELOCITY;
	_Angle = 0.0f;

	switch (Item_type)
	{
	case 1:
		_Item_Type = Item_B;
		break;
	case 2:
		_Item_Type = Item_F;
		break;
	case 3:
		_Item_Type = Item_L;
		break;
	case 4:
		_Item_Type = Item_M;
		break;
	case 5:
		_Item_Type = Item_R;
		break;
	case 6:
		_Item_Type = Item_S;
		break;
	case 7:
		_Item_Type = Item_SSS;
		break;
	}

	_Item = new CItem();
	_Item->_Item_Type = this->_Item_Type;
	_IsWaiting = true;
	//_Live_State = DYING;
}

void CFlyingCapsule::Update(float delta)
{
	_Item->Update(delta);
	if (_IsWaiting)
	{
		if (stage == STAGE2)
		{
			if (Bill->_Y - 100 > this->_Y)
			{
				_IsWaiting = false;
				_Live_State = ALIVE;
			}
		}
		else
		{
			if (G_ViewPort._X > this->_X + this->_Width / 2)
			{
				_IsWaiting = false;
				_Live_State = ALIVE;
			}
		}
	}

	else
	{
		if (_Live_State == DESTROYED) return;
		if (_Live_State == ALIVE) ProcessAlive(delta);
		if (_Live_State == DYING)
			ProcessDying(TIME_DIE, DIE_ANIMATION_COUNT, ANIMATE_DIE_RATE);
	}
	/*if (_Live_State == ALIVE)
	{
	_X += _Vx * delta;
	_Y += FLYING_CAPSULE_AMPLITUDE * sin(_Angle);
	_Angle += FLYING_CAPSULE_ANGLE_W;
	}*/
}

void CFlyingCapsule::ProcessAlive(float delta)
{
	if (stage == STAGE2)
	{
		_Y += FLYING_CAPSULE_VELOCITY * delta;
		_X += FLYING_CAPSULE_AMPLITUDE * sin(_Angle);
		_Angle += FLYING_CAPSULE_ANGLE_W;
	}
	else
	{
		_X += FLYING_CAPSULE_VELOCITY * delta;
		_Y += FLYING_CAPSULE_AMPLITUDE * sin(_Angle);
		_Angle += FLYING_CAPSULE_ANGLE_W;
	}
}

void CFlyingCapsule::Render()
{
	_Item->Render();
	/*if (_Live_State != DESTROYED && _IsWaiting == false)
	{
	GameTexture->Render(GetRect(), this, G_ViewPort);
	}*/
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE && _IsWaiting == false) 	GameTexture->Render(GetRect(), this, G_ViewPort);
	if (_Live_State == DYING)
		GameTexture->Render(GetDyingRect(), this, G_ViewPort);
}

void CFlyingCapsule::Appear(float X, float Y)
{
	/*if (stage == STAGE2)
	{
	_Vy = FLYING_CAPSULE_VELOCITY;
	}
	else
	{
	_Vx = FLYING_CAPSULE_VELOCITY;
	}
	_X = X;
	_Y = Y;
	_Live_State = ALIVE;*/
}

void CFlyingCapsule::TakeDamaged(int damage)
{
	if (_IsWaiting) return;
	_HP -= damage;
	if (_HP <= 0)
	{
		_Item->Appear(_X, _Y);
		ChangeState(DYING);
	}
}

void CFlyingCapsule::Destroyed()
{
	_Live_State = DESTROYED;
}

CFlyingCapsule::~CFlyingCapsule(){}