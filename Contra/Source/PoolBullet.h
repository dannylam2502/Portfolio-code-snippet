#ifndef _POOLBULLET_H_
#define _POOLBULLET_H_
#include "BasicBullet.h"
#include "FBullet.h"
#include "S_Bullet.h"
#include "MBullet.h"
#include "LBullet.h"
#include <vector>
using namespace std;

enum BULLET_TYPE { BASIC, R, M, L, F, S };

#define LIMIT_BASE	4
#define LIMIT_F		4
#define LIMIT_S		10
#define LIMIT_M		6
#define LIMIT_L		1
class CPoolBullet
{
protected:
public:
	BULLET_TYPE _Bullet_Type;

	vector<CBaseBullet*> listBasic;
	vector<CBaseBullet*> listF;
	vector<CBaseBullet*> listS;
	vector<CBaseBullet*> listM;
	vector<CBaseBullet*> listL;

	int _Limit;
	void Load();
	void Update(float delta);
	void Render();
	int NumOfBulletInList();
	int CheckNum();

	void SetLimit(int num)
	{
		_Limit = num;
	}
	void SetLimitByType()
	{
		switch (_Bullet_Type)
		{
		case BASIC:
			SetLimit(LIMIT_BASE);
			break;
		case R:
			break;
		case M:
			SetLimit(LIMIT_M);
			break;
		case L:
			SetLimit(LIMIT_L);
			break;
		case F:
			SetLimit(LIMIT_F);
			break;
		case S:
			SetLimit(LIMIT_S);
			break;
		default:
			break;
		}
	}
	void AddBullet();
	void ChangeType(BULLET_TYPE Type);

	void DestroyBullet();
	void CheckHit();

	vector<CBaseBullet*> GetListBullet();
	CPoolBullet();
	~CPoolBullet();
};

#endif