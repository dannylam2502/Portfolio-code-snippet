#include "ContraGame.h"
#include "Texture.h"
#include "Resources.h"
#include "Bill.h"
#include "PoolBullet.h"
#include "WorldMap.h"
#include "QuadTree.h"
#include "LifeNum.h"
//test
#include "Boss_Stage1.h"
#include "Gun.h"
#include "Pool_Enemy_Bullet.h"
#include "MainBoss_Stage3.h"
#include "Ship.h"

#include "ExploreAnimation.h"
#include "MapAnimation.h"

#include "GameEnd.h"


extern VIEWPORT G_ViewPort;
extern CSound* Intro;
extern vector<CGameObject * > G_listObj;
extern vector<CMapAnimation*> list_Animation;

vector<CGameObject *> list_Obj_Handle;
CTexture* GameTexture;
CBill* Bill;
CPoolBullet* PoolBullet;
CPool_Enemy_Bullet* Pool_Enemy_Bullet;
CWorldMap* WorldMap;
bool Up, Down, Left, Right;
STAGE stage;
CQuadTree *root_node;
RECT QT_Handle_RECT, Obj_Handle_RECT;
CLifeNum* Life;
int test = 0;
//test
CExploreAnimation* Animation;
CBoss_Stage1* Boss;
CMainBoss_Stage3* Boss_Stage3;
CShip* Ship;

CGameEnd* GameEnd;


bool Flag_Explode_Bridge = false;
CGameObject* BridgeTemp;

bool Flag_ChangeState = false;
bool Active_Ship = false;


CContraGame::CContraGame(HINSTANCE hInstance, LPCSTR name, int FrameRate) :
CGame(hInstance, name, FrameRate)
{
	_Game_State = MENUING;
	_Game_End = false;
	_End_Stage = false;
	stage = STAGE1;
}

void CContraGame::LoadResource(LPDIRECT3DDEVICE9 d3ddv)
{
	D3DXCreateSprite(G_Device, &G_SpriteHandler);
	Up = Down = Left = Right = false;
	GameTexture = new CTexture();
	Bill = new CBill();
	Menu = new CMenu();
	PoolBullet = new CPoolBullet();
	Pool_Enemy_Bullet = new CPool_Enemy_Bullet();
	WorldMap = new CWorldMap();
	InitSound();
	Boss = new CBoss_Stage1();
	Boss_Stage3 = new CMainBoss_Stage3();
	Ship = new CShip(OBJ_BOSS_STAGE3_SHIP, 5100, 100, 62, 66);
	Animation = new CExploreAnimation();
	Life = new CLifeNum();

	GameEnd = new CGameEnd();
}
void CContraGame::Update(float delta)
{
	if (!Flag_ChangeState)
	{
		if (_Game_State == MENUING && Menu->isStarted())
		{
			delete Menu;
			_Game_State = INTROING;
			Intro = new CIntro();
		}

		if (_Game_State == INTROING && Intro->isFinish())
		{
			delete Intro;
			_Game_State = PLAYING;
			_End_Stage = false;
		}

		if (_Game_State == PLAYING && _Lose)
		{
			GameOver = new CGameOver();
			_Game_State = GAMEOVER;
			PlaySound(Game_Over);
		}
		switch (_Game_State)
		{
		case MENUING:
			Menu->Update(delta);
			break;
		case INTROING:
			Intro->Update(delta);
			break;
		case GAMEOVER:
			StopSound(BG_Map1);
			StopSound(BG_Map2);
			StopSound(BG_Map3);
			StopSound(GoBoss);
			PlaySound(Game_Over, 10);
			GameOver->Update(delta);
			break;
		case GAMEEND:
			StopSound(GoBoss);
			StopSound(BG_Map3);
			PlaySound(WinGame,10);
			GameEnd->Update(delta);
			break;
		case PLAYING:
			StopSound(Game_Over);
			GamePlayUpdate(delta);
			switch (stage)
			{
			case STAGE1:
				if (Bill->_X < 3020)
					PlaySound(BG_Map1, 10);
				else
				{
					StopSound(BG_Map1);
					PlaySound(GoBoss, 1);
				}
				StopSound(BG_Map2);
				StopSound(BG_Map3);
				break;
			case STAGE2:
				StopSound(GoBoss);
				if (Bill->_Y < 1900)
					PlaySound(BG_Map2, 10);
				else
				{
					StopSound(BG_Map2);
					PlaySound(GoBoss,10);
				}
				StopSound(BG_Map1);
				StopSound(BG_Map3);
				break;
			case STAGE3:
				StopSound(GoBoss);
				if (Bill->_X < 4800)
					PlaySound(BG_Map3, 10);
				else
				{
					PlaySound(GoBoss, 10);
					StopSound(BG_Map3);
				}
				StopSound(BG_Map1);
				StopSound(BG_Map2);
				break;
			}
			break;
		}
	}
}
void CContraGame::RenderFrames()
{
	if (!Flag_ChangeState)
	{
		G_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		switch (_Game_State)
		{
		case MENUING:
			Menu->Render();
			break;
		case INTROING:
			Intro->Render();
			break;
		case GAMEOVER:
			GameOver->Render();
			break;
		case GAMEEND:
			GameEnd->Render();
			break;
		case PLAYING:
			GamePlayRender();
			break;
		default:
			break;
		}

		G_SpriteHandler->End();
	}
}

