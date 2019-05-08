#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_

#include "Enemy.h"

class CEnemy_Bullet : public CEnemy
{
protected:
public:

	CEnemy_Bullet(int Type, float X, float Y, int Width, int Height);

	virtual void Update(float delta);
	virtual void Render();
	virtual void Fire(float X, float Y, float Vx, float Vy);
	virtual void Fire(float X, float Y, float Vx, float Vy, float Gravity);
	void UpdateCollision();
	void CheckDestroyed();

	~CEnemy_Bullet();
};

#endif