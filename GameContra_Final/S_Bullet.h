#ifndef _S_BULLET_H
#define _S_BULLET_H

#include "PlayerBullet.h"

class CS_Bullet : public CPlayerBullet
{
public:
	CS_Bullet(int Type, float X, float Y, int damage);
	CS_Bullet();
	~CS_Bullet();
	void Update(float delta);
	void Render();
	void Fire(int numofbullet);
	RECT GetRect()
	{
		RECT srect;
		//srect = { 0 + _AnimateIndex * 10, 30, 10 + _AnimateIndex * 10, 40 };
		srect = { 0, 30, 10, 40 };
		return srect;
	}

private:

};
#endif