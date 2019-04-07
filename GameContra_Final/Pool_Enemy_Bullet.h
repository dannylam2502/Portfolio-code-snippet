#ifndef _POOL_ENEMY_BULLET_H_
#define _POOL_ENEMY_BULLET_H_

#include "EnemyBullet.h"
#include <vector>

using namespace std;

class CPool_Enemy_Bullet
{
protected:
public:

	vector<CEnemy_Bullet*> _List_Small;
	vector<CEnemy_Bullet*> _List_Large;
	vector<CEnemy_Bullet*> _List_Moon;
	vector<CEnemy_Bullet*> _List_D;
	vector<CEnemy_Bullet*> _List_Bom;

	CPool_Enemy_Bullet();

	void Load(CEnemy* Source);
	void Load(int Type, float X, float Y, float Vx, float Vy, float Gravity);
	void AddBullet();
	void Update(float delta);
	void Render();

	vector<CEnemy_Bullet*> GetListByType(int Type);
	int CheckNum(int Type);

	~CPool_Enemy_Bullet();
};

#endif