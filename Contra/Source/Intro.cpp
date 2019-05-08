#include "Intro.h"

#include <d3dx9.h>
#include <stdio.h>
#include <stdlib.h> 
#include "Resources.h"
#include "WorldMap.h"
//#include "Texture.h"

extern LPD3DXSPRITE G_SpriteHandler;
extern LPDIRECT3DDEVICE9 G_Device;
extern LPDIRECT3DSURFACE9 G_Surface;
extern STAGE stage;
extern CWorldMap* WorldMap;

#define TIME_INTRO 3000
#define TIME_FLASH 200
CIntro::CIntro()
{
	LoadResource();
	//if (stage != STAGE1)
	//{
	//	WorldMap->LoadResource();
	//}
	open_time = GetTickCount();
	is_finish = false;
	is_open = true;

	G_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &G_Surface);
	flash = true;
}

void CIntro::LoadResource()
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	switch (stage)
	{
	case STAGE1:
		result = D3DXGetImageInfoFromFile(INTRO_IMAGE_PATH, &info);
		break;
	case STAGE2:
		result = D3DXGetImageInfoFromFile(INTRO_WATERFALL_IMAGE_PATH, &info);
		break;
	case STAGE3:
		result = D3DXGetImageInfoFromFile(INTRO_SNOWFIELD_IMAGE_PATH, &info);
		break;
	default:
		break;
	}
	result = D3DXGetImageInfoFromFile(INTRO_WATERFALL_IMAGE_PATH, &info);
	if (result != D3D_OK)
	{
		return;
	}
	switch (stage)
	{
	case STAGE1:
		Source = INTRO_IMAGE_PATH;
		break;
	case STAGE2:
		Source = INTRO_WATERFALL_IMAGE_PATH;
		break;
	case STAGE3:
		Source = INTRO_SNOWFIELD_IMAGE_PATH;
		break;
	default:
		break;
	}
	result = D3DXCreateTextureFromFileEx(
		G_Device,
		Source,
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

void CIntro::Update(float Delta)
{
	if (GetTickCount() - open_time > TIME_INTRO && is_open && !is_finish)
	{
		is_finish = true;
	}
	if (GetTickCount() - last_time > TIME_FLASH && is_open)
	{
		last_time = GetTickCount();
		flash = !flash;
	}

}

void CIntro::Render()
{
	D3DXVECTOR3 pos(0.0f, 50.0f, 0.0f);//112
	RECT srect = { 0, 0, 635, 330 };
	G_SpriteHandler->Draw(
		_Image,
		&srect,
		NULL,
		&pos,
		D3DCOLOR_XRGB(255, 255, 255));
	
	G_SpriteHandler->End();
	if (flash)
	{
		RECT p = { 85, 0, 200, 100 }; //RECT  p
		RECT hi = { 115, 0, 200, 150 }; //RECT hi
		G_Device->ColorFill(G_Surface, &p, D3DCOLOR_XRGB(0, 0, 0));
		G_Device->ColorFill(G_Surface, &hi, D3DCOLOR_XRGB(0, 0, 0));
	}
}

CIntro::~CIntro()
{
	//StopSound(CIntro);
	if (_Image)
		_Image->Release();
	
}