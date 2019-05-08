#include "Right_Hand_Boss_Stage2.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"
#include "PoolBullet.h"

#define NUM_STONE 5
#define APPEARED_SPEED 0.5f
#define TIME_APPEAR	4000
#define TIME_SWING 3500
#define TIME_SPIN 1000
#define TIME_HUNG 1000
#define TIME_FINISH_MOVE 1500
#define TIME_LOCK_TARGET 4000
#define TIME_PREPARE 600
#define ANGLE_W_OFFSET 0.1f

#define TIME_BETWEEN_ATTACK 2000

#define RIGHT_HAND_BULLET_SPPED 0.07f
#define RIGHT_HAND_ATTACK_COUNT 1
#define RIGHT_HAND_ANIMATE_RATE	5

#define X_VARY	0.5f//1.5f
#define Y_VARY 0.63f//1.0f

extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
extern CPoolBullet* PoolBullet;

CRight_Hand_Boss::CRight_Hand_Boss() :CEnemy(OBJ_BOSS_STAGE2_HAND, 0, 0, 100, 100)
{
	ChangeState(RIGHT_APPEAR);
	_Time_Wait = 0;
	Create();
}
CRight_Hand_Boss::CRight_Hand_Boss(int Type, float X, float Y, float Width, float Height) :CEnemy(Type, X, Y, Width, Height)
{
	ChangeState(RIGHT_APPEAR);
	_Time_Wait = 0;
	Create();
}
CRight_Hand_Boss::~CRight_Hand_Boss()
{

}

void CRight_Hand_Boss::Update(float delta)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(delta);
}

void CRight_Hand_Boss::ProcessAlive(float delta)
{
	for each (CBoss_Stone* stone in _List_Stone)
	{
		stone->Update(delta);
	}

	ProcessState();
	ProcessStone();
	UpdateCollision();
}

void CRight_Hand_Boss::UpdateCollision()
{
	for each (CBaseBullet* bullet in PoolBullet->GetListBullet())
	{
		COLLISION_INFO co;
		co = bullet->CollidedWith(_List_Stone.at(NUM_STONE - 1));
		if (co._IsCollided)
		{
			for each (CBoss_Stone* stone in _List_Stone)
			{
				stone->TakeDamaged(bullet->_Damage);
			}
			bullet->Destroyed();
		}
	}
}
void CRight_Hand_Boss::Render()
{
	for each (CBoss_Stone* stone in _List_Stone)
	{
		stone->Render();
	}
}

void CRight_Hand_Boss::ProcessStone()
{
	/*for (int i = 1; i < _List_Stone.size(); i++)
	{
	if (_List_Stone.at(i - 1)->_IsUp) _List_Stone.at(i)->_IsUp = true;
	else _List_Stone.at(i)->_IsUp = false;
	}*/

	switch (_State_Right)
	{
	case RIGHT_SWING:
		Swing();
		break;
	case RIGHT_APPEAR:
		Appear();
		break;
	case RIGHT_SPIN:
		Spin();
		break;
	case RIGHT_HUNG:
		Hung();
		break;
	case RIGHT_FINISH_MOVE:
		Finish();
		break;
	case RIGHT_LOCK_TARGET:
		LockTarget();
		break;
	case RIGHT_BEFORE_SWING:
		Prepare();
		break;
	default:
		break;
	}
	/*if (_List_Stone.at(0)->_Angle > ANGLE_MAX)
	{
	_List_Stone.at(0)->Wait();
	_List_Stone.at(0)->_Angle = ANGLE_MAX;
	_List_Stone.at(0)->_IsUp = false;
	}
	if (_List_Stone.at(0)->_Angle < ANGLE_MIN)
	{
	_List_Stone.at(0)->Wait();
	_List_Stone.at(0)->_Angle = ANGLE_MIN;
	_List_Stone.at(0)->_IsUp = true;
	}

	if (_List_Stone.at(1)->_Angle > ANGLE_MAX)
	{
	_List_Stone.at(1)->Wait();
	_List_Stone.at(1)->_Angle = ANGLE_MAX;
	_List_Stone.at(1)->_IsUp = false;
	}
	if (_List_Stone.at(1)->_Angle < ANGLE_MIN)
	{
	_List_Stone.at(1)->Wait();
	_List_Stone.at(1)->_Angle = ANGLE_MIN;
	_List_Stone.at(1)->_IsUp = true;
	}*/
}

