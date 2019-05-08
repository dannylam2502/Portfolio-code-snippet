#ifndef _EXPLORE_H_
#define _EXPLORE_H_

#include "GameObject.h"

class CExploreAnimation : public CGameObject
{
protected:
public:
	int _Time_Remain;

	CExploreAnimation();
	~CExploreAnimation();
	void Update();
	void Render(float X, float Y);
	void ChangeState(LIVE_STATE state);
	void DrawAnimation(float X, float Y);
	RECT GetRect();

};

#endif