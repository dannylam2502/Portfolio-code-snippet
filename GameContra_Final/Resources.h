#include <d3dx9.h>
#include "dxaudio.h"
//class này cung cấp tất cả resources cho các class khác (âm thanh và hình ảnh bên ngoài)
//define resource hình ở đây, ví dụ "resources\\image\\contra.jpg"
#define BILL_IMAGE_PATH "Resource\\Image\\Bill.png"
#define INTRO_IMAGE_PATH "Resource\\image\\Intro.png"
#define INTRO_WATERFALL_IMAGE_PATH "Resource\\image\\Intro2.png"
#define INTRO_SNOWFIELD_IMAGE_PATH "Resource\\image\\Intro3.png"
#define MENU_IMAGE_PATH	"Resource\\image\\Menu.png"
#define MISC_IMAGE_PATH "Resource\\image\\Misc.png"
#define BOSS_STAGE1_IMAGE_PATH "Resource\\image\\Boss.png"
#define BACKGROUND_STAGE1_IMAGE_PATH "Resource\\Map\\Stage1.png"
#define GAMEOVER_IMAGE_PATH			"Resource\\image\\GameOver.png"
#define GAMEEND_IMAGE_PATH "Resource\\image\\GameEnd.png"

//Map Animation
#define MAP_ANIMATION_PATH	"Resource\\Map\\Map_Animation.png"
//Stage 1
#define BACKGROUND_STAGE1_IMAGE_PATH "Resource\\Map\\Stage1.png"
#define STAGE1_MAP_PATH "Resource\\Map\\Stage1_Map.txt"
#define STAGE1_OBJ_PATH "Resource\\Map\\Stage1_Objects.txt"
#define STAGE1_QT_PATH "Resource\\Map\\Stage1_QuadTree.txt"
#define STAGE1_MAP_WIDTH	3320
//Stage 2
#define BACKGROUND_STAGE2_IMAGE_PATH "Resource\\Map\\Stage2.png"
#define STAGE2_MAP_PATH "Resource\\Map\\Stage2_Map.txt"
#define STAGE2_OBJ_PATH "Resource\\Map\\Stage2_Objects.txt"
#define STAGE2_QT_PATH "Resource\\Map\\Stage2_QuadTree.txt"
#define STAGE2_MAP_HEIGHT	2176
//Stage 3
#define BACKGROUND_STAGE3_IMAGE_PATH "Resource\\Map\\Stage3.png"
#define STAGE3_MAP_PATH "Resource\\Map\\Stage3_Map.txt"
#define STAGE3_OBJ_PATH "Resource\\Map\\Stage3_Objects.txt"
#define STAGE3_QT_PATH "Resource\\Map\\Stage3_QuadTree.txt"
#define STAGE3_MAP_WIDTH	5104

#define ENEMY_IMAGE_PATH "Resource\\image\\Enemies.png"
#define ENEMY_TWO_IMAGE_PATH "Resource\\image\\Enemies2.png"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define VIEWPORT_X_START 0
#define VIEWPORT_Y_START 0

extern CSound* Intro;
extern CSound* BG_Map1;
extern CSound* BG_Map2;
extern CSound* BG_Map3;
extern CSound* Base_Bullet;
extern CSound* SBullet;
extern CSound* LBullet;
extern CSound* FBullet;
extern CSound* MBullet;
extern CSound* Bill_Jump;
extern CSound* Bill_Dead;
extern CSound* Eat_Item;
extern CSound* Attack_Cannon;
extern CSound* Enemy_Attack;
extern CSound* Boom;
extern CSound* Destroy_Bridge;
extern CSound* Destroy_Enemy;
extern CSound* GoBoss;
extern CSound* Boss_1;
extern CSound* Destroy_Boss;
extern CSound* Pass_Boss;
extern CSound* Game_Over;
extern CSound* WinGame;
void InitSound();