void CContraGame::GamePlayUpdate(float delta)
{
	UpdateViewPort();
	UpdateQuadTree();
	Life->Update(delta);
	PoolBullet->Update(delta);
	//test
	Pool_Enemy_Bullet->Update(delta);
	/*Right_Hand_Boss->Update(delta);
	Left_Hand_Boss->Update(delta);
	Eye_Boss->Update(delta);*/
	//Ship->Update(delta);

	for each (CMapAnimation *obj in list_Animation)
	{
		obj->Update(delta);
	}

#pragma region ProcessBridge
	for each (CGameObject *obj in list_Obj_Handle)
	{
		if (obj->_Type == OBJ_BRIDGE && obj->_Live_State == DESTROYED)
		{
			BridgeTemp = obj;
			Flag_Explode_Bridge = true;
		}

		if (Flag_Explode_Bridge && obj->_Type == OBJ_BRIDGE && obj->_Live_State == ALIVE)
		{
			if (obj->_X - BridgeTemp->_X <= 10 + 4 * obj->_Width)
			{
				if (obj->_X - BridgeTemp->_X <= 10 + obj->_Width)
					obj->_Live_State = DYING;
			}
			else
				Flag_Explode_Bridge = false;
		}

		obj->Update(delta);
	}
#pragma endregion
	if (!_End_Stage)
	{
		Bill->Update(delta);
		if (stage == STAGE1)
		{
			Boss->Update(delta);
			if (Boss->_Live_State == DESTROYED)
			{
				StopSound(GoBoss);
				PlaySound(Pass_Boss);
				_End_Stage = true;
				ChangeStage();
			}
		}
		if (stage == STAGE2)
		{
			Boss_Stage3->Update(delta);
			if (Boss_Stage3->_Mouth->_Live_State == DESTROYED)
			{
				StopSound(GoBoss);
				PlaySound(Pass_Boss);
				_End_Stage = true;
				ChangeStage();
			}
		}
		if (stage == STAGE3)
		{
			if (Bill->_X >= 5104 - G_ViewPort._Width)
				Active_Ship = true;
			if (Active_Ship)
				Ship->Update(delta);
			if (Ship->_Live_State == DESTROYED)
			{
				
				//PlaySound(Pass_Boss);
				_End_Stage = true;
				ChangeStage();
			}
		}
	}
	else
	{
		Intro = new CIntro();
		_Game_State = INTROING;
		//ChangeStage();
		/*Bill->_Vx = 0.07f;
		Bill->SetStateHead(MOVE);
		Bill->SetStateLeg(MOVE);
		EndStageUpdate(delta);*/
	}
}

