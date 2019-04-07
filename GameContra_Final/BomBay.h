#pragma once
#include "Enemy.h"

#define BOM_WIDTH	14
#define BOM_HEIGHT	14

class CBomBay :
	public CEnemy

{
public:
	CBomBay();
	CBomBay(int type, float x, float y, int width, int height);
	~CBomBay();

	void Update(float dt);
	void ProcessAlive(float dt);
	void Attack();
	D3DXVECTOR3 CheckTarget();
	D3DXVECTOR2 Find_XY();
};