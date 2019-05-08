#include "ContraGame.h"
#include <Windows.h>

int WinMain(HINSTANCE hInstance,HINSTANCE PrevInstance,LPSTR lpCmdLine, int CmdShow)
{
	CContraGame *test = new CContraGame(hInstance,"Contra",30);
	
	test->Init();
	test->Run();

	return 0;
}