#pragma once
#include "Enemy.h"
#include "Item.h"
#define FALCON_WIDTH 34
#define FALCON_HEIGHT 32
enum FALCON_STATE{ FALCON_CLOSE, FALCON_OPEN, FALCON_CLOSING, FALCON_OPENING };
class CFalcon :
	public CEnemy
{
public:
	CItem* _Item;
	ITEM_TYPE _Item_Type;
	DWORD last_state;
	CFalcon(int Type, int _x, int _y, int width, int height, int);
	void Update(float dt);
	void Render();
	void GetNextAnimation();
	void ProcessAlive(float delta);
	void TakeDamaged(int);
	FALCON_STATE _state;
	RECT GetRect()
	{
		RECT srect;
		srect = { 0 + _AnimateIndex   *FALCON_WIDTH, 411, 34 + _AnimateIndex   *FALCON_WIDTH, 443 };
		return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 197 + _Animation_Die_Count * 30, 0, 227 + _Animation_Die_Count * 30, 30 };
		return srect;
	}
	void Destroyed();
	void Appear(float X, float Y);
	~CFalcon();
};




