#include "Platform.h"
#include "Texture.h"

extern CTexture* GameTexture;

CPlatform::CPlatform(int Type, int pos_x, int pos_y, int width, int height) : CGameObject(Type, pos_x, pos_y, width, height)
{
}

CPlatform::~CPlatform()
{
}
void CPlatform::Render()
{
	//GameTexture->Render(GetRect(),this,G_ViewPort);
}