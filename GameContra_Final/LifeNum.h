#ifndef _LIFE_NUM_H_
#define _LIFE_NUM_H_

#include "GameObject.h"

class CLifeNum : public CGameObject
{
protected:
public:
	CLifeNum();
	~CLifeNum();
	void Update(float delta);
	void Render();
	RECT GetRect();
};

#endif
