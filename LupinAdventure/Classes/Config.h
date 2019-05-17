#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "cocos2d.h"
#include <vector>
#include "Box2D/Box2D.h"
#include <map>

using namespace std;
USING_NS_CC;

#pragma region Constants
static	Size	SCREEN_SIZE(0, 0);
static	Rect	MY_ZERO_RECT(-1, -1, -1, -1);

#define BOX2D_RATIO		32								// 32 pixels = 1 meter
#define MIN_MOVE_STEP_SIZE	0.03125f					// (1 / BOX2D_RATIO) min step size is 1 pixel
static const b2Vec2 CHAR_MOVE_VELOCITY(9.375f, 0.0f);	// 300 / BOX2D_RATIO, 850 / BOX2D_RATIO
static const b2Vec2 CHAR_JUMP_VELOCITY(0.0f, 26.5625f);
static const b2Vec2 CHAR_CLIMB_VELOCITY(0.0f, 5.3125f);	// 1/5 jump velo

static const Vec2	CHAR_SPAW_POS_IN_PIXEL(150.0f, 200.0f);

#define KEYMAP_SIZE			255

#define STATE_HIT_FLASH_TIME		0.05f
#define STATE_HIT_LOST_CONTROL_TIME	1.0f
#define STATE_HIT_DURATION			3.0f

static const b2Vec3		B2VEC3_ZERO(0.0f, 0.0f, 0.0f);
static const b2Vec2		B2VEC2_ZERO(0.0f, 0.0f);

static const Vec2		FLIP_FACE_RIGHT(-1.0f, 1.0f);
static const Vec2		FLIP_FACE_LEFT(1.0f, 1.0f);
#define FLIP_X			FLIP_FACE_LEFT
#define FLIP_NONE		FLIP_FACE_RIGHT

static const b2Vec2				PHYSICS_GRAVITY(0.0f, -98.0f);

static const PhysicsMaterial	PHYSIC_MATERIAL_PLATFORM(1.0f, 0.0f, 1.0f);
static const PhysicsMaterial	PHYSIC_MATERIAL_OBSTACLE(1.0f, 0.0f, 0.0f);
static const PhysicsMaterial	PHYSIC_MATERIAL_CHARACTER(-1.0f, 0.0f, 1.0f);
#define PHYSIC_MATERIAL_MOB		PHYSIC_MATERIAL_CHARACTER
#define PHYSIC_MATERIAL_BULLET	PHYSIC_MATERIAL_OBSTACLE

#pragma endregion

#pragma region Support Macros
#define FOR(I)						for(int i = 0; i < I; ++i)

#define GET_SIGN(F)					((F > 0) - (F < 0))
#define GET_BIG(A, B)				(A > B ? A : B)

#define SWAP(ptrA, ptrB, ptrType)	{ptrType* temp = ptrA; ptrA = ptrB; ptrB = temp;}

#define IS_GOING_LEFT(veloX)		(veloX < -CO_MOVE_STEP_SIZE)
#define IS_GOING_RIGHT(veloX)		(veloX > CO_MOVE_STEP_SIZE)
#pragma endregion

#pragma region Links and Paths
static const char* XML_MAP_LIST						= "Map//MapList.xml";
static const char* XML_PLAYER_RESOURCE				= "Character//DrukenLupin";
static const char* XML_MAP_KEY						= "Map";
static const char* XML_MAP_ELEM_NAME				= "name";
static const char* XML_MAP_ELEM_TMX					= "tmx";
static const char* XML_MAP_ELEM_BG					= "background";
static const char* XML_ANIMATION_SKILL_DARK_BLUE	= "Ball//DarkBall";
static const char* XML_ANIMATION_ELEM_ANI			= "Animation";
static const char* XML_ANIMATION_ATTR_KEY			= "Key";
static const char* XML_BULLETINFO					= "Bullet//BulletInfo.xml";
static const char* XML_BULLETINFO_ELEM_INFO			= "Info";
static const char* XML_MOBINFORMATION_ELEM			= "MobInformation";


static const char* RESOURCE_PATH_TRAP				= "Trap//Trap";
static const char* RESOURCE_PATH_ITEM				= "Item//potion";

