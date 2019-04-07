#include "Eye_Boss_Stage2.h"
#include "Texture.h"

#define EYE_BOSS_ANIMATE_RATE	7
extern CTexture* GameTexture;
CEye_Boss_Stage2::CEye_Boss_Stage2() :CEnemy(OBJ_BOSS_STAGE2_EYE, 104, 2126, 10, 10)
{
	_Facing = FACING_RIGHT;
}
CEye_Boss_Stage2::CEye_Boss_Stage2(int Type, float X, float Y, int Width, int Height) :
CEnemy(Type, X, Y, Width, Height)
{
	_Facing = FACING_RIGHT;
}
void CEye_Boss_Stage2::Update(float delta)
{
	DWORD now = GetTickCount();
	if (now - _Last_Time > 1000 / EYE_BOSS_ANIMATE_RATE)
	{
		_Last_Time = now;
		_AnimateIndex++;
		if (_AnimateIndex >= 3)
		{
			_AnimateIndex = 0;
		}
	}
}

void CEye_Boss_Stage2::Render()
{
	GameTexture->Render(GetRect(), this, G_ViewPort);
}