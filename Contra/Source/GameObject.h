#pragma once

#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "Global.h"

//define object đang hướng về phía nào
#define FACING_LEFT -1
#define FACING_RIGHT 1

//define hướng mà object va chạm
#define COLLIDED_NONE	0
#define COLLIDED_LEFT	1
#define COLLIDED_RIGHT	2
#define COLLIDED_TOP	3
#define COLLIDED_BOT	4

//define các loại object trong game
#define OBJ_BILL					1
//Map Animation
#define OBJ_MAP_ANIMATION			3
#define OBJ_EXPLORE_ANIMATION		4
//Platform
#define OBJ_PLATFORM				5
#define OBJ_WATER					6
#define OBJ_BRIDGE					7
#define OBJ_PLATFORM_ROCK			8
//Enemy
#define OBJ_SOLDIER					10
#define OBJ_RIFLEMAN				11
#define OBJ_CANNON1					12
#define OBJ_CANNON2					13
#define OBJ_TANK					14
#define OBJ_ARTILLERY				15
#define OBJ_DIVER					16
#define OBJ_ROCK					17
#define OBJ_BOM						18
#define OBJ_FIRE					19
//Enemy Bullet
#define OBJ_ENEMY_BULLET			30
#define OBJ_LARGE_BULLET			31
#define OBJ_BOM_BULLET				32
//PlayerBullet
#define OBJ_PLAYER_BULLET			20
//Item
#define OBJ_FLYING_CAPSULE			21
#define OBJ_ITEM					22
//
#define OBJ_HIDE_RIFLEMAN			23
//
#define OBJ_FALCON					24
#define OBJ_LIFE_NUM				25
//Boss
#define OBJ_BOSS_STAGE1				90
#define OBJ_BOSS_STAGE1_GUN			91
#define OBJ_BOSS_STAGE2_HAND		92
#define OBJ_BOSS_STAGE2_STONE		93
#define OBJ_BOSS_STAGE2_EYE			94
#define OBJ_BOSS_STAGE2_MOUTH		97
#define OBJ_BOSS_STAGE2_SKIN		98
#define OBJ_BOSS_STAGE3_SHIP		95
#define OBJ_BOSS_STAGE3_SMALLSHIP	96
//Các trạng thái của object 
enum LIVE_STATE { ALIVE, DESTROYED, DYING };
class CGameObject
{
protected:

public:
	LPD3DXSPRITE _SpriteHandler;
	float _X;
	float _Y;

	int _Width;
	int _Height;

	int _Facing;

	int _HP;

	int _Type;

	float _X_Last;
	float _Y_Last;

	float _Vx;
	float _Vy;
	
	float _Vx_Last;
	float _Vy_Last;

	DWORD _Last_Time;

	float _Gravity;

	LIVE_STATE _Live_State;

	int _AnimateIndex;

	D3DCOLOR _Render_Color;

	int _Attack_Count;
	int _Time_Remain;
	int _Animation_Die_Count;
	DWORD _Last_Check;

	//Constructor và destructor
	CGameObject(int Type, float X, float Y, int Width, int Height);
	~CGameObject();

	virtual void Render();
	virtual void LoadResource();
	virtual void Update(float dt);
	virtual BOX GetBox();
	virtual void UpdateCollision();
	virtual void Destroyed();

	virtual void TakeDamaged(int damage);
	virtual void ChangeState(LIVE_STATE state);
	virtual void ProcessDying(int Time_Die, int Animation_Count, int Animation_Rate);
	virtual void ProcessAlive(float delta);
	virtual void GetNextAnimation();

	virtual RECT GetRect();

	virtual COLLISION_INFO CollidedWith(CGameObject * o);
	//int SimpleCollidedWith(CGameObject * o);

	void UpdatePosition(float Delta);
	void FixPositionCollid(CGameObject * o, int CollidPos);

	int GetFacing() { return _Facing; }
	int GetHealth(){ return _HP; }

	int GetLiveState(){ return _Live_State; }

	//Lấy hình nổ
	RECT GetDyingRect();
	LPD3DXSPRITE GetSpriteHandler() { return _SpriteHandler; }
/*
	virtual RECT GetRect()
	{
		RECT srect;
		srect = { 0, 0, 50, 70 };
		return srect;
	}*/
};

