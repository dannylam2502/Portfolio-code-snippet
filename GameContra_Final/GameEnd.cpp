#include "Resources.h"
#include "GameEnd.h"
#include <stdio.h>
#include <stdlib.h> 
#include "ContraGame.h"


extern LPD3DXSPRITE G_SpriteHandler;
extern LPDIRECT3DDEVICE9 G_Device;
extern LPDIRECT3DSURFACE9 G_Surface;

CGameEnd::CGameEnd()
{
	x = 0;
	y = 240;
	is_Finish = false;

	LoadResource();
}

void CGameEnd::LoadResource()
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	result = D3DXGetImageInfoFromFile(GAMEEND_IMAGE_PATH, &info);
	if (result != D3D_OK)
	{
		return;
	}

	result = D3DXCreateTextureFromFileEx(
		G_Device,
		GAMEEND_IMAGE_PATH,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		&info,
		NULL,
		&_Image);

	if (result != D3D_OK)
	{
		return;
	}
}

void CGameEnd::Update(float delta)
{
	if (!is_Finish)
	{
		y--;
		if (y <= 0)
			is_Finish = true;
	}
}

void CGameEnd::Render()
{
	//if (!is_Finish)
	{
		D3DXVECTOR3 pos;
		pos.x = x;
		pos.y = y;
		pos.z = 0;
		RECT srect = { 0, 0, 635, 330 };
		G_SpriteHandler->Draw(
			_Image,
			&srect,
			NULL,
			&pos,
			D3DCOLOR_XRGB(255, 255, 255));

		//G_SpriteHandler->End();
	}
}