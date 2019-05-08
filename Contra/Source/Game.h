#include <windows.h>
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Global.h"

#define KEY_DOWN(code) ( IsKeyDown(code) )
#define KEYBOARD_BUFFER_SIZE	1024

class CGame
{
protected:
	void _Init();						//INIT COMPONENTS FOR GAME

	DWORD _dt;
	int _FrameRate;

	int _ScreenWidth;
	int _ScreenHeight;
	int _Depth;

	LPCSTR _Name;

	BYTE  _KeyStates[256];
	int IsKeyDown(int KeyCode);		//CHECK KEYBOARD DOWN
	DIDEVICEOBJECTDATA _KeyEvents[KEYBOARD_BUFFER_SIZE];

	static LRESULT CALLBACK _winProc(HWND hwnd, UINT messeage, WPARAM wParam, LPARAM lParam);

	void InitWindow();
	void InitDirectX();

	void RenderFrame();

	void _InitKeyboard();
	void _ProcessKeyBoard();

	virtual void RenderFrames();
	virtual void LoadResource(LPDIRECT3DDEVICE9 d3ddv);
	virtual void ProcessInput();
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	virtual void Update(float delta);
	//LPDIRECT3DSURFACE9 CreateSurfaceFromFile(LPDIRECT3DDEVICE9 d3ddv, LPWSTR FilePath);

public:
	CGame(HINSTANCE hInstance,LPCSTR name, int FrameRate);
	~CGame();
	void Init();
	void Run();
};