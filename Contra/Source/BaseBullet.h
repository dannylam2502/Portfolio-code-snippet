#ifndef _BASE_BULLET_H
#define _BASE_BULLET_H

#include "GameObject.h"

class CBaseBullet : public CGameObject
{
protected:
public:
	float _Angle;
	POINT _Directon;
	CBaseBullet();
	CBaseBullet(int Type, float X, float Y, int damage);
	~CBaseBullet();

	int _Damage;

	virtual void Update(float delta);
	virtual void Render();
	virtual void Fire();
	virtual void Fire(int i);
	virtual void Destroyed();
};

#endif