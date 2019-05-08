#ifndef _SWEPT_AABB_H_
#define _SWEPT_AABB_H_

#include <math.h>
using namespace std;
struct BOX
{
	BOX(float _x, float _y, float _w, float _h, float _vx, float _vy)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		vx = _vx;
		vy = _vy;
	}

	BOX(float _x, float _y, float _w, float _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		vx = 0.0f;
		vy = 0.0f;
	}

	// position of top-left corner
	float x, y;

	// dimensions
	float w, h;

	// velocity
	float vx, vy;
};

struct COLLISION_INFO
{
	bool _IsCollided;
	float time;
	float x;
	float y;
};

bool AABBCheck(BOX b1, BOX b2);
bool AABB(BOX b1, BOX b2, float& moveX, float& moveY);
BOX GetSweptBroadphaseBOX(BOX b);
float SweptAABB(BOX b1, BOX b2, float& normalx, float& normaly);
BOX CalculateVelocity(BOX b1, BOX b2);

#endif