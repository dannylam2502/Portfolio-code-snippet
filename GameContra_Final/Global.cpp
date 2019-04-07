#include "Global.h"
#include "GameObject.h"


#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <math.h>

using namespace std;
int							G_MapWidth;			//count the tiles in map width, 1 tile 50px
int							G_MapHeight;			//count the tiles in map height, 1 tile 50px

int							G_ScreenWidth;			//width of game screen
int							G_ScreenHeight;			//height of game screen

D3DFORMAT					G_BackBufferFormat;
RECT						G_WindowSize;			//size of game screen

HINSTANCE					G_hInstance;		// Handle of the game instance
HWND						G_hWnd;			// Handle of the Game Window

LPDIRECT3D9					G_DirectX;			//handle dx
LPDIRECT3DDEVICE9			G_Device;			//dx device
LPDIRECT3DSURFACE9			G_BackBuffer;		//backbuffer

LPDIRECTINPUT8				G_DirectInput;		// The DirectInput object         
LPDIRECTINPUTDEVICE8		G_KeyBoard;			// The keyboard device 
LPD3DXSPRITE				G_SpriteHandler;		
LPDIRECT3DSURFACE9			G_Surface;
VIEWPORT G_ViewPort;
float GetAngle(POINT p)
{
	if (p.x == 1 && p.y == 1) return M_PI / 4.0f;
	if (p.x == 1 && p.y == -1) return 7 * M_PI / 4.0f;
	if (p.x == 1 && p.y == 0) return 0.0f;
	if (p.x == 0 && p.y == 1) return M_PI / 2.0f;
	if (p.x == 0 && p.y == -1) return -M_PI / 2.0f;
	if (p.x == 0 && p.y == 0) return -2.0f;
	if (p.x == -1 && p.y== 1) return 3.0f * M_PI / 4.0f;
	if (p.x == -1 && p.y == -1) return -3.0f*M_PI / 4.0f;
	if (p.x == -1 && p.y == 0) return -M_PI;
	return 0.0f;
}

int GetCollisionPosition(COLLISION_INFO info)
{
	if (info._IsCollided == false) return COLLIDED_NONE;
	if (info.x < 0.0f) return COLLIDED_RIGHT;
	if (info.x > 0.0f) return COLLIDED_LEFT;
	if (info.y > 0.0f) return COLLIDED_BOT;
	if (info.y < 0.0f) return COLLIDED_TOP;
	return COLLIDED_NONE;
}