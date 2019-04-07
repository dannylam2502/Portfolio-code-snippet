#ifndef _ITEM_H_
#define _ITEM_H_

#include "GameObject.h"
#include <vector>
using namespace std;

enum ITEM_TYPE { Item_R, Item_L, Item_S, Item_M, Item_F, Item_B, Item_SSS };

class CItem : public CGameObject
{
protected:
public:
	ITEM_TYPE _Item_Type;
	bool _landed; //Xac dinh item rot dat
	CItem();
	CItem(int Type, float X, float Y, int Width, int Height);
	~CItem();

	void Update(float dt);
	void Render();

	void Appear(float X, float Y);

	bool CheckLanded(); //Ham kiem tra landed
	void UpdateCollision();
	RECT GetRect()
	{
		RECT srect;
		switch (_Item_Type)
		{
		case Item_R:
			srect = { 0, 70, 30, 90 };
			break;
		case Item_L:
			srect = { 0, 90, 30, 110 };
			break;
		case Item_S:
			srect = { 0, 110, 30, 130 };
			break;
		case Item_M:
			srect = { 0, 130, 30, 150 };
			break;
		case Item_F:
			srect = { 0, 150, 30, 170 };
			break;
		case Item_B:
			srect = { 0, 170, 30, 190 };
			break;
		case Item_SSS:
			srect = { 0 + _AnimateIndex * 30, 190, 30 + _AnimateIndex * 30, 210 };
			break;
		default:
			srect = { 0, 0, 0, 0 };
			break;
		}
		return srect;
	}

};
#endif