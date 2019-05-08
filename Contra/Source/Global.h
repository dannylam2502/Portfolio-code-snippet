#pragma once
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "SweptAABB.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

struct VIEWPORT
{
	int _Width;		// Height of view port
	int _Height;		// Width of view port

	int _X;			// Position of view port in World space
	int _Y;
};
enum ANGLE{A_0, A_45, A_90, A_135, A_180, A_225, A_270, A_315};

extern int							G_MapWidth;			//count the tiles in map width, 1 tile 50px
extern int							G_MapHeight;			//count the tiles in map height, 1 tile 50px

extern int							G_ScreenWidth;			//width of game screen
extern int							G_ScreenHeight;			//height of game screen

extern D3DFORMAT					G_BackBufferFormat;
extern RECT							G_WindowSize;			//size of game screen

extern HINSTANCE					G_hInstance;		// Handle of the game instance
extern HWND							G_hWnd;			// Handle of the Game Window

extern LPDIRECT3D9					G_DirectX;			//handle dx
extern LPDIRECT3DDEVICE9			G_Device;			//dx device
extern LPDIRECT3DSURFACE9			G_BackBuffer;		//backbuffer

extern LPDIRECTINPUT8				G_DirectInput;		// The DirectInput object         
extern LPDIRECTINPUTDEVICE8			G_KeyBoard;			// The keyboard device 
extern LPD3DXSPRITE					G_SpriteHandler;	// spriteHandler to draw texture
extern LPDIRECT3DSURFACE9			G_Surface;
extern VIEWPORT G_ViewPort;
float GetAngle(POINT p);
int GetCollisionPosition(COLLISION_INFO info);


