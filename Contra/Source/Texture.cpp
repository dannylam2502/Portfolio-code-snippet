#include "Texture.h"
#include "Global.h"
#include "Resources.h"

extern LPDIRECT3DDEVICE9 G_Device;
extern LPD3DXSPRITE G_SpriteHandler;
extern VIEWPORT G_ViewPort;
CTexture::CTexture()
{
	_BILL = CreateTexture(BILL_IMAGE_PATH);
	_MISC = CreateTexture(MISC_IMAGE_PATH);
	_BOSS = CreateTexture(BOSS_STAGE1_IMAGE_PATH);
	_ENEMY = CreateTexture(ENEMY_IMAGE_PATH);
	_ENEMY_TWO = CreateTexture(ENEMY_TWO_IMAGE_PATH);
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture(int Type)
{
	switch (Type)
	{
	case OBJ_BILL:
		return _BILL;
	case OBJ_PLAYER_BULLET:
	case OBJ_ENEMY_BULLET:
	case OBJ_FLYING_CAPSULE:
	case OBJ_ITEM:
	case OBJ_PLATFORM:
	case OBJ_BOM_BULLET:
	case OBJ_LARGE_BULLET:
	case OBJ_LIFE_NUM:
		return _MISC;
	case OBJ_BOSS_STAGE1:
	case OBJ_BOSS_STAGE1_GUN:
	case OBJ_BOSS_STAGE2_HAND:
	case OBJ_BOSS_STAGE2_STONE:
	case OBJ_BOSS_STAGE2_EYE:
	case OBJ_BOSS_STAGE2_MOUTH:
	case OBJ_BOSS_STAGE2_SKIN:
	case OBJ_BOSS_STAGE3_SHIP:
	case OBJ_BOSS_STAGE3_SMALLSHIP:
		return _BOSS;
	case OBJ_TANK:
	case OBJ_ARTILLERY:
	case OBJ_PLATFORM_ROCK:
	case OBJ_FIRE:
	case OBJ_ROCK:
	case OBJ_DIVER:
	case OBJ_FALCON:
	case OBJ_EXPLORE_ANIMATION:
	case OBJ_BRIDGE:
		return _ENEMY_TWO;
	case OBJ_SOLDIER:
	case OBJ_RIFLEMAN:
	case OBJ_CANNON1:
	case OBJ_CANNON2:
	case OBJ_HIDE_RIFLEMAN:
		return _ENEMY;
	}
	return _ENEMY;
}

void CTexture::Render(RECT srect, CGameObject* obj, VIEWPORT G_ViewPort)
{
	D3DXVECTOR3 rect_pos(obj->_X, obj->_Y, 0.0f);
	//rect_pos.x -= (obj->_Width - (obj->GetRect().right - obj->GetRect().left)) / 2;

	D3DXMATRIX mat;
	//D3DXVECTOR2 center((float)(obj->_X + (obj->GetRect().right - obj->GetRect().left) / 2), (float)(obj->_Y + (obj->GetRect().bottom - obj->GetRect().top) / 2));
	//D3DXVECTOR2 center((obj->_X + obj->_Width / 2), (obj->_Y + obj->_Height / 2));
	D3DXVECTOR2 center((float)(obj->_X + (srect.right - srect.left) / 2), (float)(obj->_Y + (srect.bottom - srect.top) / 2));

	D3DXVECTOR2 scale = D3DXVECTOR2((float)obj->GetFacing(), -1.0f);
	D3DXMatrixTransformation2D(&mat, &center, 0.0f, &scale, NULL, 0.0f, NULL);

	D3DXMATRIX matTmp;
	D3DXMatrixTranslation(&matTmp, -(float)G_ViewPort._X, (float)(-(G_ViewPort._Y + G_ViewPort._Height)), 0.0f);
	mat *= matTmp;
	D3DXMatrixScaling(&matTmp, 1.0f, -1.0f, 1.0f);
	mat *= matTmp;
	//D3DXVECTOR3 center_p((float)(srect.right - srect.left) / 2, (float)(srect.bottom - srect.top) / 2, 0);
	//G_SpriteHandler->SetTransform(&mat);
	obj->GetSpriteHandler()->SetTransform(&mat);
	obj->GetSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);
	obj->GetSpriteHandler()->Draw(
		this->GetTexture(obj->_Type),
		&srect,
		NULL,
		//&center_p,
		&rect_pos,
		obj->_Render_Color
		);
	obj->GetSpriteHandler()->End();
}