void CContraGame::UpdateQuadTree()
{
	if (stage != STAGE2)
		QT_Handle_RECT = Obj_Handle_RECT = { G_ViewPort._X, G_ViewPort._Y, G_ViewPort._X + G_ViewPort._Width, G_ViewPort._Y + G_ViewPort._Height };
	else
	{
		QT_Handle_RECT = { G_ViewPort._X, STAGE2_MAP_HEIGHT - G_ViewPort._Y - G_ViewPort._Height, G_ViewPort._X + G_ViewPort._Width, STAGE2_MAP_HEIGHT - G_ViewPort._Y };
		Obj_Handle_RECT = { G_ViewPort._X, G_ViewPort._Y, G_ViewPort._X + G_ViewPort._Width, G_ViewPort._Y + G_ViewPort._Height };
	}
	list_Obj_Handle = root_node->Object_Handle(QT_Handle_RECT, Obj_Handle_RECT);

	//out put list size
	char* output = new char[3];
	_itoa_s(list_Obj_Handle.size(), output, 3, 10);
	if (test != list_Obj_Handle.size())
	{
		test = list_Obj_Handle.size();
		//output[2] = '\n';
		OutputDebugStringA(output);
		OutputDebugStringA("\n");
	}

}

void CContraGame::GamePlayRender()
{
	WorldMap->Render();
	Life->Render();
	for each (CGameObject *obj in list_Obj_Handle)
	{
		obj->Render();
	}
	if (stage == STAGE1)
	{
		Boss->Render();
		if (Boss->_Live_State == DESTROYED)
		{
			Animation->DrawAnimation(3250, 50);
			Animation->DrawAnimation(3300, 50);
			Animation->DrawAnimation(3350, 50);
			Animation->DrawAnimation(3400, 50);
			Animation->DrawAnimation(3450, 50);
			Animation->DrawAnimation(3500, 50);

			//Bill->landing_obj = NULL;
			/*stage = STAGE2;
			WorldMap->LoadResource();*/
			//ChangeStage();
			PlaySound(Destroy_Boss);
		}
	}
	if (stage == STAGE2)
	{
		Boss_Stage3->Render();
	}
	if (stage == STAGE3 && Active_Ship)
	{
		Ship->Render();
	}
	Pool_Enemy_Bullet->Render();
	PoolBullet->Render();
	Bill->Render();
	if (_End_Stage)
	{
		EndStageRender();
	}

	if (Bill->_Life_Num <= 0)
	{
		_Lose = true;
	}
}

void CContraGame::UpdateViewPort()
{
	if (stage == STAGE1)
	{
		if (Bill->_X_Last > STAGE1_MAP_WIDTH - G_ViewPort._Width)
		{
			if (G_ViewPort._X < STAGE1_MAP_WIDTH - G_ViewPort._Width)
			{
				G_ViewPort._X++;
			}
		}
		else
		{
			if (Bill->_X_Last <= Bill->_X)
			{
				G_ViewPort._X = Bill->_X + Bill->_Width - G_ViewPort._Width / 2;
			}
		}
	}
	if (stage == STAGE2)
	{
		if (Bill->_Y_Last > STAGE2_MAP_HEIGHT - G_ViewPort._Height + Bill->_Height)
		{
			if (G_ViewPort._Y < STAGE2_MAP_HEIGHT - G_ViewPort._Height)
			{
				G_ViewPort._Y++;
			}
		}
		else
		{
			if (Bill->_Y_Last <= Bill->_Y && Bill->_Y < STAGE2_MAP_HEIGHT - G_ViewPort._Height)
			{
				G_ViewPort._Y = Bill->_Y - G_ViewPort._Height / 2;
			}
		}
	}

	if (stage == STAGE3)
	{
		if (Bill->_X_Last > STAGE3_MAP_WIDTH - G_ViewPort._Width)
		{
			if (G_ViewPort._X < STAGE3_MAP_WIDTH - G_ViewPort._Width)
			{
				G_ViewPort._X++;
			}
		}
		else
		{
			if (Bill->_X_Last <= Bill->_X)
			{
				G_ViewPort._X = Bill->_X + Bill->_Width - G_ViewPort._Width / 2;
			}
		}
	}

	if (G_ViewPort._X < 0)	G_ViewPort._X = 0;
	if (G_ViewPort._Y < 0)	G_ViewPort._Y = 0;
}

void CContraGame::ProcessInput()
{
	Up = Down = Left = Right = false;
	if (IsKeyDown(DIK_UP)) Up = true;
	if (IsKeyDown(DIK_DOWN)) Down = true;
	if (IsKeyDown(DIK_LEFT)) Left = true;
	if (IsKeyDown(DIK_RIGHT)) Right = true;

	Bill->_direction = Bill->GetDirection(Up, Down, Left, Right);
	Bill->TriggerControl(Up, Down, Left, Right);

	if (IsKeyDown(DIK_DOWN))
	{
		Bill->_Y -= 0.00001f;
	}
	/*if (IsKeyDown(DIK_C))
	{
	DWORD now = GetTickCount();
	if (now - _Last_Frame > 1000 / 10)
	{
	_Last_Frame = now;
	if (PoolBullet->_Bullet_Type == M && PoolBullet->CheckNum() < 6)
	{
	OnKeyDown(DIK_C);
	}
	}
	}*/
}

