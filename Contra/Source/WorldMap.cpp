#include "WorldMap.h"
#include "Texture.h"
#include "Resources.h"
#include "Platform.h"
#include "Cannon1.h"
#include "Cannon2.h"
#include "Soldier.h"
#include "RifleMan.h"
#include "Rock.h"
#include "Platform_Rock.h"
#include "FlyingCapsule.h"
#include "Water.h"
#include "Falcon.h"
#include "Fire.h"
#include "Diver.h"
#include "Tank.h"
#include "Artillery.h"
#include "Bridge.h"
#include "BomBay.h"
#include "Hide_Rifleman.h"

#include "QuadTree.h"

#include "MapAnimation.h"

vector<CGameObject * > G_listObj;			//list of objects
vector<vector<int>> Map_Matrix;				//map matrix 

vector<CMapAnimation*> list_Animation;		//list animation

extern CQuadTree *root_node;
extern STAGE stage;

#define ANIMATION_STAGE13		2
#define ANIMATION_STAGE2		3

CWorldMap::CWorldMap()
{
	LoadResource();
	D3DXCreateSprite(G_Device, &_SpriteHandler);
}


CWorldMap::~CWorldMap()
{
}

void CWorldMap::LoadResource()
{
	_Animation = CreateTexture(MAP_ANIMATION_PATH);
	if (stage == STAGE1)
	{
		_Background = CreateTexture(BACKGROUND_STAGE1_IMAGE_PATH);
		loadMapFromFile(STAGE1_MAP_PATH);
		loadObjectsFromFile(STAGE1_OBJ_PATH);
		LoadQuadTreeFromFile(STAGE1_QT_PATH);
	}

	if (stage == STAGE2)
	{
		_Background = CreateTexture(BACKGROUND_STAGE2_IMAGE_PATH);
		loadMapFromFile(STAGE2_MAP_PATH);
		loadObjectsFromFile(STAGE2_OBJ_PATH);
		LoadQuadTreeFromFile(STAGE2_QT_PATH);
	}

	if (stage == STAGE3)
	{
		_Background = CreateTexture(BACKGROUND_STAGE3_IMAGE_PATH);
		loadMapFromFile(STAGE3_MAP_PATH);
		loadObjectsFromFile(STAGE3_OBJ_PATH);
		LoadQuadTreeFromFile(STAGE3_QT_PATH);
	}
	CreateAnimation();
}