void CRight_Hand_Boss::ProcessState()
{
	DWORD now = GetTickCount();
	switch (_State_Right)
	{
	case RIGHT_SWING:
		if (now - _Last_State > TIME_SWING)
		{
			ChangeState(RIGHT_SPIN);
		}
		break;
	case RIGHT_APPEAR:
		if (now - _Last_State > TIME_APPEAR)
		{
			ChangeState(RIGHT_BEFORE_SWING);
		}
		break;
	case RIGHT_SPIN:
		if (now - _Last_State > TIME_SPIN)
		{
			ChangeState(RIGHT_HUNG);
		}
		break;
	case RIGHT_HUNG:
		if (now - _Last_State > TIME_HUNG)
		{
			ChangeState(RIGHT_FINISH_MOVE);
		}
		break;
	case RIGHT_FINISH_MOVE:
		if (now - _Last_State > TIME_FINISH_MOVE)
		{
			ChangeState(RIGHT_LOCK_TARGET);
		}
		break;
	case RIGHT_LOCK_TARGET:
		if (now - _Last_State > TIME_LOCK_TARGET)
		{
			ChangeState(RIGHT_BEFORE_SWING);
		}
		break;
	case RIGHT_BEFORE_SWING:
		if (now - _Last_State > TIME_PREPARE)
		{
			ChangeState(RIGHT_SWING);
		}
		break;
	}
}

void CRight_Hand_Boss::Prepare()
{
	float angle = 1.0f;

	//_List_Stone.at(0)->_Angle_Max = _List_Stone.at(0)->_Angle + 2.0f;
	//_List_Stone.at(0)->_Angle_Min = _List_Stone.at(0)->_Angle - 2.0f;
	//_List_Stone.at(0)->_Angle = 0.0f;
	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		if (sin(_List_Stone.at(i)->_Angle) < 0.0f) // 0.35f = 7pi/8
		{
			_List_Stone.at(i)->_Angle += ANGLE_W_RATE*1.5f;
		}
		else
		{
			if (cos(_List_Stone.at(i)->_Angle) < cos(angle))
			{
				_List_Stone.at(i)->_Angle -= ANGLE_W_RATE*1.5f;
			}
			else
			{
				_List_Stone.at(i)->_Angle += ANGLE_W_RATE;
			}
		}
	}
	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		_List_Stone.at(i + 1)->_Angle_Max = _List_Stone.at(i)->_Angle + 0.65f;
		if (_List_Stone.at(i + 1)->_Angle > _List_Stone.at(i + 1)->_Angle_Max)
		{
			_List_Stone.at(i + 1)->Wait(1);
		}
		//_List_Stone.at(i)->Wait();
		_List_Stone.at(i + 1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
		_List_Stone.at(i + 1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
	}
}

void CRight_Hand_Boss::Swing()
{
	for (int i = 1; i < _List_Stone.size(); i++)
	{
		_List_Stone.at(i)->_X_Center = _List_Stone.at(i - 1)->_X_Center + _R*cos(_List_Stone.at(i - 1)->_Angle);
		_List_Stone.at(i)->_Y_Center = _List_Stone.at(i - 1)->_Y_Center + _R*sin(_List_Stone.at(i - 1)->_Angle);
	}
	for (int i = 1; i < _List_Stone.size(); i++)
	{

		if (_List_Stone.at(i - 1)->_IsUp)
		{
			_List_Stone.at(i)->_Angle_Max = _List_Stone.at(i - 1)->_Angle - Y_VARY;
			if (_List_Stone.at(i)->_Angle_Max >= _List_Stone.at(0)->_Angle_Max * 0.9*i)
			{
				_List_Stone.at(i)->_Angle_Max = _List_Stone.at(0)->_Angle_Max * 0.9*i;
			}
			_List_Stone.at(i)->_Angle_Min = _List_Stone.at(i - 1)->_Angle - 0.6f;
		}
		else
		{
			_List_Stone.at(i)->_Angle_Max = _List_Stone.at(i - 1)->_Angle + 0.6f;
			_List_Stone.at(i)->_Angle_Min = _List_Stone.at(i - 1)->_Angle + Y_VARY;
			if (_List_Stone.at(i)->_Angle_Min <= _List_Stone.at(0)->_Angle_Min * 0.9*i)
			{
				_List_Stone.at(i)->_Angle_Min = _List_Stone.at(0)->_Angle_Min * 0.9*i;
			}
		}
	}
}

