#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class CGameOver {
protected:

	bool is_finish;
	bool is_open;
	bool flash;
	DWORD open_time;
	DWORD last_time;
public:
	LPDIRECT3DTEXTURE9 _Image;
	LPCSTR Source;

	CGameOver::CGameOver();

	void LoadResource();
	void Update(float Delta);
	void Render();

	~CGameOver();
};