void CWorldMap::Render()
{
	if (stage != STAGE2)
	{
		/*int maxsize = (G_ViewPort._X / CELLSIZE);
		if (maxsize > Map_Matrix[0].size() - SCREEN_WIDTH / CELLSIZE / 2 - 2)
		maxsize = Map_Matrix[0].size() - SCREEN_WIDTH / CELLSIZE / 2 - 2;
		for (int i = maxsize; i < maxsize + SCREEN_WIDTH / CELLSIZE / 2 + 2; i++)
		{
		for (int j = 0; j < Map_Matrix.size(); j++)
		{
		CTexture::Render(this->GetRect(Map_Matrix[j][i] % 10 + 1, Map_Matrix[j][i] / 10 + 1), i*CELLSIZE - G_ViewPort._X, j*CELLSIZE + OFFSET, _Background, _SpriteHandler);
		}
		}*/

		int maxsize = (G_ViewPort._X / CELLSIZE);
		if (maxsize > Map_Matrix[0].size() - SCREEN_WIDTH / CELLSIZE / 2 - 2)
			maxsize = Map_Matrix[0].size() - SCREEN_WIDTH / CELLSIZE / 2 - 2;
		for (int i = maxsize; i < maxsize + SCREEN_WIDTH / CELLSIZE / 2 + 2; i++)
		{
			for (int j = 0; j < Map_Matrix.size(); j++)
			{
				if (stage == STAGE1)
				{
					switch (Map_Matrix[j][i])
					{
					case 0:
					case 1:
					case 4:
					case 5:
					case 6:
					case 7:
					case 14:
					case 21:
					case 28:
					case 40:
					case 45:
					case 46:
					case 47:
					case 48:
					case 53:
					case 67:
						for each (CMapAnimation	*obj in list_Animation)
						{
							if (obj->map_sequence == Map_Matrix[j][i])
							{
								obj->setPosition(i*CELLSIZE - G_ViewPort._X, j*CELLSIZE + OFFSET);
								obj->Render(_Animation, _SpriteHandler);
								break;
							}
						}
						break;
					default:
						CTexture::Render(this->GetRect(Map_Matrix[j][i] % 10 + 1, Map_Matrix[j][i] / 10 + 1), i*CELLSIZE - G_ViewPort._X, j*CELLSIZE + OFFSET, _Background, _SpriteHandler);
						break;
					}
				}

				if (stage == STAGE3)
				{
					switch (Map_Matrix[j][i])
					{
					case 79:
					case 80:
					case 82:
					case 83:
					case 84:
					case 85:
					case 86:
					case 87:
					case 88:
					case 89:
						for each (CMapAnimation	*obj in list_Animation)
						{
							if (obj->map_sequence == Map_Matrix[j][i])
							{
								obj->setPosition(i*CELLSIZE - G_ViewPort._X, j*CELLSIZE + OFFSET);
								obj->Render(_Animation, _SpriteHandler);
								break;
							}
						}
						break;
					default:
						CTexture::Render(this->GetRect(Map_Matrix[j][i] % 10 + 1, Map_Matrix[j][i] / 10 + 1), i*CELLSIZE - G_ViewPort._X, j*CELLSIZE + OFFSET, _Background, _SpriteHandler);
						break;
					}
				}
			}
		}
	}
	else
	{
		/*int maxsize = Map_Matrix.size() - 1 - G_ViewPort._Y / CELLSIZE - 17;
		if (maxsize < 0)
		maxsize = 0;
		for (int i = 0; i < Map_Matrix[0].size(); i++)
		{
		for (int j = maxsize; j <= maxsize + 17; j++)
		{
		int pos_render_y = G_ViewPort._Height + G_ViewPort._Y - (Map_Matrix.size() - 1 - j)*CELLSIZE - CELLSIZE;
		CTexture::Render(this->GetRect(Map_Matrix[j][i] % 10 + 1, Map_Matrix[j][i] / 10 + 1), i*CELLSIZE, pos_render_y, _Background, _SpriteHandler);
		}
		}*/

		int maxsize = Map_Matrix.size() - 1 - G_ViewPort._Y / CELLSIZE - 17;
		if (maxsize < 0)
			maxsize = 0;
		for (int i = 0; i < Map_Matrix[0].size(); i++)
		{
			for (int j = maxsize; j <= maxsize + 17; j++)
			{
				int pos_render_y = G_ViewPort._Height + G_ViewPort._Y - (Map_Matrix.size() - 1 - j)*CELLSIZE - CELLSIZE;

				switch (Map_Matrix[j][i])
				{
				case 7:
				case 11:
				case 12:
				case 14:
				case 26:
				case 27:
				case 29:
				case 32:
				case 33:
				case 40:
				case 47:
				case 48:
				case 50:
				case 51:
				case 52:
				case 58:
				case 60:
				case 61:
				case 74:
				case 75:
				case 125:
					for each (CMapAnimation	*obj in list_Animation)
					{
						if (obj->map_sequence == Map_Matrix[j][i])
						{
							obj->setPosition(i*CELLSIZE, pos_render_y);
							obj->Render(_Animation, _SpriteHandler);
							break;
						}
					}
					break;
				default:
					CTexture::Render(this->GetRect(Map_Matrix[j][i] % 10 + 1, Map_Matrix[j][i] / 10 + 1), i*CELLSIZE, pos_render_y, _Background, _SpriteHandler);
					break;
				}
			}
		}
	}

}

