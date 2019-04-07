#ifndef __INTRO_H__
#define __INTRO_H__

#include <d3d9.h>
#include <d3dx9.h>

class CIntro {
protected:
	bool flash;

	float cloud_x, cloud_y;

	int bat_index;

	bool is_finish;
	bool is_open;
	DWORD open_time;
	DWORD last_time;
public:
	LPDIRECT3DTEXTURE9 _Image;
	LPCSTR Source;

	CIntro::CIntro();

	void LoadResource();
	void Update(float Delta);
	void Render();

	bool isFinish(){ return is_finish; }

	~CIntro();
};

#endif