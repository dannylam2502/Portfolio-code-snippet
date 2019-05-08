#pragma once

#include <d3dx9.h>
#include <d3d.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "GameObject.h"

#define CELLSIZE 16		//kích thước các ô đơn vị trong map
#define OFFSET 16

enum STAGE { STAGE1, STAGE2, STAGE3 };

class CWorldMap
{
	LPDIRECT3DTEXTURE9 _Background;		//map texture
	LPDIRECT3DTEXTURE9 _Animation;
	LPD3DXSPRITE _SpriteHandler;
public:
	CWorldMap();
	~CWorldMap();

	void Render();
	void LoadResource();									//load map resource
	void loadMapFromFile(string File_Path);					//load file text map
	void LoadQuadTreeFromFile(string QuadTree_File_Path);	//load file quad tree
	void SplitString(vector<int> &kq, string temp,char ignorechar,int n);			//split 1 line in file text map and push them into a vetor variable

	void loadObjectsFromFile(string Obj_path);				//load file text objects

	void CreateAnimation();

	RECT GetRect(int column,int row)
	{
		RECT srect;
		srect = { (column - 1)*CELLSIZE, (row - 1)*CELLSIZE, column *CELLSIZE, row * CELLSIZE };
		//srect = { (column - 1)*CELLSIZE + column*2, (row - 1)*CELLSIZE + row*2, column *CELLSIZE + column*2, row * CELLSIZE +row*2 };
		//srect = { 0, 0, 200, 200 };
		return srect;
	}
};