void CWorldMap::loadObjectsFromFile(string Obj_Path)
{
	G_listObj.clear();

	ifstream f(Obj_Path);
	string temp = "";
	if (f.is_open())
	{
		vector<int> row;
		string previousLine = "";
		getline(f, temp);
		CGameObject* obj;
		while (!f.eof())
		{
			string temp = "";
			getline(f, temp);
			if (temp != previousLine && temp != "")
			{

				previousLine = temp;
				SplitString(row, temp, '\t', 2);
				//if (stage == STAGE1)
				{
					//if (row.at(1) != OBJ_BOM)
					{
						if (row.at(1) == OBJ_HIDE_RIFLEMAN)
						{
							obj = new CHide_Rifleman(OBJ_HIDE_RIFLEMAN, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}
						if (row.at(1) == OBJ_BOM)
						{
							obj = new CBomBay(OBJ_BOM, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}
						if (row.at(1) == OBJ_WATER)
						{
							obj = new CWater(OBJ_WATER, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}
						if (row.at(1) == OBJ_PLATFORM)
						{
							obj = new CPlatform(OBJ_PLATFORM, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_CANNON2)
						{
							obj = new CCannon2(OBJ_CANNON2, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_CANNON1)
						{
							obj = new CCannon1(OBJ_CANNON1, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_RIFLEMAN)
						{
							obj = new CRifleman(OBJ_RIFLEMAN, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_SOLDIER)
						{
							obj = new CSoldier(OBJ_SOLDIER, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_FLYING_CAPSULE)
						{
							obj = new CFlyingCapsule(OBJ_FLYING_CAPSULE, row.at(2), row.at(3), row.at(4), row.at(5), row.at(6));
							G_listObj.push_back(obj);
						}
						if (row.at(1) == OBJ_FALCON)
						{
							obj = new CFalcon(OBJ_FALCON, row.at(2), row.at(3), row.at(4), row.at(5), row.at(6));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_BRIDGE)
						{
							obj = new CBridge(OBJ_BRIDGE, row.at(2), row.at(3) + 16, row.at(4), row.at(5), row.at(7));
							obj->_Facing = FACING_RIGHT;
							G_listObj.push_back(obj);
						}

						//stage 2
						if (row.at(1) == OBJ_FIRE)
						{
							obj = new CFire(OBJ_FIRE, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}
						if (row.at(1) == OBJ_PLATFORM_ROCK)
						{
							obj = new CPlatform_Rock(OBJ_PLATFORM_ROCK, row.at(2), row.at(3), row.at(4), row.at(5), row.at(7));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_ROCK)
						{
							obj = new CRock(OBJ_ROCK, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_DIVER)
						{
							obj = new CDiver(OBJ_DIVER, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						//stage 3
						if (row.at(1) == OBJ_TANK)
						{
							obj = new CTank(OBJ_TANK, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}

						if (row.at(1) == OBJ_ARTILLERY)
						{
							obj = new CArtillery(OBJ_ARTILLERY, row.at(2), row.at(3), row.at(4), row.at(5));
							G_listObj.push_back(obj);
						}
					}
					/*else
					{
						obj = new CGameObject(OBJ_CANNON1, row.at(2), row.at(3), row.at(4), row.at(5));
						G_listObj.push_back(obj);
					}*/
				}

				/*if (stage == STAGE2)
				{

				}*/

				row.clear();
			}
		}
		row.end();
	}
	f.close();
}

void CWorldMap::LoadQuadTreeFromFile(string QuadTree_File_Path)
{
	RECT rec = { 0, 0, 0, 0 };
	root_node = new CQuadTree(rec);
	ifstream file(QuadTree_File_Path);

	string temp = "";
	getline(file, temp);

	if (file.is_open())
		root_node->Load(file, G_listObj);
	file.close();
}

void CWorldMap::loadMapFromFile(string File_Path)
{
	Map_Matrix.clear();

	ifstream f(File_Path);
	string temp = "";
	int j = 0;
	if (f.is_open())
	{
		vector<int> row;
		string previousLine = "";
		while (!f.eof())
		{
			string temp = "";
			getline(f, temp);
			if (temp != previousLine)
			{
				previousLine = temp;
				SplitString(row, temp, ' ', 1);
				j++;
				Map_Matrix.push_back(row);
				row.clear();
			}
		}
		row.end();
		Map_Matrix.pop_back();
	}
	f.close();
}


void CWorldMap::SplitString(vector<int> &kq, string temp, char ignorechar, int n)
{
	int count = 0;
	string STRING = "";
	for (int i = 0; i < temp.length(); i++)
	{
		if (temp.at(i) == ignorechar)
			count++;
		else
		{
			if (count < n)
				STRING += temp.at(i);
		}
		if (count == n)
		{
			int a = 0;
			istringstream(STRING) >> a;
			kq.push_back(a);
			count = 0;
			STRING = "";
		}
	}
}

void CWorldMap::CreateAnimation()
{
	list_Animation.clear();
	for (int i = 0; i < Map_Matrix.size(); i++)
	{
		for (int j = 0; j < Map_Matrix[0].size(); j++)
		{
			int count = 0;
			for each (CMapAnimation *obj in list_Animation)
			{
				if (obj->map_sequence == Map_Matrix[i][j])
					goto  L;
			}

			if (stage == STAGE1)
			{
				switch (Map_Matrix[i][j])
				{
				case 0:
				case 1:
				case 4:
				case 5:
				case 6:
				case 7:
				case 14:
				case 21:
				case 28:
				case 40:
				case 45:
				case 46:
				case 47:
				case 48:
				case 53:
				case 67:
					CMapAnimation *obj = new CMapAnimation(OBJ_MAP_ANIMATION, 0, 0, 16, 16, Map_Matrix[i][j], ANIMATION_STAGE13, 1);
					list_Animation.push_back(obj);
					break;
				}
			}

			if (stage == STAGE2)
			{
				switch (Map_Matrix[i][j])
				{
				case 7:
				case 11:
				case 12:
				case 14:
				case 26:
				case 27:
				case 29:
				case 32:
				case 33:
				case 40:
				case 47:
				case 48:
				case 50:
				case 51:
				case 52:
				case 58:
				case 60:
				case 61:
				case 74:
				case 75:
				case 125:
					CMapAnimation *obj = new CMapAnimation(OBJ_MAP_ANIMATION, 0, 0, 16, 16, Map_Matrix[i][j], ANIMATION_STAGE2, 2);
					list_Animation.push_back(obj);
					break;
				}
			}

			if (stage == STAGE3)
			{
				switch (Map_Matrix[i][j])
				{
				case 79:
				case 80:
				case 82:
				case 83:
				case 84:
				case 85:
				case 86:
				case 87:
				case 88:
				case 89:
					CMapAnimation *obj = new CMapAnimation(OBJ_MAP_ANIMATION, 0, 0, 16, 16, Map_Matrix[i][j], ANIMATION_STAGE13, 3);
					list_Animation.push_back(obj);
					break;
				}
			}

		L:;
		}

	}
}
