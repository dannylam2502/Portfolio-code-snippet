#ifndef _WATER_H_
#define _WATER_H_

#include "GameObject.h"

class CWater : public CGameObject
{
protected:
public:
	CWater(int Type, float X, float Y, int Width, int Height);
	CWater();
	~CWater();
};

#endif