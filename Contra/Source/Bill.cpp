#include "Bill.h"
#include "Texture.h"
#include "Resources.h"
#include "PoolBullet.h"
#include "WorldMap.h"
#include "Resources.h"
#include "Ship.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern CPoolBullet* PoolBullet;
extern vector<CGameObject *> list_Obj_Handle;
extern STAGE stage;
extern CShip* Ship;

#define BILL_SPEED				0.06f//0.17f//0.07f
#define BILL_JUMP_SPEED			0.18f//0.18f
#define BILL_ANIMATE_RATE		8
#define BILL_ATTACK_RATE		10
#define BILL_TIME_DIE			4000
#define BILL_TIME_INVUL			5000
#define BILL_FLASH_RATE			10

#define BILL_WIDTH				7
#define BILL_HEIGHT				30


#define ANIMATION_MOVE_HEAD		6
#define ANIMATION_MOVE_LEG		3
#define ANIMATION_JUMP			4
#define ANIMATION_TOP_HEAD		3
#define ANIMATION_BOT_HEAD		3
#define ANIMATION_STAND			2
#define ANIMATION_LIE			2
#define ANIMATION_UP			2
#define ANIMATION_DIE			5

#define TIME_SHOOTING			200
#define START_LIFE_NUM			3

CBill::CBill() :CGameObject(OBJ_BILL, 50, 170, BILL_WIDTH, BILL_HEIGHT)
{
	//_Gravity = 0;
	//_State = STAND;
	_State_Head = STAND;
	_State_Leg = STAND;
	_Action = REST;
	_Facing = FACING_RIGHT;
	_Is_Control = true;
	_AnimateIndex_Head = 0;
	_AnimateIndex_Leg = 0;
	_Animation_Rate = BILL_ANIMATE_RATE;
	_Live_State = ALIVE;
	_Last_Shoot = GetTickCount();
	_direction.x = 0;
	_direction.y = 0;
	_Is_Swim = false;
	_Life_Num = START_LIFE_NUM;
}

void CBill::Damaged(int damage, int x, int y)
{

}
void CBill::Render()
{
	if (!_Is_Swim)	GameTexture->Render(GetRectLeg(), this, G_ViewPort);
	GameTexture->Render(GetRectHead(), this, G_ViewPort);
}
void CBill::LoadResource()
{

}
void CBill::Update(float dt)
{
	UpdatePosition(dt);

	_Vx_Last = _Vx;
	_Vy_Last = _Vy;
	if (_X > _X_Last) _X_Last = _X;
	if (_Y > _Y_Last) _Y_Last = _Y;

	if (_State_Head == DIE)
	{
		ProcessDying();
		return;
	}
	//Process Invul
	ProcessInvul();
	//process facing
	ProcessFacing();

	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / _Animation_Rate)
	{
		_Last_Time = now;
		GetNextAnimation();
	}

	if (_Is_Flash)
	{
		if (_Time_Invul < BILL_TIME_INVUL)
		{
			_Render_Color = D3DCOLOR_XRGB(100, 100, 100);
		}
		else
		{
			_Render_Color = D3DCOLOR_XRGB(255, 100, 100);
		}
	}
	else
	{
		_Render_Color = D3DCOLOR_XRGB(255, 255, 255);
	}

	if (now - _Time_Shooting > TIME_SHOOTING)
	{
		_Action = REST;
	}

	if (_X != last_landing_x)
	{
		landing_obj = NULL;
	}
	UpdateCollision();
}