void CTexture::Render(RECT srect, CGameObject* obj, VIEWPORT G_ViewPort, float X, float Y)
{
	D3DXVECTOR3 rect_pos(X, Y, 0.0f);
	//rect_pos.x -= (obj->_Width - (obj->GetRect().right - obj->GetRect().left)) / 2;

	D3DXMATRIX mat;
	//D3DXVECTOR2 center((float)(obj->_X + (obj->GetRect().right - obj->GetRect().left) / 2), (float)(obj->_Y + (obj->GetRect().bottom - obj->GetRect().top) / 2));
	//D3DXVECTOR2 center((obj->_X + obj->_Width / 2), (obj->_Y + obj->_Height / 2));
	D3DXVECTOR2 center((float)(X + (srect.right - srect.left) / 2), (float)(Y + (srect.bottom - srect.top) / 2));

	D3DXVECTOR2 scale = D3DXVECTOR2((float)obj->GetFacing(), -1.0f);
	D3DXMatrixTransformation2D(&mat, &center, 0.0f, &scale, NULL, 0.0f, NULL);

	D3DXMATRIX matTmp;
	D3DXMatrixTranslation(&matTmp, -(float)G_ViewPort._X, (float)(-(G_ViewPort._Y + G_ViewPort._Height)), 0.0f);
	mat *= matTmp;
	D3DXMatrixScaling(&matTmp, 1.0f, -1.0f, 1.0f);
	mat *= matTmp;
	//D3DXVECTOR3 center_p((float)(srect.right - srect.left) / 2, (float)(srect.bottom - srect.top) / 2, 0);
	//G_SpriteHandler->SetTransform(&mat);
	obj->GetSpriteHandler()->SetTransform(&mat);
	obj->GetSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);
	obj->GetSpriteHandler()->Draw(
		this->GetTexture(obj->_Type),
		&srect,
		NULL,
		//&center_p,
		&rect_pos,
		obj->_Render_Color
		);
	obj->GetSpriteHandler()->End();
}

LPDIRECT3DTEXTURE9 CreateTexture(LPSTR FilePath)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	LPDIRECT3DTEXTURE9 Texture;

	result = D3DXGetImageInfoFromFile(FilePath, &info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Failed to get information from image file!", "Error", NULL);
		return NULL;
	}

	result = D3DXCreateTextureFromFileEx(
		G_Device,
		FilePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(155, 155, 155),
		&info,
		NULL,
		&Texture);

	if (result != D3D_OK)
	{
		MessageBox(NULL, "Failed to create texture from file!", "Error", NULL);
		return NULL;
	}
	return Texture;
}

CTexture::~CTexture()
{
	_BILL->Release();
}
void CTexture::Render(RECT rect, float X, float Y, LPDIRECT3DTEXTURE9 image, LPD3DXSPRITE _spriteHandler)
{
	D3DXVECTOR3 position(X, Y, 0);

	//D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
	//D3DXVECTOR3 center((float)_Width / 2, (float)_Height / 2, 0);
	/*D3DXMATRIX mat;
	D3DXMatrixScaling(&mat, 0.5f, 0.5f, 0.5f);

	_spriteHandler->SetTransform(&mat);*/

	_spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	_spriteHandler->Draw(
		image,
		&rect,
		//&center,
		NULL,
		&position,
		D3DCOLOR_XRGB(255, 255, 255)
		);
	_spriteHandler->End();
}