void CRight_Hand_Boss::Appear()
{
	for each (CBoss_Stone* stone in _List_Stone)
	{
		stone->Wait();
	}
	for (int i = NUM_STONE - 1; i > 0; i--)
	{
		if (_List_Stone.at(i)->_Y_Center < _List_Stone.at(0)->_Y_Center + i * STONE_WIDTH *sin(_List_Stone.at(i)->_Angle))
		{
			if (i != NUM_STONE - 1)
			{
				if (_List_Stone.at(i + 1)->_Live_State == ALIVE)
				{
					_List_Stone.at(i)->_X_Center += APPEARED_SPEED*cos(_List_Stone.at(i)->_Angle);
					_List_Stone.at(i)->_Y_Center += APPEARED_SPEED*sin(_List_Stone.at(i)->_Angle);
				}
			}
			else
			{
				_List_Stone.at(i)->_X_Center += APPEARED_SPEED*cos(_List_Stone.at(i)->_Angle);
				_List_Stone.at(i)->_Y_Center += APPEARED_SPEED*sin(_List_Stone.at(i)->_Angle);
			}
		}
		if (_List_Stone.at(i)->_X_Center > _List_Stone.at(0)->_X_Center + STONE_WIDTH *cos(_List_Stone.at(i)->_Angle))
		{
			_List_Stone.at(i)->_Live_State = ALIVE;
		}
	}
	if (_List_Stone.at(1)->_Live_State == DESTROYED)
	{
		_Time_Wait = 500;
	}
	if (_Time_Wait > 0)
	{
		DWORD now = GetTickCount();
		if (now - _Last_Time > 30)
		{
			_Last_Time = now;
			_Time_Wait -= 30;
		}
		_List_Stone.at(0)->_Live_State = DESTROYED;
	}
	if (_Time_Wait < 0)
	{
		_List_Stone.at(0)->_Live_State = ALIVE;
	}
}

