#include "GameOver.h"
#include "Texture.h"
#include "Resources.h"

extern LPD3DXSPRITE G_SpriteHandler;
extern LPDIRECT3DDEVICE9 G_Device;
extern LPDIRECT3DSURFACE9 G_Surface;

#define TIME_FLASH 200

CGameOver::CGameOver()
{
	LoadResource();
	G_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &G_Surface);
}

void CGameOver::LoadResource()
{
	_Image = CreateTexture(GAMEOVER_IMAGE_PATH);
}

void CGameOver::Update(float delta)
{
	if (GetTickCount() - last_time > TIME_FLASH)
	{
		last_time = GetTickCount();
		flash = !flash;
	}
}

void CGameOver::Render()
{
	G_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	RECT srect = { 0, 0, 256, 240 };
	G_SpriteHandler->Draw(
		_Image,
		&srect,
		NULL,
		&pos,
		D3DCOLOR_XRGB(255, 255, 255));

	G_SpriteHandler->End();
	if (flash)
	{
		RECT p = { 80, 20, 200, 60 }; //RECT  p
		RECT hi = { 140, 70, 210, 90 }; //RECT hi
		G_Device->ColorFill(G_Surface, &p, D3DCOLOR_XRGB(0, 0, 0));
		G_Device->ColorFill(G_Surface, &hi, D3DCOLOR_XRGB(0, 0, 0));
	}
}

CGameOver::~CGameOver()
{
}
