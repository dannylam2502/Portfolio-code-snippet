#include "QuadTree.h"


bool CollidedWith(RECT A, RECT B)
{
	if (A.left > B.right ||
		A.right < B.left ||
		A.top > B.bottom ||
		A.bottom < B.top)
		return false;
	return true;
}


CQuadTree::CQuadTree(RECT _rec)
{
	rec = _rec;
	LeftTop = NULL;
	RightTop = NULL;
	LeftBot = NULL;
	RightBot = NULL;
}


CQuadTree::~CQuadTree()
{
}

void CQuadTree::CreateSubNode()
{
	RECT rect = { 0, 0, 0, 0 };
	LeftTop = new CQuadTree(rect);
	RightTop = new CQuadTree(rect);
	LeftBot = new CQuadTree(rect);
	RightBot = new CQuadTree(rect);
}

void CQuadTree::Load(ifstream &file, vector<CGameObject *> listObj)
{
	int pos_x; file >> pos_x;
	int pos_y; file >> pos_y;
	int width; file >> width;
	int height; file >> height;
	int Num_Node; file >> Num_Node;
	int object_num; file >> object_num;

	rec.left = pos_x;
	rec.top = pos_y;
	rec.right = pos_x + width;
	rec.bottom = pos_y + height;


	for (int i = 0; i < object_num; i++)
	{
		int k; file >> k;
		if (k < listObj.size())
		{
			list_Obj.push_back(listObj[k]);
		}
	}

	if (Num_Node == 4)
	{
		CreateSubNode();
		LeftTop->Load(file, listObj);
		RightTop->Load(file, listObj);
		LeftBot->Load(file, listObj);
		RightBot->Load(file, listObj);
	}
}

vector<CGameObject *> CQuadTree::Object_Handle(RECT QuadTree_Handle_rect, RECT Object_Handle_Rect)
{
	vector<CGameObject *> result;

	for each(CGameObject* obj in list_Obj)
	{
		RECT obj_rect = { obj->_X, obj->_Y, obj->_X + obj->_Width, obj->_Y + obj->_Height };
		if (CollidedWith(obj_rect, Object_Handle_Rect))
			result.push_back(obj);
	}

	if (LeftTop != NULL)
	{
		if (CollidedWith(LeftTop->rec, QuadTree_Handle_rect))
		{
			vector<CGameObject *> tmp = LeftTop->Object_Handle(QuadTree_Handle_rect, Object_Handle_Rect);
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}

	if (RightTop != NULL)
	{
		if (CollidedWith(RightTop->rec, QuadTree_Handle_rect))
		{
			vector<CGameObject *> tmp = RightTop->Object_Handle(QuadTree_Handle_rect, Object_Handle_Rect);
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}

	if (LeftBot != NULL)
	{
		if (CollidedWith(LeftBot->rec, QuadTree_Handle_rect))
		{
			vector<CGameObject *> tmp = LeftBot->Object_Handle(QuadTree_Handle_rect, Object_Handle_Rect);
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}

	if (RightBot != NULL)
	{
		if (CollidedWith(RightBot->rec, QuadTree_Handle_rect))
		{
			vector<CGameObject *> tmp = RightBot->Object_Handle(QuadTree_Handle_rect, Object_Handle_Rect);
			result.insert(result.end(), tmp.begin(), tmp.end());
		}
	}

	return result;
}
