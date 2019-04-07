#include <d3dx9.h>
#include "Resources.h"

CSound* Intro;
CSound* BG_Map1;
CSound* BG_Map2;
CSound* BG_Map3;
CSound* Base_Bullet;
CSound* SBullet;
CSound* LBullet;
CSound* FBullet;
CSound* MBullet;
CSound* Bill_Jump;
CSound* Bill_Dead;
CSound* Eat_Item;
CSound* Attack_Cannon;
CSound* Enemy_Attack;
CSound* Boom;
CSound* Destroy_Bridge;
CSound* Destroy_Enemy;
CSound* GoBoss;
CSound* Boss_1;
CSound* Destroy_Boss;
CSound* Pass_Boss;
CSound* Game_Over;
CSound* WinGame;

void InitSound()
{
	Intro = LoadSound("Resource\\Music\\Intro.wav");
	BG_Map1 = LoadSound("Resource\\Music\\BG_Map1.wav");
	BG_Map2 = LoadSound("Resource\\Music\\BG_Map2.wav");
	BG_Map3 = LoadSound("Resource\\Music\\BG_Map3.wav");
	Base_Bullet = LoadSound("Resource\\Music\\Base_Bullet.wav");
	SBullet = LoadSound("Resource\\Music\\SBullet.wav");
	LBullet = LoadSound("Resource\\Music\\LBullet.wav");
	FBullet = LoadSound("Resource\\Music\\FBullet.wav");
	MBullet = LoadSound("Resource\\Music\\MBullet.wav");
	Bill_Jump = LoadSound("Resource\\Music\\Jump.wav");
	Bill_Dead = LoadSound("Resource\\Music\\Dead.wav");
	Eat_Item = LoadSound("Resource\\Music\\Eat_Item.wav");
	Attack_Cannon = LoadSound("Resource\\Music\\Attack_Cannon.wav");
	Enemy_Attack = LoadSound("Resource\\Music\\Enemy_Attack.wav");
	Boom = LoadSound("Resource\\Music\\Boom.wav");
	Destroy_Bridge = LoadSound("Resource\\Music\\Destroy_Bridge.wav");
	Destroy_Enemy = LoadSound("Resource\\Music\\Destroy_Enemy.wav");
	GoBoss = LoadSound("Resource\\Music\\GoBoss.wav");
	Boss_1 = LoadSound("Resource\\Music\\Boss_1.wav");
	Destroy_Boss = LoadSound("Resource\\Music\\Destroy_Boss.wav");
	Pass_Boss = LoadSound("Resource\\Music\\Pass_Boss.wav");
	Game_Over = LoadSound("Resource\\Music\\Game_Over.wav");
	WinGame = LoadSound("Resource\\Music\\WinGame.wav");
}