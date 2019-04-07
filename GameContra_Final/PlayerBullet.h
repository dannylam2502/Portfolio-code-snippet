#ifndef _PLAYER_BULLET_H_
#define _PLAYER_BULLET_H_

#include "BaseBullet.h"

class CPlayerBullet :public CBaseBullet
{
protected:
public:
	CPlayerBullet();
	CPlayerBullet(int Type, float X, float Y, int damage);
	~CPlayerBullet();
	virtual void Update(float delta);
	virtual void Render();
	virtual void Fire();
	virtual void Fire(int i);

	void Destroyed();
};

#endif