void CBill::UpdateCollision()
{
#pragma region Collide With Object
	for each (CGameObject* obj in list_Obj_Handle)
	{
#pragma region Collide With Platform
		if (obj->_Type == OBJ_PLATFORM || obj->_Type == OBJ_WATER || obj->_Type == OBJ_PLATFORM_ROCK)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				if (_Is_Swim)
				{
					this->FixPositionCollid(obj, COLLIDED_BOT);
					this->Land(obj);
				}
				if (this->_Y >= obj->_Y && this->_Vy < 0 && obj->_Type != OBJ_PLATFORM_ROCK)
				{
					if (co.time > 0.0f && co.time < 1.0f)
					{
						this->_X += this->_Vx * co.time;
						this->_Y += this->_Vy * co.time;
						this->Land(obj);
					}
					else
					{
						this->FixPositionCollid(obj, COLLIDED_BOT);
						this->Land(obj);
					}
				}

				if (this->_Y >= obj->_Y + 10 && this->_Vy < 0 && obj->_Type == OBJ_PLATFORM_ROCK)
				{
					if (co.time > 0.0f && co.time < 1.0f)
					{
						this->_X += this->_Vx * co.time;
						this->_Y += this->_Vy * co.time;
						this->Land(obj);
					}
					else
					{
						this->FixPositionCollid(obj, COLLIDED_BOT);
						this->Land(obj);
					}
				}
			}
		}
		if (obj->_Type == OBJ_BRIDGE)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				if (GetCollisionPosition(co) == COLLIDED_BOT)
				{
					this->FixPositionCollid(obj, COLLIDED_BOT);
					this->Land(obj);
				}
			}
		}

#pragma endregion
		if (obj->_Type >= OBJ_SOLDIER && obj->_Type <= OBJ_FIRE && !_Is_Invul)
		{
			if (obj->_Type == OBJ_CANNON1 || obj->_Type == OBJ_CANNON2)
			{
			}
			else
			{
				COLLISION_INFO co = this->CollidedWith(obj);
				if (co._IsCollided)
				{
					this->Die(obj->_Vx);
				}
			}
		}
	}
#pragma endregion

	if (!_Is_Invul)
	{
		for each (CSmallSpaceShip *obj in Ship->list_SmallShip)
		{
			COLLISION_INFO co = this->CollidedWith(obj);
			if (co._IsCollided)
			{
				this->Die(_Vx);
			}
		}

		if (Ship->state != DISAPEAR)
		{
			COLLISION_INFO co = this->CollidedWith(Ship);
			if (co._IsCollided)
			{
				this->Die(_Vx);
			}
		}
	}

#pragma region Collide With ViewPort
	if (_X < G_ViewPort._X + 10) // Prevent Bill go to left, just like game
	{
		_X = G_ViewPort._X + 10;
	}
	if (stage == STAGE2)
	{
		if (_X > G_ViewPort._X + G_ViewPort._Width - 20) // just like game
		{
			_X = G_ViewPort._X + G_ViewPort._Width - 20;
		}
	}
	if (_Y < G_ViewPort._Y)
	{
		if (_State_Head != DIE)
		{
			_Y = G_ViewPort._Y + 10;
		}
		this->Die(0);
	}
#pragma endregion



}

void CBill::TriggerControl(bool Up, bool Down, bool Left, bool Right)
{
	//_direction = GetDirection(Up, Down, Left, Right);
	if (!_Is_Control)
		return;

	if (_Live_State == DYING || _Live_State == DESTROYED)
		return;

	if ((Up || Down || Left || Right) == false)
	{
		Rest();
		return;
	}

	if (Right || Left)
	{
		if (Right) _Facing = FACING_RIGHT;
		if (Left) _Facing = FACING_LEFT;
		Move(Up, Down);
		return;
	}
	if (Up)
	{
		StandUp();
		return;
	}
	if (Down)
	{
		Lie();
		return;
	}
}

