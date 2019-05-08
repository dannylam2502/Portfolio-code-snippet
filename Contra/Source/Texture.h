#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "GameObject.h"

class CTexture
{
protected: 
	LPDIRECT3DTEXTURE9 _BILL;
	LPDIRECT3DTEXTURE9 _MISC;
	LPDIRECT3DTEXTURE9 _BOSS;
	LPDIRECT3DTEXTURE9 _ENEMY;
	LPDIRECT3DTEXTURE9 _ENEMY_TWO;
	//LPDIRECT3DTEXTURE9 _MAP_ANIMATION;
public: 
	CTexture();
	void Render(RECT rect,CGameObject* obj, VIEWPORT G_ViewPort);
	void Render(RECT rect, CGameObject* obj, VIEWPORT G_ViewPort, float X, float Y);
	static void Render(RECT rect, float X, float Y, LPDIRECT3DTEXTURE9 image, LPD3DXSPRITE _spriteHandler);
	~CTexture();
	LPDIRECT3DTEXTURE9 GetTexture(int Type);
};
LPDIRECT3DTEXTURE9 CreateTexture(LPSTR FilePath);