#define RESOURCE_PATH_PLATFORM_DYNAMIC(name)		("Map//Reactor//Dynamic Platform//"name)
#define RESOURCE_PATH_REACTOR(name)					("Map//Reactor//"name)
#define RESOURCE_PATH_MOB(name)						("Monster//"name)
#define RESOURCE_PATH_BULLET(name)					("Ball//"name)
#define RESOURCE_PATH_TRAP(name)					("Trap//"name)
#define RESOURCE_PATH_FAMILIAR(name)				("Familiar//"name)
#pragma endregion

#pragma region Structs and Named Enums
// Dung
enum	TypeBullet
{
	BULLET_PLAYER_1 = 3000,
	BULLET_PLAYER_2,
	BULLET_PLAYER_3
};
struct	MapInfo
{
	string* name;
	string* tmxPath;
	string* bgPath;

	MapInfo(const char* n, const char* t, const char* b)
	{
		name = new string(n);
		tmxPath = new string(t);
		bgPath = new string(b);
	}

	~MapInfo()
	{
		CC_SAFE_DELETE(name);
		CC_SAFE_DELETE(tmxPath);
		CC_SAFE_DELETE(bgPath);
	}
};



// Frederick Tran
enum	KeyState
{
	KS_UP = 0,
	KS_DOWN,
	KS_PRESS,
	KS_RELEASE
};
struct	CControllerKeySet
{
	EventKeyboard::KeyCode	keyCode;

	// The state for the key activate the command.
	KeyState				keyState;

	// The command which is pushed to object if the key activated.
	int						keyCommand;

	CControllerKeySet(EventKeyboard::KeyCode code, KeyState state, int command)
	{
		keyCode = code;
		keyState = state;
		keyCommand = command;
	}
};
struct	AIAttackCommandSet
{
	float	m_ActivateRate;
	int		m_Command;
	float	m_Duration;
	float	m_CurrentDuration;

	// Parameter:
	//	+ activateRate: The rate to give the [command] to object.
	//	+ command: The given command.
	//	+ duration: The time between 2 times this command have change to activate.
	AIAttackCommandSet(float activateRate, int command, float duration)
	{
		m_ActivateRate = activateRate;
		m_Command = command;
		m_Duration = duration;
		m_CurrentDuration = 0.0f;
	}
};
struct	KeyEvent
{
	EventKeyboard::KeyCode	keyCode;
	KeyState				stateEvent;

	KeyEvent(EventKeyboard::KeyCode code, KeyState sEvent)
	{
		keyCode = code;
		stateEvent = sEvent;
	}
};



// Eric Nguyen
struct	PlistOption
{
	String	textureFileName;
	String	plist;
	String	spriteFrameName;
	int		nFrames;
	float	frameTimes;
	Vec2	anchorPoint;
};
struct	SpriteInformation
{
	int		Key;
	String	Name;
	String	Path;
	int		nFrames;
	int		From;
	int		To;
	float	frameTime;
	Vec2	anchorPoint;
};
struct	ActionInfomation
{
	int			animateKey;
	Animate*	animate;
	Vec2		anchorPoint;

	ActionInfomation(Animate* ani, Vec2 point)
	{
		this->animateKey = 0;
		this->animate = ani;
		this->anchorPoint = point;
	}

	ActionInfomation(int key, Animate* ani, Vec2 point)
	{
		this->animateKey = key;
		this->animate = ani;
		this->anchorPoint = point;
	}
};
struct	MobInformation
{
	string	name;
	float	bodyWidth;
	float	bodyHeight;
	float	velocityX;
	float	velocityY;
	float	activeRate;
	float	roundDuration;
	int		statsHP;
	int		statsMP;
	int		statsEXP;
	int		statsLV;

	MobInformation()
	{
		this->name = "";
		this->bodyWidth = this->bodyHeight	= 0.0f;
		this->velocityX = this->velocityY	= 0.0f;
		this->activeRate = 0.0f;
		this->roundDuration = 0.0f;
		this->statsHP = this->statsMP = this->statsEXP = this->statsLV = 0;
	}
};



// ZinLe
struct LevelInfo
{
	int level;
	int hp;
	int mp;
	string			resource;
};
struct BulletInfo
{
	int				type;
	string			resource;
	Vec2			shootSpot;
	LevelInfo		levelInfo;
	Vec2			runTime; // x = run time effect start, y = time live.
};
#pragma endregion

