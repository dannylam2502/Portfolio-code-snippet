#pragma once
#include "Game.h"
#include "Menu.h"
#include "Intro.h"
#include "GameOver.h"

enum GAME_STATE { MENUING, INTROING, PLAYING, PAUSING, ENDING, GAMEOVER,GAMEEND };
class CContraGame : public CGame
{
public:

	CContraGame(HINSTANCE hInstance, LPCSTR name, int FrameRate);
	~CContraGame();

	GAME_STATE _Game_State;
	CMenu* Menu;
	CIntro* Intro;
	CGameOver* GameOver;
	DWORD _Last_Frame;
	bool _Game_End;
	bool _End_Stage;
	bool _Lose;

	void LoadResource(LPDIRECT3DDEVICE9 d3ddv);
	void Update(float delta);
	void RenderFrames();
	void ProcessInput();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void UpdateQuadTree();
	void UpdateViewPort();

	void GamePlayUpdate(float delta);
	void GamePlayRender();

	void EndStageUpdate(float delta);
	void EndStageRender();

	void ChangeStage();

};