void CRight_Hand_Boss::Spin()
{
	_List_Stone.at(0)->Wait(1);
	_List_Stone.at(0)->_Angle_Max = _List_Stone.at(0)->_Angle + 2.5f;
	_List_Stone.at(0)->_Angle_Min = _List_Stone.at(0)->_Angle - 2.5f;
	_List_Stone.at(0)->_Angle += ANGLE_W_RATE;
	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		_List_Stone.at(i + 1)->_Angle_Max = _List_Stone.at(i)->_Angle + 0.65f;
		if (_List_Stone.at(i + 1)->_Angle > _List_Stone.at(i + 1)->_Angle_Max)
		{
			_List_Stone.at(i + 1)->Wait(1);
		}
		_List_Stone.at(i + 1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
		_List_Stone.at(i + 1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
		/*_List_Stone.at(i+1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(0)->_Angle - 0.65f*i);
		_List_Stone.at(i+1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(0)->_Angle - 0.65f*i);*/
	}
}

void CRight_Hand_Boss::Hung()
{
	_List_Stone.at(0)->Wait();
	_List_Stone.at(0)->_Angle_Max = _List_Stone.at(0)->_Angle + 2.5f;
	_List_Stone.at(0)->_Angle_Min = _List_Stone.at(0)->_Angle - 2.5f;
	if (sin(_List_Stone.at(0)->_Angle) > 0.37f || cos(_List_Stone.at(0)->_Angle) > 0.0f ||
		sin(_List_Stone.at(0)->_Angle) < 0.30f) // 0.35f = 7pi/8
	{
		_List_Stone.at(0)->_Angle += ANGLE_W_RATE;
	}

	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		_List_Stone.at(i + 1)->_Angle_Max = _List_Stone.at(0)->_Angle;
		if (_List_Stone.at(i + 1)->_Angle > _List_Stone.at(i + 1)->_Angle_Max)
		{
			_List_Stone.at(i + 1)->Wait(1);
		}
		_List_Stone.at(i + 1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
		_List_Stone.at(i + 1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
		/*_List_Stone.at(i+1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(0)->_Angle - 0.65f*i);
		_List_Stone.at(i+1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(0)->_Angle - 0.65f*i);*/
	}
	//for (int i = 0; i < NUM_STONE; i++)
	//{
	//	_List_Stone.at(i)->Wait();
	//	if (sin(_List_Stone.at(i)->_Angle) > 0.37f || cos(_List_Stone.at(i)->_Angle) > -0.93f ||
	//		sin(_List_Stone.at(i)->_Angle) < 0.30f || cos(_List_Stone.at(i)->_Angle) > -0.85f ) // 0.35f = 7pi/8
	//	{
	//		_List_Stone.at(i)->_Angle += ANGLE_W_RATE;
	//	}
	//}
	//for (int i = 1; i < NUM_STONE; i++)
	//{
	//	_List_Stone.at(i)->_X_Center = _List_Stone.at(i-1)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
	//	_List_Stone.at(i)->_Y_Center = _List_Stone.at(i-1)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
	//}
}

void CRight_Hand_Boss::Finish()
{
	_List_Stone.at(0)->Wait();
	_List_Stone.at(0)->_Angle_Max = _List_Stone.at(0)->_Angle + 2.5f;
	_List_Stone.at(0)->_Angle_Min = _List_Stone.at(0)->_Angle - 2.5f;
	if (sin(_List_Stone.at(0)->_Angle) > 0.9f || cos(_List_Stone.at(0)->_Angle) < 0.0f
		|| sin(_List_Stone.at(0)->_Angle) < 0.85f) // 0.35f = 7pi/8
	{
		_List_Stone.at(0)->_Angle -= ANGLE_W_RATE;
	}

	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		_List_Stone.at(i + 1)->_Angle_Min = _List_Stone.at(i)->_Angle - 0.65f;
		if (_List_Stone.at(i + 1)->_Angle < _List_Stone.at(i + 1)->_Angle_Min)
		{
			_List_Stone.at(i + 1)->Wait(1);
		}
		_List_Stone.at(i + 1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
		_List_Stone.at(i + 1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
		/*_List_Stone.at(i+1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(0)->_Angle - 0.65f*i);
		_List_Stone.at(i+1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(0)->_Angle - 0.65f*i);*/
	}
}

void CRight_Hand_Boss::LockTarget()
{
	ProcessAttack(RIGHT_HAND_ATTACK_COUNT, TIME_BETWEEN_ATTACK, RIGHT_HAND_ANIMATE_RATE, false);
	float angle = _List_Stone.at(0)->Find_Angle();
	//_List_Stone.at(0)->Wait();
	_List_Stone.at(0)->_Angle_Max = _List_Stone.at(0)->_Angle + 2.5f;
	_List_Stone.at(0)->_Angle_Min = _List_Stone.at(0)->_Angle - 2.5f;
	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		if (sin(_List_Stone.at(i)->_Angle) >= 0.0f) // 0.35f = 7pi/8
		{
			_List_Stone.at(i)->_Angle -= ANGLE_W_RATE;
		}
		else
		{
			if (cos(_List_Stone.at(i)->_Angle) > cos(angle))
			{
				_List_Stone.at(i)->_Angle -= ANGLE_W_RATE;
			}
			else
			{
				_List_Stone.at(i)->_Angle += ANGLE_W_RATE;
			}
		}
	}
	for (int i = 0; i < NUM_STONE - 1; i++)
	{
		_List_Stone.at(i)->Wait();
		//if (cos(_List_Stone.at(i+1)->_Angle) > cos(angle))
		//{
		//	_List_Stone.at(i+1)->_Angle_Min = _List_Stone.at(i)->_Angle;
		//	//_List_Stone.at(i + 1)->_Angle_Max = _List_Stone.at(i)->_Angle+0.5f;
		//}
		//else
		//{
		//	_List_Stone.at(i + 1)->_Angle_Max = _List_Stone.at(i)->_Angle;
		//	//_List_Stone.at(i + 1)->_Angle_Min = _List_Stone.at(i)->_Angle-0.5f;
		//}
		//if (_List_Stone.at(i + 1)->_Angle < _List_Stone.at(i + 1)->_Angle_Min || _List_Stone.at(i + 1)->_Angle > _List_Stone.at(i + 1)->_Angle_Max)
		//{
		//	_List_Stone.at(i + 1)->Wait(1);
		//}
		_List_Stone.at(i + 1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(i)->_Angle);
		_List_Stone.at(i + 1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(i)->_Angle);
		/*_List_Stone.at(i+1)->_X_Center = _List_Stone.at(i)->_X_Center + _R*cos(_List_Stone.at(0)->_Angle - 0.65f*i);
		_List_Stone.at(i+1)->_Y_Center = _List_Stone.at(i)->_Y_Center + _R*sin(_List_Stone.at(0)->_Angle - 0.65f*i);*/
	}
}

void CRight_Hand_Boss::ChangeState(STATE_RIGHT_HAND state)
{
	_State_Right = state;
	_Last_State = GetTickCount();
	if (_State_Right != RIGHT_APPEAR)
	{
		_R = STONE_WIDTH;
		_List_Stone.at(0)->_IsUp = true;
	}
	if (_State_Right == RIGHT_SWING)
	{
		_List_Stone.at(0)->_IsUp = false;
		_List_Stone.at(0)->_Angle_Min = ANGLE_MIN;
		_List_Stone.at(0)->_Angle_Max = ANGLE_MAX;
		for each (CBoss_Stone* stone in _List_Stone)
		{
			stone->_Angle = 1.2f;
		}
		for (int i = 1; i < NUM_STONE; i++)
		{
			_List_Stone.at(i)->_Angle_Rate = _List_Stone.at(i - 1)->_Angle_Rate/**0.8f*/;
		}
	}
	if (_State_Right == RIGHT_FINISH_MOVE)
	{
		_List_Stone.at(0)->_IsUp = false;
	}
	if (_State_Right == RIGHT_LOCK_TARGET)
	{
		_List_Stone.at(0)->_IsUp = true;
	}
	if (_State_Right == RIGHT_SPIN)
	{
		_List_Stone.at(0)->_IsUp = true;
	}
}

void CRight_Hand_Boss::Create()
{
	for (int i = 0; i < NUM_STONE; i++)
	{
		CBoss_Stone* stone = new CBoss_Stone();
		if (i == 4)
		{
			stone->_IsSpecial = true;
		}
		stone->_Angle = 1.2f;
		_List_Stone.push_back(stone);
	}

	_List_Stone.at(0)->_X_Center = 179;
	_List_Stone.at(0)->_Y_Center = 2084;

	for (int i = 1; i < NUM_STONE; i++)
	{
		_List_Stone.at(i)->_X_Center = _List_Stone.at(i - 1)->_X_Center + _R * cos(_List_Stone.at(i - 1)->_Angle);
		_List_Stone.at(i)->_Y_Center = _List_Stone.at(i - 1)->_Y_Center + _R * sin(_List_Stone.at(i - 1)->_Angle);
		_List_Stone.at(i)->_X = _List_Stone.at(i)->_X_Center - STONE_WIDTH / 2;
		_List_Stone.at(i)->_Y = _List_Stone.at(i)->_Y_Center - STONE_HEIGHT / 2;
	}
	for (int i = 1; i < NUM_STONE; i++)
	{
		_List_Stone.at(i)->_Angle_Rate = _List_Stone.at(i - 1)->_Angle_Rate*0.9f;
	}

}

void CRight_Hand_Boss::Attack()
{
	if (_List_Stone.at(NUM_STONE - 1)->_Live_State == ALIVE)
	{
		Pool_Enemy_Bullet->Load(this);
	}
}

D3DXVECTOR3 CRight_Hand_Boss::CheckTarget()
{
	D3DXVECTOR3 result;
	float angle = _List_Stone.at(NUM_STONE - 1)->Find_Angle();
	result.x = RIGHT_HAND_BULLET_SPPED * cos(angle);
	result.y = RIGHT_HAND_BULLET_SPPED * sin(angle);
	result.z = 0;
	return result;
}

D3DXVECTOR2 CRight_Hand_Boss::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _List_Stone.at(NUM_STONE - 1)->_X;
	result.y = _List_Stone.at(NUM_STONE - 1)->_Y;
	return result;
}