namespace Z_ORDER
{
	enum LAYERS
	{
		BackGround = 0,
		GameWorld,
		Effect,
		UI
	};
	enum GAMEWORLD
	{
		Map,
		BackEffect,
		Player,
		Familiar,
		FrontEffect,
		Bullet,
		FrontAllEffect
	};
	enum MAPOBJECT
	{
		Tile,
		Mob,
		Obstacle,
		MapHideBush,
	};
}

namespace ID
{
	static const char* BULLET_DARK_BALL				= "BULLET_DARK_BALL";
	static const char* BULLET_FIRE_BALL				= "BULLET_FIRE_BALL";
	static const char* BULLET_BANANA				= "BULLET_BANANA";
	static const char* BULLET_EARTH_WAKE			= "BULLET_EARTH_WAKE";
	static const char* BULLET_POISON_BOOM			= "BULLET_POISON_BOOM";
	static const char* BULLET_BANANA_SET			= "BULLET_BANANA_SET";
	static const char* BULLET_GREEN_BANANA			= "BULLET_GREEN_BANANA";
	static const char* BULLET_RED_DRAKE				= "BULLET_RED_DRAKE";
	static const char* BULLET_DARK_BAT				= "BULLET_DARK_BAT";

	static const char* FAMILIAR_FIRE				= "FAMILIAR_FIRE";

	static const char* MOB_COPPER_DRAKE				= "MOB_COPPER_DRAKE";
	static const char* MOB_CURSE_EYE				= "MOB_CURSE_EYE";
	static const char* MOB_DARK_DRAKE				= "MOB_DARK_DRAKE";
	static const char* MOB_DRAGON					= "MOB_DRAGON";
	static const char* MOB_EVIL_EYE					= "MOB_EVIL_EYE";
	static const char* MOB_FAUST					= "MOB_FAUST";
	static const char* MOB_HORNY_MUSHROOM			= "MOB_HORNY_MUSHROOM";
	static const char* MOB_ICE_DRAKE				= "MOB_ICE_DRAKE";
	static const char* MOB_KING_SMILE				= "MOB_KING_SMILE";
	static const char* MOB_LUPIN					= "MOB_LUPIN";
	static const char* MOB_MALADY					= "MOB_MALADY";
	static const char* MOB_PIG						= "MOB_PIG";
	static const char* MOB_RED_DRAKE				= "MOB_RED_DRAKE";
	static const char* MOB_RIBBON					= "MOB_RIBBON";
	static const char* MOB_SNAIL					= "MOB_SNAIL";
	static const char* MOB_STATIC_LUPIN_LEFT		= "MOB_STATIC_LUPIN_LEFT";
	static const char* MOB_STATIC_LUPIN_RIGHT		= "MOB_STATIC_LUPIN_RIGHT";
	static const char* MOB_STRIGE					= "MOB_STRIGE";
	static const char* MOB_ZOMBIE_LUPIN				= "MOB_ZOMBIE_LUPIN";

	static const char* PLAYER						= "PLAYER";

	static const char* PLATFORM_DYNAMIC				= "PLATFORM_DYNAMIC";
	static const char* PLATFORM_DYNAMIC_TYPE1_X		= "PLATFORM_DYNAMIC_TYPE1_X";
	static const char* PLATFORM_DYNAMIC_TYPE1_Y		= "PLATFORM_DYNAMIC_TYPE1_Y";
	static const char* PLATFORM_DYNAMIC_TYPE2_X		= "PLATFORM_DYNAMIC_TYPE2_X";
	static const char* PLATFORM_DYNAMIC_TYPE2_Y		= "PLATFORM_DYNAMIC_TYPE2_Y";
	static const char* PLATFORM_HITCH				= "PLATFORM_HITCH";
	static const char* PLATFORM_LADDER				= "PLATFORM_LADDER";
	static const char* PLATFORM_POLYGON				= "PLATFORM_POLYGON";
	static const char* PLATFORM_OBSTACLE			= "PLATFORM_OBSTACLE";
	static const char* PLATFORM_ROPE				= "PLATFORM_ROPE";
	static const char* PLATFORM_STATIC				= "PLATFORM_STATIC";

