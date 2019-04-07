#ifndef __MENU_H__
#define __MENU_H__

#include <d3d9.h>
#include <d3dx9.h>


class CMenu {
protected:
	DWORD start_time;
	DWORD last_time;
	bool flash;
	bool is_started;
	bool start;
	bool is_moving;

	float _X, _Y;
	int _Width, _Height;
public:
	LPDIRECT3DTEXTURE9 _Image;
	//LPD3DXSPRITE _SpriteHandler;
	//LPDIRECT3DSURFACE9 _bf;

	CMenu::CMenu();

	void LoadResource();
	void Update(float Delta);
	void Render();

	void Start();
	bool isStarted(){ return is_started; }
	bool isMoving(){ return is_moving; }
	void setX(float x){ this->_X = x; }
	void Reset();
	~CMenu();
};

#endif