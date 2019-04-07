#ifndef _FLYINGCAPSULE_H_
#define _FLYINGCAPSULE_H_

#include "GameObject.h"
#include "Item.h"
class CFlyingCapsule : public CGameObject
{
protected:
public:
	float _Angle;
	CItem* _Item;
	ITEM_TYPE _Item_Type;
	bool _IsWaiting;

	CFlyingCapsule(int Type, float X, float Y, int Width, int Height,int Item_type);
	~CFlyingCapsule();

	void Update(float dt);
	void Render();
	void ProcessAlive(float delta);
	void Destroyed();
	void Appear(float X, float Y);
	void TakeDamaged(int damage);
	RECT GetRect()
	{
		RECT srect;
		srect = { 0, 50, 30, 70 };
		return srect;
	}
	RECT GetDyingRect()
	{
		RECT srect;
		srect = { 0 + _Animation_Die_Count * 30, 290, 30 + _Animation_Die_Count * 30, 320 };
		return srect;
	}
};

#endif