void CBill::Rest()
{
	if (_State_Head == JUMP || _State_Head == DOWN) return;
	if (landing_obj != NULL)
	{
		_Vx = landing_obj->_Vx; 		//di chuyển theo nếu obj có vận tốc
	}
	else
	{
		_Vx = 0;
	}
	SetStateHead(STAND);
	SetStateLeg(STAND);
}
void CBill::Jump()
{
	if (!_Is_Control) return;
	if (_State_Head == JUMP || _State_Head == DOWN || this->_Is_Swim) return;
	if (_State_Head == LIE)
	{
		SetStateHead(DOWN);
		SetStateLeg(NONE);
		this->_Y -= 10;//"jump" Down 10, just like game 
	}
	else
	{
		_Vy = BILL_JUMP_SPEED;
		SetStateHead(JUMP);
		SetStateLeg(NONE);
	}

}
void CBill::Move(bool Up, bool Down)
{
	if (_Facing == FACING_RIGHT)
	{
		_Vx = BILL_SPEED;
	}
	if (_Facing == FACING_LEFT)
	{
		_Vx = -BILL_SPEED;
	}
	if (_State_Head != JUMP)
	{
		if (landing_obj == NULL)
		{
			SetStateHead(DOWN);
			SetStateLeg(NONE);
		}
		else
		{
			if (Up) SetStateHead(MOVE_TOP);
			else if (Down) SetStateHead(MOVE_BOT);
			else SetStateHead(MOVE);
			SetStateLeg(MOVE);
		}

	}
}
void CBill::Lie()
{
	if (_State_Head != JUMP && _State_Head != DOWN)
	{
		_Vx = 0;
		SetStateHead(LIE);
		SetStateLeg(NONE);
	}
}
void CBill::StandUp()
{
	if (_State_Head != JUMP)
	{
		_Vx = 0;
		SetStateHead(UP);
		SetStateLeg(NONE);
	}
}
void CBill::Shoot()
{
	if (!_Is_Control) return;
	if (PoolBullet->CheckNum() < 0 && PoolBullet->_Bullet_Type != L) return;// Nếu không còn đạn trong Pool
	_Action = SHOOT;
	_Time_Shooting = GetTickCount();
	if (_State_Head != JUMP)
	{
		GetNextHeadAnimation();
	}
	PoolBullet->Load();
}

void CBill::Die(float Vx)
{
	if (_State_Head == DIE) return;
	_Is_Control = false;
	if (Vx > 0)
	{
		_Vx = BILL_SPEED;
	}
	else
	{
		_Vx = -BILL_SPEED;
	}
	_Vy = BILL_JUMP_SPEED / 2;
	_Time_Die = BILL_TIME_DIE;
	_AnimateIndex = 0;
	SetStateHead(DIE);
	SetStateLeg(NONE);
	PoolBullet->ChangeType(BASIC);
	_Last_Invul_Time = GetTickCount();
	PlaySound(Bill_Dead, 10);
}
void CBill::ProcessDying()
{
	UpdateCollision();
	if (_Time_Die > 0)
	{
		_Time_Die -= 30; // fps
		DWORD now = GetTickCount();
		if (_AnimateIndex < ANIMATION_DIE - 1)
		{
			if (now - _Last_Time > 1000 / _Animation_Rate)
			{
				_Last_Time = now;
				_AnimateIndex++;
				GetNextAnimation();
			}
		}
		else
		{
			_Vx = 0;
		}
	}
	else
	{
		ProcessReborn();
	}

}

void CBill::ProcessReborn()
{
	_Life_Num--;
	if (_Life_Num <= 0) return;
	if (stage == STAGE2)
	{
		for each (CGameObject *obj in list_Obj_Handle)
		{
			if (obj->_Type == OBJ_PLATFORM && obj->_Y <= 80 && obj->_X < 80)
			{
				_X = obj->_X + 8;
				_Y = obj->_Y + 35;
				break;
			}

			/*if (obj->_Type == OBJ_PLATFORM &&  obj->_Y - _Y <= 35)
			{
			_X = obj->_X + 8;
			_Y = obj->_Y + 8;
			break;
			}*/

		}
	}
	else
	{
		_X = G_ViewPort._X + 30;//Just like game
		//_Y = G_ViewPort._Y + G_ViewPort._Height - _Height;
		_Y = G_ViewPort._Y + G_ViewPort._Height - 70;
		//_X_Last = _X;
		//_Y_Last = _Y;
	}
	SetStateHead(JUMP);
	_Vy = 0;
	_Is_Control = true;
	_Is_Invul = true;
	_Time_Invul = BILL_TIME_INVUL;
}

void CBill::ProcessInvul()
{
	if (_Time_Invul > 0)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Flash > 1000 / BILL_FLASH_RATE)
		{
			_Is_Flash = !_Is_Flash;
			_Last_Flash = now;
		}
		if (now - _Last_Invul_Time >= 30)
		{
			_Time_Invul -= (now - _Last_Invul_Time);//fps
			_Last_Invul_Time = now;
		}
		//_Time_Invul -= 30;
		_Is_Invul = true;
	}
	else
	{
		_Is_Invul = false;
		_Is_Flash = false;
	}
}