	static const char* REACTOR_GUIDE_JUMP			= "REACTOR_GUIDE_JUMP";
	static const char* REACTOR_GUIDE_RIGHT			= "REACTOR_GUIDE_RIGHT";
	static const char* REACTOR_GUIDE_SHOOT			= "REACTOR_GUIDE_SHOOT";
	static const char* REACTOR_GUIDE_UP				= "REACTOR_GUIDE_UP";
	static const char* REACTOR_PLATFORM_SPAWNER_Y	= "REACTOR_PLATFORM_SPAWNER_Y";
	static const char* REACTOR_PORTAL_1				= "REACTOR_PORTAL_1";
	static const char* REACTOR_PORTAL_2				= "REACTOR_PORTAL_2";
	static const char* REACTOR_SPRING				= "REACTOR_SPRING";
	static const char* REACTOR_SPRING_UP_LEFT		= "REACTOR_SPRING_UP_LEFT";
	static const char* REACTOR_SPRING_UP_RIGHT		= "REACTOR_SPRING_UP_RIGHT";

	static const char* TRAP_HOLE					= "TRAP_HOLE";
	static const char* TRAP_SHURIKEN_TYPE1_X		= "TRAP_SHURIKEN_TYPE1_X";
	static const char* TRAP_SHURIKEN_TYPE1_Y		= "TRAP_SHURIKEN_TYPE1_Y";
	static const char* TRAP_SHURIKEN_TYPE2_X		= "TRAP_SHURIKEN_TYPE2_X";
	static const char* TRAP_SHURIKEN_TYPE2_Y		= "TRAP_SHURIKEN_TYPE2_Y";
	static const char* TRAP_SPIKE_50				= "TRAP_SPIKE_50";

	static const char* TRAP_TRAP1					= "TRAP_TRAP1";

	static const char* ITEM_BONUS_HP				= "ITEM_BONUS_HP";
	static const char* ITEM_RESTORE_HP_100			= "ITEM_RESTORE_HP_100";
	static const char* ITEM_RESTORE_HP_250			= "ITEM_RESTORE_HP_250";
	static const char* ITEM_RESTORE_MP_50			= "ITEM_RESTORE_MP_50";

};

namespace TAG
{
	namespace COMPONENT
	{
		enum
		{
			NONE = 0,
			STATS,
			MOVE,
			CONTROLLER,
			ANIMATION,
			DAMAGE,
			ATTACK,
			SENSOR,
			AI_MOVE,
			AI_ATTACK,
			STATE,
			FAMILIAR,
			HPBAR
		};
	}
	namespace OBJECT
	{
		using namespace ID;
	}
}

namespace NAME
{
	using namespace ID;
}

namespace ANIMATION_INDEX
{
	enum
	{
		FIRST = 2000,
		STAND = 2000,
		MOVE,
		HIT,
		DIE,
		JUMP,
		ATTACK_AFTER,
		REGEN,
		LADDER,
		ROPE,
		ATTACK_BEFORE,
		EMPTY_SLOT_1,
		EMPTY_SLOT_2,
		LAST_PLUS_1
	};

	namespace FAMILIAR
	{
		enum
		{
			ATTACK_AFTER = ATTACK_AFTER,
			ATTACK_BEFORE = ATTACK_BEFORE,
			DIE = DIE,
			HIT = HIT,
			MOVE = MOVE,
			REGEN = REGEN,
			STAND = STAND
		};
	}

	namespace BULLET
	{
		enum
		{
			ICON = 2000,
			HIT,
			BALL,
			EFFECT_START,
			EFFECT_END
		};
	}

	namespace TRAP
	{
		enum
		{
			TRAP1 = 2000,
			TRAP2,
			TRAP3,
			TRAP4,
			TRAP5,
			TRAP6,
			TRAP7,
			TRAP8
		};
	}

	namespace ITEM
	{
		enum
		{
			BONUS_HP = 2000,
			BONUS_HP_EFFECT,
			BONUS_HP_ACT,
			HP1,
			HP1_EFFECT,
			HP1_ACT,
			HP2,
			HP2_EFFECT,
			HP2_ACT,
			MP1,
			MP1_EFFECT,
			MP1_ACT
		};
	}