void CContraGame::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_RETURN:
		if (_Game_State == MENUING)
		{
			if (Menu->isMoving()) Menu->setX(0);
			else Menu->Start();
		}
		if (_Game_State == GAMEOVER)
		{
			Intro = new CIntro();
			_Game_State = INTROING;
			_Lose = false;
			Bill->_Life_Num = 4;
			if (stage == STAGE1)
			{
				stage = STAGE3;
				ChangeStage();
				return;
			}
			if (stage == STAGE2)
			{
				stage = STAGE1;
				ChangeStage();
				return;
			}
			if (stage == STAGE3)
			{
				stage = STAGE2;
				ChangeStage();
				return;
			}
		}
		break;
	case DIK_X:
		Bill->Jump();
		break;
	case DIK_C:
		Bill->Shoot();
		//PoolBullet->Load();
		break;
	case DIK_2:
		break;
	case DIK_3:
		Bill->_Is_Control = true;
		break;
	case DIK_Q:
		PoolBullet->ChangeType(S);
		break;
	case DIK_W:
		Bill->_Life_Num = 30;
		break;
	case DIK_5:
		PoolBullet->ChangeType(L);
		break;
	case DIK_B:
		if (stage == STAGE1)
			Bill->_X = 2400;
		if (stage == STAGE3)
			Bill->_X = 4000;
		if (stage == STAGE2)
			Bill->_Y = 1900;		
		break;
	case DIK_N:
		Bill->_X = 2400;
		break;
	case DIK_SPACE:
		ChangeStage();
		break;
	default:
		break;
	}
}

void CContraGame::EndStageUpdate(float delta)
{
	//Bill->_Is_Control = false;
	//if (Bill->_X < G_ViewPort._Y + G_ViewPort._Width)
	//{
	//	Bill->Update(delta);
	//}
	//if (Bill->_X > G_ViewPort._Y + G_ViewPort._Width/3 && Bill->_X < G_ViewPort._Y + G_ViewPort._Width)
	//{
	//	Bill->_Vy = 0.2f;
	//}
}

void CContraGame::EndStageRender()
{
	for (int i = 0; i < 6; i++)
	{
		Animation->DrawAnimation(3200 + 50 * i,50);
	}
}

void CContraGame::ChangeStage()
{
	Active_Ship = false;
	Bill->landing_obj = NULL;
	Flag_ChangeState = true;
	list_Obj_Handle.clear();
	for each (CGameObject* obj in G_listObj)
	{
		delete obj;
	}
	G_listObj.clear();
	for each (CMapAnimation* obj in list_Animation)
	{
		delete obj;
	}
	list_Animation.clear();
	if (stage == STAGE1)
	{
		/*Bill->_Y = 70;
		Bill->_X = 50;*/
		Bill->_Y_Last = 70;
		Bill->_X_Last = 30;
		stage = STAGE2;
		goto L;
	}
	if (stage == STAGE2)
	{
		Bill->_Y = 100;
		Bill->_X = 50;
		Bill->_X_Last = Bill->_Y_Last = 0;
		stage = STAGE3;
		goto L;
	}
	if (stage == STAGE3 && GameEnd->is_Finish == false)
	{
		_Game_State = GAMEEND;
		goto L1;
	};

	/*if (GameEnd->is_Finish == true)
	{
		Menu->Reset();
		_Game_State = MENUING;
		GameEnd->y = 240;
		GameEnd->is_Finish = false;
		stage = STAGE1;
	}*/
L:	WorldMap->LoadResource();
L1:	G_ViewPort._X = 0;
	G_ViewPort._Y = 0;
	Bill->_Vx = Bill->_Vy = 0;
	/*Bill->ProcessReborn();
	Bill->_Life_Num++;*/
	Flag_ChangeState = false;
}

void CContraGame::OnKeyUp(int KeyCode)
{

}

CContraGame::~CContraGame()
{

}