void CBill::Land(CGameObject* obj)
{
	if (obj != landing_obj)
	{
		landing_obj = obj;
		last_landing_x = _X;
		last_landing_y = _Y;
		if (last_landing_x < obj->_X)
			last_landing_x = obj->_X;
		else if (last_landing_x + _Width > obj->_X + obj->_Width)
			last_landing_x = obj->_X + obj->_Width - _Width;
	}
	if (landing_obj->_Type == OBJ_WATER)	_Is_Swim = true;
	else _Is_Swim = false;
	if (_State_Head == JUMP || _State_Head == DOWN)
	{
		_Vx = 0;
		_State_Head = STAND;
		PlaySound(Bill_Jump);
	}
}


void CBill::GetNextAnimation()
{
	if (_State_Head == JUMP)
	{
		this->GetNextHeadAnimation(ANIMATION_JUMP);
	}
	if (_Vx != 0)
	{
		switch (_State_Head)
		{
		case MOVE:
			if (_Action != SHOOT) 	this->GetNextHeadAnimation(ANIMATION_MOVE_HEAD);
			else this->GetNextHeadAnimation(ANIMATION_STAND);
			this->GetNextLegAnimation(ANIMATION_MOVE_LEG);
			break;
		case MOVE_TOP:
			this->GetNextHeadAnimation(ANIMATION_TOP_HEAD);
			this->GetNextLegAnimation(ANIMATION_MOVE_LEG);
			break;
		case MOVE_BOT:
			this->GetNextHeadAnimation(ANIMATION_BOT_HEAD);
			this->GetNextLegAnimation(ANIMATION_MOVE_LEG);
			break;
		case DIE:
			this->GetNextHeadAnimation(ANIMATION_DIE);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (_State_Head)
		{
		case LIE:
			if (_Action == SHOOT)
				GetNextHeadAnimation();
			else _AnimateIndex_Head = 0;
			break;
		case STAND:
			if (_Action == SHOOT)
				GetNextHeadAnimation();
			else _AnimateIndex_Head = 0;
			break;
		case DOWN:
			_AnimateIndex_Head = 0;
			break;
		default:
			break;
		}
	}
}

void CBill::SetStateHead(STATE state)
{
	if (_State_Head != state)
	{
		_State_Head = state;
		_AnimateIndex_Head = 0;
		if (_State_Head == DOWN)
		{
			_Vx = 0.0f;
		}
	}
}
void CBill::SetStateLeg(STATE state)
{
	if (_State_Leg != state)
	{
		_State_Leg = state;
		_AnimateIndex_Leg = 0;
	}
}
void CBill::GetNextHeadAnimation()
{
	switch (_State_Head)
	{
	case STAND:
		GetNextHeadAnimation(ANIMATION_STAND);
		break;
	case MOVE:
		if (_Action == SHOOT) GetNextHeadAnimation(ANIMATION_STAND);
		else GetNextHeadAnimation(ANIMATION_MOVE_HEAD);
		break;
	case LIE:
		GetNextHeadAnimation(ANIMATION_LIE);
		break;
	case DIE:
		GetNextHeadAnimation(ANIMATION_DIE);
		break;
	case JUMP:
		GetNextHeadAnimation(ANIMATION_JUMP);
		break;
	case MOVE_TOP:
		GetNextHeadAnimation(ANIMATION_TOP_HEAD);
		break;
	case MOVE_BOT:
		GetNextHeadAnimation(ANIMATION_BOT_HEAD);
		break;
	case UP:
		GetNextHeadAnimation(ANIMATION_UP);
		break;
	case NONE:
		break;
	default:
		break;
	}
}

RECT CBill::GetRectHead()
{
	RECT srect;
	switch (_State_Head)
	{
	case STAND:
		if (_Action == SHOOT) srect = { 13 + _AnimateIndex_Head * 25, 72, 25 + 13 + _AnimateIndex_Head * 25, 36 + 72 };
		else
		if (_Is_Swim)
		{
			srect = { 110, 240, 110 + 25, 240 + 36 };
		}
		else	srect = { 13, 72, 25 + 13, 36 + 72 };
		break;
	case MOVE:
		if (_Action == SHOOT) srect = { 13 + _AnimateIndex_Head * 25, 72, 25 + 13 + _AnimateIndex_Head * 25, 36 + 72 };
		else
		if (_Is_Swim)
		{
			srect = { 110, 240, 110 + 25, 240 + 36 };
		}
		else srect = { 12 + _AnimateIndex_Head * 25, 340, 12 + 25 + _AnimateIndex_Head * 25, 340 + 36 };
		break;
	case JUMP:
		srect = { 0 + _AnimateIndex_Head * 25, 180, 25 + _AnimateIndex_Head * 25, 180 + 25 };
		break;
	case LIE:
		if (_Is_Swim)
		{
			srect = { 0, 281, 35, 299 };
		}
		else srect = { 0 + _AnimateIndex_Head * 35, 262, 35 + _AnimateIndex_Head * 35, 18 + 262 };
		//srect = { 91, 310, 126, 330 };
		break;
	case MOVE_TOP:
		srect = { 13 + _AnimateIndex_Head * 25, 106, 25 + 13 + _AnimateIndex_Head * 25, 36 + 106 };
		break;
	case MOVE_BOT:
		srect = { 13 + _AnimateIndex_Head * 25, 144, 25 + 13 + _AnimateIndex_Head * 25, 36 + 144 };
		break;
	case UP:
		if (_Is_Swim)
		{
			srect = { 100 + _AnimateIndex_Head * 25, 180, 125 + _AnimateIndex_Head * 25, 226 };
		}
		else srect = { 13 + _AnimateIndex_Head * 25, 216, 25 + 13 + _AnimateIndex_Head * 25, 46 + 216 };
		break;
	case DOWN:
		srect = { 125, 0, 150, 40 };
		break;
	case DIE:
		srect = { 0 + _AnimateIndex_Head * 24, 310, 24 + _AnimateIndex_Head * 24, 310 + 36 };
		if (_AnimateIndex_Head > 3)
		{
			srect = { 91, 310, 126, 330 };
		}
		break;
	default:
		//srect = { 0, 0, 0, 0 };
		break;
	}
	return srect;
}
BOX CBill::GetBox()
{
	if (_State_Head == LIE || _State_Head == DIE) return BOX(_X - 10, _Y, 30, 10);
	//if (_Facing == FACING_RIGHT)	return BOX(_X + float(_Width / 2) - 5.0f, _Y, _Width / 2, _Height, _Vx, _Vy);
	if (_Facing == FACING_RIGHT)	return BOX(_X + 5.0f, _Y, _Width, _Height, _Vx, _Vy);
	if (_Facing == FACING_LEFT)		return BOX(_X + float(_Width / 2) + 5.0f, _Y, _Width, _Height, _Vx, _Vy);
}

POINT CBill::GetDirection(bool Up, bool Down, bool Left, bool Right)
{
	POINT p;
	p.x = 0;
	p.y = 0;
	switch (_Facing)
	{
	case FACING_LEFT:
		p.x = -1;
		break;
	default:
		p.x = 1;
		break;
	}
	if (Up)
	{
		p.y = 1;
		if ((Down || Left || Right) == false) p.x = 0;
	}
	else if (Down && _State_Head != LIE)
	{
		p.y = -1;
		if ((Up || Left || Right) == false) p.x = 0;
	}
	else p.y = 0;
	if (Up && Down) p.y = 0;

	return p;
}

void CBill::ProcessFacing()
{
	//process facing
	if (landing_obj != NULL)
	{
		if (landing_obj->_Type != OBJ_PLATFORM_ROCK)
		{
			if (_Vx > 0) _Facing = FACING_RIGHT;
			else if (_Vx < 0) _Facing = FACING_LEFT;
			else if (_Vx_Last < 0) _Facing = FACING_LEFT;
			else if (_Vx_Last > 0) _Facing = FACING_RIGHT;
		}
	}
}