	namespace SPRING
	{
		enum
		{
			STRAIGHT = 2000,
			CROSS
		};
	}
}

namespace STATE_INDEX
{
	enum
	{
		FIRST = 7000,
		STAND = 7000,
		MOVE,
		HIT,
		DIE,
		JUMP,
		ATTACK,
		POWER,
		CLIMB,
		SPAWN,
		LAST_PLUS_1
	};
}

namespace COMMAND
{
	// 1000 ~ 8999
	namespace COMPONENT
	{
		/*1000*/namespace MOVE
		{
			enum
			{
				LEFT = 1000,
				RIGHT,
				UP,
				DOWN,
				JUMP,
				DASH,
				FLY,
				CHASE,
				KNOCK_BACK,
				STOP_LEFT,
				STOP_RIGHT,
				STOP_UP,
				STOP_DOWN,
				STOP_ALL,
				BOTH
			};
		}
		/*2000*/namespace ANIMATION
		{
			namespace CHANGE_TO
			{
				using namespace ANIMATION_INDEX;
			}
			
			enum
			{
				START_SHADOW = 2100,
				STOP_SHADOW,
				INVISIBLE,
				VISIBLE,
				START_LOOP,
				STOP_LOOP,
				DONE,
				FACE_LEFT,
				FACE_RIGHT
			};
		}
		/*3000*/namespace ATTACK
		{
			enum
			{
				ACTIVATE = 3000
			};
		}
		/*4000*/namespace AI_MOVE
		{
			enum
			{
				STOP_CHASE = 4000,
				START_CHASE
			};
		}
		/*5000*/namespace STATS
		{
			enum
			{
				RESET = 5000
			};
		}
		/*6000*/namespace SENSOR
		{
			enum
			{
				REMOVED = 6000
			};
		}
		/*7000*/namespace STATE
		{
			namespace CHANGE_TO
			{
				using namespace STATE_INDEX;
			};
		}
		/*8000*/namespace AI_ATTACK
		{
			enum
			{
				GIVE_UP_TARGET = 8000
			};
		}
		/*9000*/namespace FAMILIAR
		{
			enum
			{
				FIRST = 9000,
				NEW_FAMILIAR = 9000,
				ACTIVATE_ABILLITY,
				NEXT,
				PREVIOUS,
				LAST_PLUS_1
			};
		}
	}

	// 10000 ~
	namespace ENTITY
	{
		enum
		{
			NONE = 0,
			LANDING = 10000,
			CLIMB_START,
			CLIMB_STOP,
			CLIMB_READY,
			CLIMB_UNREADY,
			EXPIRED_DAMAGE,
			ACTIVATE,
			DEACTIVATE,
			MAKE_DAMAGE,
			DIE

		};
	}
}

namespace BITMASK
{
	// For body filter
	namespace OBJECT
	{
		enum
		{
			ALL				= 0xff,
			NONE			= 0,	// 0000 0000
			GENERAL			= 1,	// 0000 0001	The destroyable map object
			MOB				= 2,	// 0000 0010
			PLAYER			= 4,	// 0000 0100
			BOTH_ALLY		= 6,	// 0000 0110	Mark both Player and Mob
			PLAYER_BULLET	= 8,	// 0000 1000
			MOB_BULLET		= 16,	// 0001 0000
			ALL_BULLET		= 24,	// 0001 1000	Mark both Player and Mob bullets
			TILE			= 32,	// 0010 0000
			LADDER_ROPE		= 64,	// 0100 0000
			OBSTACLE		= 128,	// 1000	0000
			TRAP			= 256,	// 1 0000 0000
			HITCH			= 512   // 1 0000 00000
		};
	}

	// For SContactListener
	namespace COLLIDE
	{
		enum
		{
			// 1st Object in Contact Listener
			PLAYER					= 1,
			ENEMY					= 2,
			GENERAL_OBJECT			= 4,

			// 2nd Object in Contact Listener
			WITH_OPPONENT			= 8,
			WITH_TILE				= 16,
			WITH_GENERAL_OBJECT		= 32,
			WITH_OPPONENT_BULLET	= 64,
			WITH_CLIMBABLE_OBJECT	= 128
		};
	}
}

#endif // !__CONFIG_H__