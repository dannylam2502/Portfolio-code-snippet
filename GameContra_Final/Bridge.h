#pragma once
#include "GameObject.h"

#define BRIDGE_WIDTH		32
#define BRIDGE_HEIGHT	16

//enum STATE_BRIGE{BRIGE_ALIVE,BRIGE_DESTROYING,BRIGE_DESTROYED};

class CBridge :
	public CGameObject
{
public:
	DWORD _last_TimeExplovise;
	CBridge();
	CBridge(int type, float x, float y, int width, int height, int _SeqcOnBri);

	int temp_x, temp_y;
	int SequenceOnBrige;

	~CBridge();

	void GetNextAnimation();
	void Update(float dt);
	void Render();

	RECT GetRect()
	{
		RECT result = { 0, 0, 0, 0 };
		if (_Live_State == ALIVE)
			result = { 0 + _AnimateIndex*BRIDGE_WIDTH, 447, 32 + _AnimateIndex*BRIDGE_WIDTH, 479 };
		if (_Live_State == DYING)
			result = { 0 + _AnimateIndex * 48, 479, 48 + _AnimateIndex * 48, 527 };
		/*if (_Live_State == DESTROYED)
		result = {200,0,233,32};*/
		return result;
	}

	RECT GetBottomRect()
	{
		RECT result = { 0, 0, 0, 0 };
		if (SequenceOnBrige == -1)
			result = { 96, 447, 128, 479 };
		if (SequenceOnBrige == 0)
			result = { 112, 447, 144, 479 };
		if (SequenceOnBrige == 1)
			result = { 128, 447, 160, 479 };
		return result;
	}
};

