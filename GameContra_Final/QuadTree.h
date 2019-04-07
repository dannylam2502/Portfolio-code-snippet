#pragma once
#include "GameObject.h"
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class CQuadTree
{
	RECT rec;
	CQuadTree *LeftTop;
	CQuadTree *RightTop;
	CQuadTree *LeftBot;
	CQuadTree *RightBot;

	vector<CGameObject *> list_Obj;
public:
	CQuadTree(RECT rec);
	~CQuadTree();

	void CreateSubNode();
	void Load(ifstream &file, vector<CGameObject *> listobj);

	vector<CGameObject *> Object_Handle(RECT QuadTree_Handle_rect, RECT Object_Handle_Rect);
};

