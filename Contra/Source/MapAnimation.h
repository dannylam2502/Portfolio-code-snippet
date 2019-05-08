#pragma once
#include "GameObject.h"

#define CELLSIZE 16

class CMapAnimation :
	public CGameObject
{
public:
	int map_sequence;
	int animation;
	int stage;

	CMapAnimation(int type, float x, float y, int width, int height, int _map_sequence, int _animation,int stage);
	
	~CMapAnimation();

	void Render(LPDIRECT3DTEXTURE9 _Animation, LPD3DXSPRITE spritehandler);
	void Update(float dt);

	RECT GetRect()
	{
		RECT srect;
		if (stage == 1)
		{
			switch (map_sequence)
			{
				//row 1
			case 0:
				srect = { 0 + _AnimateIndex *CELLSIZE, 0, 16 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 1:
				srect = { 32 + _AnimateIndex *CELLSIZE, 0, 48 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 4:
				srect = { 64 + _AnimateIndex *CELLSIZE, 0, 80 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 5:
				srect = { 96 + _AnimateIndex *CELLSIZE, 0, 112 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 6:
				srect = { 128 + _AnimateIndex *CELLSIZE, 0, 144 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 7:
				srect = { 160 + _AnimateIndex *CELLSIZE, 0, 176 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 14:
				srect = { 192 + _AnimateIndex *CELLSIZE, 0, 208 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 21:
				srect = { 224 + _AnimateIndex *CELLSIZE, 0, 240 + _AnimateIndex * CELLSIZE, 16 };
				break;
			case 40:
				srect = { 256 + _AnimateIndex *CELLSIZE, 0, 272 + _AnimateIndex * CELLSIZE, 16 };
				break;

				//row 2
			case 45:
				srect = { 0 + _AnimateIndex *CELLSIZE, 16, 16 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 46:
				srect = { 32 + _AnimateIndex *CELLSIZE, 16, 48 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 47:
				srect = { 64 + _AnimateIndex *CELLSIZE, 16, 80 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 48:
				srect = { 96 + _AnimateIndex *CELLSIZE, 16, 112 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 53:
				srect = { 128 + _AnimateIndex *CELLSIZE, 16, 144 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 67:
				srect = { 160 + _AnimateIndex *CELLSIZE, 16, 176 + _AnimateIndex * CELLSIZE, 32 };
				break;
			case 28:
				srect = { 192 + _AnimateIndex *CELLSIZE, 16, 208 + _AnimateIndex * CELLSIZE, 32 };
				break;
			}
		}

		if (stage == 2)
		{
			switch (map_sequence)
			{
				//row 3
			case 7:
				srect = { 0 + _AnimateIndex *CELLSIZE, 32, 16 + _AnimateIndex * CELLSIZE, 48 };
				break;
			case 11:
				srect = { 48 + _AnimateIndex *CELLSIZE, 32, 64 + _AnimateIndex * CELLSIZE, 48 };
				break;
			case 12:
				srect = { 96 + _AnimateIndex *CELLSIZE, 32, 112 + _AnimateIndex * CELLSIZE, 48 };
				break;
			case 14:
				srect = { 144 + _AnimateIndex *CELLSIZE, 32, 160 + _AnimateIndex * CELLSIZE, 48 };
				break;
			case 26:
				srect = { 192 + _AnimateIndex *CELLSIZE, 32, 208 + _AnimateIndex * CELLSIZE, 48 };
				break;
			case 27:
				srect = { 240 + _AnimateIndex *CELLSIZE, 32, 256 + _AnimateIndex * CELLSIZE, 48 };
				break;
				//row 4
			case 29:
				srect = { 0 + _AnimateIndex *CELLSIZE, 48, 16 + _AnimateIndex * CELLSIZE, 64 };
				break;
			case 32:
				srect = { 48 + _AnimateIndex *CELLSIZE, 48, 64 + _AnimateIndex * CELLSIZE, 64 };
				break;
			case 33:
				srect = { 96 + _AnimateIndex *CELLSIZE, 48, 112 + _AnimateIndex * CELLSIZE, 64 };
				break;
			case 40:
				srect = { 144 + _AnimateIndex *CELLSIZE, 48, 160 + _AnimateIndex * CELLSIZE, 64 };
				break;
			case 47:
				srect = { 192 + _AnimateIndex *CELLSIZE, 48, 208 + _AnimateIndex * CELLSIZE, 64 };
				break;
			case 48:
				srect = { 240 + _AnimateIndex *CELLSIZE, 48, 256 + _AnimateIndex * CELLSIZE, 64 };
				break;

				//row 5
			case 50:
				srect = { 0 + _AnimateIndex *CELLSIZE, 64, 16 + _AnimateIndex * CELLSIZE, 80 };
				break;
			case 51:
				srect = { 48 + _AnimateIndex *CELLSIZE, 64, 64 + _AnimateIndex * CELLSIZE, 80 };
				break;
			case 52:
				srect = { 96 + _AnimateIndex *CELLSIZE, 64, 112 + _AnimateIndex * CELLSIZE, 80 };
				break;
			case 58:
				srect = { 144 + _AnimateIndex *CELLSIZE, 64, 160 + _AnimateIndex * CELLSIZE, 80 };
				break;
			case 60:
				srect = { 192 + _AnimateIndex *CELLSIZE, 64, 208 + _AnimateIndex * CELLSIZE, 80 };
				break;
			case 61:
				srect = { 240 + _AnimateIndex *CELLSIZE, 64, 256 + _AnimateIndex * CELLSIZE, 80 };
				break;

				//row 6
			case 74:
				srect = { 0 + _AnimateIndex *CELLSIZE, 80, 16 + _AnimateIndex * CELLSIZE, 96 };
				break;
			case 75:
				srect = { 48 + _AnimateIndex *CELLSIZE, 80, 64 + _AnimateIndex * CELLSIZE, 96 };
				break;
			case 125:
				srect = { 96 + _AnimateIndex *CELLSIZE, 80, 112 + _AnimateIndex * CELLSIZE, 96 };
				break;
			}
		}

		if (stage == 3)
		{
			switch (map_sequence)
			{
				//row 7
			case 79:
				srect = { 0 + _AnimateIndex *CELLSIZE, 96, 16 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 80:
				srect = { 32 + _AnimateIndex *CELLSIZE, 96, 48 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 82:
				srect = { 64 + _AnimateIndex *CELLSIZE, 96, 80 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 83:
				srect = { 96 + _AnimateIndex *CELLSIZE, 96, 112 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 84:
				srect = { 128 + _AnimateIndex *CELLSIZE, 96, 144 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 85:
				srect = { 160 + _AnimateIndex *CELLSIZE, 96, 176 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 86:
				srect = { 192 + _AnimateIndex *CELLSIZE, 96, 208 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 87:
				srect = { 224 + _AnimateIndex *CELLSIZE, 96, 240 + _AnimateIndex * CELLSIZE, 112 };
				break;
			case 88:
				srect = { 256 + _AnimateIndex *CELLSIZE, 96, 272 + _AnimateIndex * CELLSIZE, 112 };
				break;

				//row 8
			case 89:
				srect = { 0 + _AnimateIndex *CELLSIZE, 112, 16 + _AnimateIndex * CELLSIZE, 128 };
				break;
			}
		}
		return srect;
	}
	void GetNextAnimation();

	void setPosition(float x, float y)
	{
		_X = x;
		_Y = y;
	}
};

