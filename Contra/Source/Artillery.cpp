#include "Artillery.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "Pool_Enemy_Bullet.h"

#include <iostream>
using namespace std;
extern CTexture* GameTexture;
extern CBill* Bill;
extern CPool_Enemy_Bullet* Pool_Enemy_Bullet;
#define OFSET 3

#define ANIMATION_ARTILLERY		 2
#define ARTILLERY_ANIMATE_RATE	 5
#define ARTILLERY_TIME_DIE		1000
#define ARTILLERY_DIE_AINATION_COUNT 3
#define ARTILLERY_MAX_ATTACK_COUNT 2
#define ARTILLERY_TIME_WAIT 2000
#define ARTILLERYATTACK_RATE 2

CArtillery::CArtillery(int Type, float _x, float _y, int width, int height) : CEnemy(Type, _x, _y, width, height)
{
	width = ARTILLERY_WIDTH;
	height = ARTILLERY_HEIGHT;
	_HP = 10;
}
void CArtillery::Update(float dt)
{
	if (_Live_State == DESTROYED) return;
	if (_Live_State == ALIVE) ProcessAlive(dt);
	if (_Live_State == DYING)
		ProcessDying(ARTILLERY_TIME_DIE, ARTILLERY_DIE_AINATION_COUNT, ARTILLERY_ANIMATE_RATE);
}
void CArtillery::ProcessAlive(float delta)
{
	ProcessAttack(ARTILLERY_MAX_ATTACK_COUNT, ARTILLERY_TIME_WAIT, ARTILLERYATTACK_RATE, true);
}
void CArtillery::GetNextAnimation()
{
	_AnimateIndex++;
	if (_AnimateIndex >= ANIMATION_ARTILLERY) _AnimateIndex = 0;
}
void CArtillery::Attack()
{
	CheckDirection();
	Pool_Enemy_Bullet->Load(this);
}
D3DXVECTOR3 CArtillery::CheckTarget()
{
	CheckDirection();
	D3DXVECTOR3 result;
	result.x = -0.1f;
	result.y = 0;
	result.z = 0;
	return result;
}
D3DXVECTOR2 CArtillery::Find_XY()
{
	D3DXVECTOR2 result;
	result.x = _X;
	result.y = _Y + _Height / 2 + OFSET;
	return result;
}
CArtillery::~CArtillery()
{
}