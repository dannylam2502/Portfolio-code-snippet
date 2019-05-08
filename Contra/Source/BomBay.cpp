#include "BomBay.h"
#include "Texture.h"
#include "Pool_Enemy_Bullet.h"
#include "Resources.h"

extern CTexture* GameTexture;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;

#define BOMBAY_MAX_ATTACK_COUNT		1
#define BOMBAY_TIME_WAIT			500
#define	BOMBAY_ATTACK_RATE			2

CBomBay::CBomBay() :CEnemy(OBJ_BOM, 0, 0, 0, 0)
{

}

CBomBay::CBomBay(int type, float x, float y, int width, int height) : CEnemy(type, x, y, width, height)
{
	_Live_State = ALIVE;
}

CBomBay::~CBomBay()
{

}

void CBomBay::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
}

void CBomBay::ProcessAlive(float delta)
{
	ProcessAttack(BOMBAY_MAX_ATTACK_COUNT, BOMBAY_TIME_WAIT, BOMBAY_ATTACK_RATE, false);
}

void CBomBay::Attack()
{
	Pool_Enemy_Bullet->Load(this);
	PlaySound(Boom);
}

D3DXVECTOR3 CBomBay::CheckTarget()
{
	D3DXVECTOR3 result;
	result.x = 0.02f;
	result.y = 0;
	result.z = -0.0002f;
	return result;
}
D3DXVECTOR2 CBomBay::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X + BOM_WIDTH;
	result.y = _Y + BOM_HEIGHT;
	return result;
}