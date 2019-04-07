#include "Menu.h"
#include <d3dx9.h>
#include <stdio.h>
#include <stdlib.h> 
#include "Resources.h"

extern LPD3DXSPRITE G_SpriteHandler;
extern LPDIRECT3DDEVICE9 G_Device;
extern LPDIRECT3DSURFACE9 G_Surface;

#define MENU_VELOCITY_BACK 0.2f
#define MENU_TIME 3000
#define FLASH_TIME 200

CMenu::CMenu()
{
	LoadResource();
	start_time = GetTickCount();
	G_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &G_Surface);
	flash = false;
	_X = SCREEN_WIDTH;
	_Y = 0;
	is_moving = true;
}

void CMenu::Reset()
{
	LoadResource();
	start_time = GetTickCount();
	flash = false;
	_X = SCREEN_WIDTH;
	_Y = 0;
	is_moving = true;
}

void CMenu::LoadResource()
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	result = D3DXGetImageInfoFromFile(MENU_IMAGE_PATH, &info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Failed to get information from image file!", "Error", NULL);
		return;
	}

	_Width = info.Width;
	_Height = info.Height;

	result = D3DXCreateTextureFromFileEx(
		G_Device,
		MENU_IMAGE_PATH,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(250, 255, 250),
		&info,
		NULL,
		&_Image);

	if (result != D3D_OK)
	{
		MessageBox(NULL, "Failed to get information from image file!", "Error", NULL);
		return;
	}
}

void CMenu::Update(float Delta)
{
	if (is_moving)
	{
		_X -= MENU_VELOCITY_BACK * Delta;
		if (_X <= 0)
		{
			is_moving = false;
		}
	}
	if (start && GetTickCount() - start_time > MENU_TIME)
		is_started = true;

	if (start && GetTickCount() - last_time > FLASH_TIME)
	{
		last_time = GetTickCount();
		flash = !flash;
	}
}

void CMenu::Start()
{
	if (!start)
	{
		PlaySound(Intro);
		start = true;
		start_time = GetTickCount();
	}
}

void CMenu::Render()
{
	D3DXVECTOR3 pos(_X, _Y, 0.0f);
	RECT srect = { 0, 0, 256, 240 };
	G_SpriteHandler->Draw(
		_Image,
		&srect,
		NULL,
		&pos,
		D3DCOLOR_XRGB(255, 255, 255));
	//CTexture::Render(srect, _X, _Y, _Image);
	G_SpriteHandler->End();
	if (flash)
	{
		RECT b = { 50, 150, 120, 160 };
		G_Device->ColorFill(G_Surface, &b, D3DCOLOR_XRGB(0, 0, 0));
	}
}

CMenu::~CMenu()
{
	if (_Image)
		_Image->Release();
	//G_SpriteHandler->Release();
}