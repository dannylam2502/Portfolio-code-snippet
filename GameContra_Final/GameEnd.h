#ifndef __GAMEEND_H__
#define __GAMEEND_H__

#include <d3d9.h>
#include <d3dx9.h>

class CGameEnd {
public:
	bool is_Finish = false;
	int x, y;

	LPDIRECT3DTEXTURE9 _Image;

	CGameEnd();

	void LoadResource();
	void Update(float Delta);
	void Render();

	~CGameEnd();
};

#endif