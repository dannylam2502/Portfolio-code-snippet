#include "Game.h"
#include "Resources.h"
extern VIEWPORT G_ViewPort;
CGame::CGame(HINSTANCE hInstance,LPCSTR name,int FrameRate)
{
	G_DirectInput = NULL;
	G_DirectX = NULL;
	G_Device = NULL;
	G_BackBuffer = NULL;
	_ScreenWidth = SCREEN_WIDTH;
	_ScreenHeight = SCREEN_HEIGHT;
	//_ScreenWidth = 640;
	//_ScreenHeight = 480;
	G_ViewPort._Width = 256;
	G_ViewPort._Height = 240;
	G_ViewPort._X = VIEWPORT_X_START;
	G_ViewPort._Y = VIEWPORT_Y_START;
	_Name = name;
	_FrameRate = FrameRate;

	G_hInstance = hInstance;
}

void CGame::InitWindow()
{
	WNDCLASS w;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.hCursor = LoadCursor(G_hInstance, IDC_ARROW);
	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	w.hInstance = G_hInstance;
	w.lpfnWndProc = (WNDPROC)_winProc;
	w.lpszClassName = _Name;
	w.lpszMenuName = NULL;
	w.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&w)) return;
	G_hWnd = CreateWindow(_Name, "Contra", WS_OVERLAPPEDWINDOW, 0, 30, _ScreenWidth, _ScreenHeight, NULL, NULL, G_hInstance, NULL);
	if (G_hWnd == false) return;
	ShowWindow(G_hWnd, SW_SHOW);
	UpdateWindow(G_hWnd);
}

void CGame::InitDirectX()
{
	G_DirectX = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS _d3dpp;

	ZeroMemory(&_d3dpp,sizeof(_d3dpp));

	_d3dpp.Windowed = TRUE;
	_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_d3dpp.BackBufferCount = 1;
	_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	_d3dpp.BackBufferWidth = 256;
	_d3dpp.BackBufferHeight = 240;

	G_DirectX->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		G_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&_d3dpp,
		&G_Device);

	if(G_Device == NULL)
		MessageBox(NULL,"Fail to create direct divice!","ERROR",NULL);

	G_Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&G_BackBuffer);
}

void CGame::Update(float delta)
{

}

void CGame::Init()
{
	InitWindow();
	InitDirectX();
	_InitKeyboard();
	DirectSound_Init(G_hWnd);
	LoadResource(G_Device);
}

void CGame::Run()
{
	MSG msg;
	int done = 0;
	DWORD frame_start = GetTickCount();;

	DWORD tick_per_frame = 100 / _FrameRate;

	OutputDebugString(" -> Main game loop has been started ");

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();
		_dt = now - frame_start;
		//if (_dt > 30)
		//	_dt = 30;
		if (_dt >= tick_per_frame)
		{
			frame_start = now;
			RenderFrame();
			Update(_dt);
		}

		_ProcessKeyBoard();

		ProcessInput();
	}
}


void CGame::RenderFrame()
{
	if(G_Device->BeginScene())
	{
		G_Device->ColorFill(G_BackBuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));
		RenderFrames();
		G_Device->EndScene();
	}
	G_Device->Present(NULL,NULL,NULL,NULL);
}


void CGame::RenderFrames()
{
	G_Device->ColorFill(G_BackBuffer,NULL,D3DCOLOR_XRGB(0,0,0));
}

void CGame::LoadResource(LPDIRECT3DDEVICE9 d3ddv)
{
}

CGame::~CGame()
{
	if(G_DirectX != NULL) 
		G_DirectX->Release();
	if(G_Device != NULL)
		G_Device->Release();
}


void CGame::_InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(G_hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&G_DirectInput, NULL
		);

	// TO-DO: put in exception handling
	if (hr != DI_OK) return;

	hr = G_DirectInput->CreateDevice(GUID_SysKeyboard, &G_KeyBoard, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK) return;

	hr = G_KeyBoard->SetDataFormat(&c_dfDIKeyboard);

	hr = G_KeyBoard->SetCooperativeLevel(G_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size


	hr = G_KeyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (hr != DI_OK) return;

	hr = G_KeyBoard->Acquire();
	if (hr != DI_OK) return;
}
void CGame::_ProcessKeyBoard()
{
	HRESULT hr ;
	hr = G_KeyBoard->GetDeviceState(sizeof(_KeyStates), _KeyStates);
	// Collect all buffered events
	if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		G_KeyBoard->Acquire();
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = G_KeyBoard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _KeyEvents, &dwElements, 0);
	
	if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		G_KeyBoard->Acquire();
	// Scan through all data, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = _KeyEvents[i].dwOfs;
		int KeyState = _KeyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			OnKeyDown(KeyCode);
		else
			OnKeyUp(KeyCode);
	}
}
void CGame::ProcessInput()
{

}

void CGame::OnKeyDown(int KeyCode)
{

}
void CGame::OnKeyUp(int KeyCode)
{

}
int CGame::IsKeyDown(int KeyCode)
{
	return (_KeyStates[KeyCode] & 0x80) > 0;
}
LRESULT CALLBACK CGame::_winProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	}

	return DefWindowProc(hwnd,message,wParam,lParam);
}


//LPDIRECT3DSURFACE9 CGame::CreateSurfaceFromFile(LPDIRECT3DDEVICE9 d3ddv, LPWSTR FilePath)
//{
//	D3DXIMAGE_INFO info;
//
//	HRESULT result = D3DXGetImageInfoFromFile(FilePath, &info);
//	if (result != D3D_OK)
//	{
//		MessageBox(NULL, (LPCWSTR)"Failed to get image info", (LPCWSTR)"Error", NULL);
//		return NULL;
//	}
//
//	LPDIRECT3DSURFACE9 surface;
//
//	d3ddv->CreateOffscreenPlainSurface(
//		info.Width,				// width
//		info.Height,			// height
//		D3DFMT_X8R8G8B8,		// format
//		D3DPOOL_DEFAULT,
//		&surface,
//		NULL);
//
//	result = D3DXLoadSurfaceFromFile(
//		surface, 		// surface
//		NULL,			// destination palette	
//		NULL,			// destination rectangle 
//		FilePath,
//		NULL,			// source rectangle
//		D3DX_DEFAULT, 	// filter image
//		D3DCOLOR_XRGB(0, 0, 0),				// transparency (0 = none)
//		NULL);			// reserved
//
//	if (result != D3D_OK)
//	{
//		MessageBox(NULL, (LPCWSTR)"D3DXLoadSurfaceFromFile() failed", (LPCWSTR)"Error", NULL);
//		return NULL;
//	}
//
//	return surface;
//}