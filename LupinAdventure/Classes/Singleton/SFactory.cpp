#include "SFactory.h"

#include "tinyxml2/tinyxml2.h"
#include "Config.h"
#include "Utils.h"
#include "UParam.h"
#include "Scene/GamePlayScene.h"
#include "Scene/GameOverScene.h"
#include "TileMap/SMapManager.h"
#include "Entity/EObject.h"

#include "Singleton/SFactoryEffect.h"
#include "Singleton/SCamera.h"

#include "Component/CStats.h"
#include "Component/CMove.h"
#include "Component/CController.h"
#include "Component/CAnimation.h"
#include "Component/CAttack.h"
#include "Component/CAIMove.h"
#include "Component/CState.h"
#include "Component/CDamage.h"
#include "Component/CSensor.h"
#include "Component/CFamiliar.h"
#include "Component/CReact.h"
#include "Component/CHPBar.h"

#include "Component/State.h"
#include "Component/StandState.h"
#include "Component/StateClimb.h"
#include "Component/StateJump.h"
#include "Component/StateMove.h"
#include "Component/StateHit.h"
#include "Component/StateAttack.h"
#include "Component/StateDie.h"

#include "Familiar/FamiliarFire.h"
#include "Familiar/FamiliarDark.h"

// Dung 30/6
#include "CCShake.h"

b2World* SFactory::m_PhysicsWorld = nullptr;
//map<const char*, MobInformation*> SFactory::m_MobInformation;

Layer* SFactory::m_GameWorld = nullptr;

vector<EObject*> SFactory::m_DestroyedEntity;

#pragma region Constructor - Do nothing
SFactory::SFactory()
{
}

SFactory::~SFactory()
{
}
#pragma endregion

void SFactory::initialize(b2World* physicsWorld, Layer* gameworld)
{
	m_PhysicsWorld = physicsWorld;
	m_GameWorld = gameworld;
}

#pragma region About Destroy Entities
void destroyEntityIfOutOfViewPort(EObject* entity)
{
	if (entity->getBoundingBox().intersectsRect(SCamera::getInstance()->getCameraViewPort()))
	{
		return;
	}

	SFactory::addToDestroyer(entity);
}

void SFactory::destroyEntity(EObject* entity)
{
	auto body = (b2Body*)entity->getUserData();

	if (body != nullptr)
	{
		body->GetWorld()->DestroyBody(body);
	}

	entity->removeFromParentAndCleanup(true);
}

void SFactory::setDestroyOnOutOfViewPort(EObject* object)
{
	Action* removeAction =
		RepeatForever::create(Sequence::create(DelayTime::create(0.1f),
		CallFunc::create(std::bind(&destroyEntityIfOutOfViewPort, object)), nullptr));

	object->runAction(removeAction);
}

void SFactory::setDestroyOnCommand(EObject* object, int command)
{
	auto action = CallFunc::create(std::bind(&SFactory::addToDestroyer, object));
	action->retain();

	object->setProperty(TAG::COMPONENT::NONE, UPARAM_TYPE_ACTION_PAIR, UParam(command, action));
}

void SFactory::setDestroyOnTime(EObject* object, float time)
{
	auto waitAction = DelayTime::create(time);
	auto destroyAction = CallFunc::create(std::bind(&SFactory::addToDestroyer, object));
	auto finalAction = Sequence::create(waitAction, destroyAction, nullptr);

	object->runAction(finalAction);
}

void SFactory::updateDestroyer()
{
	for (EObject* entity : m_DestroyedEntity)
	{
		destroyEntity(entity);
	}

	m_DestroyedEntity.clear();
}

void SFactory::addToDestroyer(EObject* entity)
{
	for (EObject* object : m_DestroyedEntity)
	{
		if (object == entity)
		{
			return;
		}
	}
	m_DestroyedEntity.push_back(entity);
}
#pragma endregion

#pragma region Create Body and Sensor
b2Body* SFactory::createBody(EObject* object, float posX, float posY, 
	float height, float width, PhysicsMaterial material,
	b2Filter collistionBitmask, b2BodyType bodyType, b2Vec2 center)
{
	b2Body* result = nullptr;

	posX /= BOX2D_RATIO;
	posY /= BOX2D_RATIO;
	width /= BOX2D_RATIO;
	height /= BOX2D_RATIO;

	if (object != nullptr)
		result = (b2Body*)object->getUserData();

	if (result == nullptr)
	{
		b2BodyDef bodyInfo;
		bodyInfo.type = bodyType;
		bodyInfo.position.Set(posX, posY);
		bodyInfo.userData = object;
		result = m_PhysicsWorld->CreateBody(&bodyInfo);
	}

	b2PolygonShape shape;
	b2FixtureDef bodyMaterial;

	// Box2D get 1/2 of width and height to set the object bounding box.
	shape.SetAsBox(width / 2, height / 2, b2Vec2(center.x * width, center.y * height), 0.0f);
	bodyMaterial.shape = &shape;
	bodyMaterial.density = material.density;
	bodyMaterial.friction = material.friction;
	bodyMaterial.restitution = material.restitution;
	bodyMaterial.filter = collistionBitmask;

	result->CreateFixture(&bodyMaterial);
	result->SetFixedRotation(true);

	if (object != nullptr)
	{
		object->setContentSize(Size(width * BOX2D_RATIO, height * BOX2D_RATIO));
		object->setUserData(result);
	}
	
	return result;
}

b2Body* SFactory::createSensor(EObject* object, float posX, float posY,
	float height, float width, b2Filter collistionBitmask, b2BodyType bodyType, b2Vec2 center)
{
	b2Body* result = nullptr;

	posX /= BOX2D_RATIO;
	posY /= BOX2D_RATIO;
	width /= BOX2D_RATIO;
	height /= BOX2D_RATIO;

	if (object != nullptr)
		result = (b2Body*)object->getUserData();

	if (result == nullptr)
	{
		b2BodyDef bodyInfo;
		bodyInfo.type = bodyType;
		bodyInfo.position.Set(posX, posY);
		bodyInfo.userData = object;

		result = m_PhysicsWorld->CreateBody(&bodyInfo);
	}

	b2PolygonShape shape;
	b2FixtureDef bodyMaterial;

	// Box2D get 1/2 of width and height to set the object bounding box.
	shape.SetAsBox(width / 2, height / 2,  b2Vec2(center.x * width, center.y * height), 0.0f);
	bodyMaterial.shape = &shape;
	bodyMaterial.isSensor = true;
	bodyMaterial.filter = collistionBitmask;

	result->CreateFixture(&bodyMaterial);
	result->SetFixedRotation(true);

	if (object != nullptr)
	{
		object->setContentSize(Size(width * BOX2D_RATIO, height * BOX2D_RATIO));
		object->setUserData(result);
	}

	return result;
}

b2Body*	SFactory::createSensor(EObject* object, float posX, float posY, float radius,
	b2Filter collistionBitmask, b2BodyType bodyType)
{
	b2Body* result = nullptr;

	posX /= BOX2D_RATIO;
	posY /= BOX2D_RATIO;
	radius /= BOX2D_RATIO;

	if (object != nullptr)
		result = (b2Body*)object->getUserData();

	if (result == nullptr)
	{
		b2BodyDef bodyInfo;
		bodyInfo.type = bodyType;
		bodyInfo.position.Set(posX, posY);
		bodyInfo.userData = object;

		result = m_PhysicsWorld->CreateBody(&bodyInfo);
	}

	b2CircleShape shape;
	b2FixtureDef bodyMaterial;

	// Box2D get 1/2 of width and height to set the object bounding box.
	shape.m_p.Set(0, 0);
	shape.m_radius = radius;
	bodyMaterial.shape = &shape;
	bodyMaterial.isSensor = true;
	bodyMaterial.filter = collistionBitmask;

	result->CreateFixture(&bodyMaterial);
	result->SetFixedRotation(true);

	if (object != nullptr)
	{
		object->setContentSize(Size(radius * 2 * BOX2D_RATIO, radius * 2 * BOX2D_RATIO));
		object->setUserData(result);
	}

	return result;
}

// Dung
b2Body* SFactory::createBody(EObject* object, float posX, float posY,
	float height, float width, PhysicsMaterial material, 
	b2Filter collistionBitmask, ValueVector points, b2BodyType bodyType)
{
	if (points.size() == 0)
	{
		return createBody(object, posX, posY, height, width, material, collistionBitmask, b2BodyType::b2_staticBody);
	}

	b2Body* result;
	b2BodyDef bodyInfo;
	b2Vec2 vertices[b2_maxPolygonVertices];
	int vindex = 0;

	if (points.size() > b2_maxPolygonVertices) {
		return NULL;
	}
	bodyInfo.type = b2BodyType::b2_kinematicBody;
	bodyInfo.position.Set(posX / BOX2D_RATIO, posY / BOX2D_RATIO);
	bodyInfo.userData = object;
	result = m_PhysicsWorld->CreateBody(&bodyInfo);

	b2PolygonShape shape;
	b2FixtureDef bodyMaterial;

	for (Value point : points) {
		vertices[vindex].x = (point.asValueMap()["x"].asFloat() / BOX2D_RATIO);
		vertices[vindex].y = (-point.asValueMap()["y"].asFloat() / BOX2D_RATIO);
		vindex++;
	}
	
	shape.Set(vertices, vindex);
	
	bodyMaterial.shape = &shape;
	bodyMaterial.density = material.density;
	bodyMaterial.friction = material.friction;
	bodyMaterial.restitution = material.restitution;
	bodyMaterial.filter = collistionBitmask;

	result->CreateFixture(&bodyMaterial);
	result->SetFixedRotation(true);

	object->setContentSize(Size(width, height));
	object->setUserData(result);

	return result;

}
#pragma endregion

EObject* SFactory::create(const char* id)
{
	return 
		Switch<const char*, EObject* (*)()>
		([](const char* a, const char* b){return strcmp(a, b) < 0; },
		id,
		{ // IMPORTANT: the key are sorted A->Z
		{ ID::BULLET_BANANA,			&SFactory::create_BULLET_BANANA },
		{ ID::BULLET_BANANA_SET,		&SFactory::create_BULLET_BANANA_SET },
		{ ID::BULLET_DARK_BALL,			&SFactory::create_BULLET_DARK_BALL },
		{ ID::BULLET_DARK_BAT,			&SFactory::create_BULLET_DARK_BAT },
		{ ID::BULLET_EARTH_WAKE,		&SFactory::create_BULLET_EARTH_WAKE },
		{ ID::BULLET_FIRE_BALL,			&SFactory::create_BULLET_FIRE_BALL},
		{ ID::BULLET_GREEN_BANANA,		&SFactory::create_BULLET_GREEN_BANANA },
		{ ID::BULLET_POISON_BOOM,		&SFactory::create_BULLET_POISON_BOOM },
		{ ID::BULLET_RED_DRAKE,			&SFactory::create_BULLET_RED_DRAKE },

		{ ID::ITEM_BONUS_HP,			&SFactory::create_ITEM_BONUS_HP },
		{ ID::ITEM_RESTORE_HP_100,		&SFactory::create_ITEM_RESTORE_HP_100 },
		{ ID::ITEM_RESTORE_HP_250,		&SFactory::create_ITEM_RESTORE_HP_250 },
		{ ID::ITEM_RESTORE_MP_50,		&SFactory::create_ITEM_RESTORE_MP_50 },

		{ ID::MOB_COPPER_DRAKE,			&SFactory::create_MOB_COPPER_DRAKE },
		{ ID::MOB_CURSE_EYE,			&SFactory::create_MOB_CURSE_EYE },
		{ ID::MOB_DARK_DRAKE,			&SFactory::create_MOB_DARK_DRAKE },
		{ ID::MOB_DRAGON,				&SFactory::create_MOB_DRAGON },
		{ ID::MOB_EVIL_EYE,				&SFactory::create_MOB_EVIL_EYE },
		{ ID::MOB_FAUST,				&SFactory::create_MOB_FAUST },
		{ ID::MOB_HORNY_MUSHROOM,		&SFactory::create_MOB_HORNY_MUSHROOM },
		{ ID::MOB_ICE_DRAKE,			&SFactory::create_MOB_ICE_DRAKE },
		{ ID::MOB_KING_SMILE,			&SFactory::create_MOB_KING_SMILE },
		{ ID::MOB_LUPIN,				&SFactory::create_MOB_LUPIN },
		{ ID::MOB_MALADY,				&SFactory::create_MOB_MALADY },
		{ ID::MOB_PIG,					&SFactory::create_MOB_PIG },
		{ ID::MOB_RED_DRAKE,			&SFactory::create_MOB_RED_DRAKE },
		{ ID::MOB_RIBBON,				&SFactory::create_MOB_RIBBON },
		{ ID::MOB_SNAIL,				&SFactory::create_MOB_SNAIL },
		{ ID::MOB_STATIC_LUPIN_LEFT,	&SFactory::create_MOB_STATIC_LUPIN_LEFT },
		{ ID::MOB_STATIC_LUPIN_RIGHT,	&SFactory::create_MOB_STATIC_LUPIN_RIGHT },
		{ ID::MOB_STRIGE,				&SFactory::create_MOB_STRIGE },
		{ ID::MOB_ZOMBIE_LUPIN,			&SFactory::create_MOB_ZOMBIE_LUPIN },

		
		{ ID::PLATFORM_DYNAMIC_TYPE1_X, &SFactory::create_PLATFORM_DYNAMIC_TYPE1_X },
		{ ID::PLATFORM_DYNAMIC_TYPE1_Y, &SFactory::create_PLATFORM_DYNAMIC_TYPE1_Y },
		{ ID::PLATFORM_DYNAMIC_TYPE2_X, &SFactory::create_PLATFORM_DYNAMIC_TYPE2_X },
		{ ID::PLATFORM_DYNAMIC_TYPE2_Y, &SFactory::create_PLATFORM_DYNAMIC_TYPE2_Y },

		{ ID::PLATFORM_HITCH,			&SFactory::create_PLATFORM_HITCH },
		{ ID::PLATFORM_LADDER,			&SFactory::create_PLATFORM_LADDER },
		{ ID::PLATFORM_OBSTACLE,		&SFactory::create_PLATFORM_OBSTACLE },
		{ ID::PLATFORM_POLYGON,			&SFactory::create_PLATFORM_POLYGON },
		{ ID::PLATFORM_ROPE,			&SFactory::create_PLATFORM_ROPE },
		{ ID::PLATFORM_STATIC,			&SFactory::create_PLATFORM_STATIC },

		{ ID::PLAYER,					&SFactory::create_PLAYER },

		{ ID::REACTOR_GUIDE_JUMP,		&SFactory::create_REACTOR_GUIDE_JUMP },
		{ ID::REACTOR_GUIDE_RIGHT,		&SFactory::create_REACTOR_GUIDE_RIGHT },
		{ ID::REACTOR_GUIDE_SHOOT,		&SFactory::create_REACTOR_GUIDE_SHOOT },
		{ ID::REACTOR_GUIDE_UP,			&SFactory::create_REACTOR_GUIDE_UP },
		{ ID::REACTOR_PLATFORM_SPAWNER_Y, &SFactory::create_REACTOR_PLATFORM_SPAWNER_Y },
		{ ID::REACTOR_PORTAL_1,			&SFactory::create_REACTOR_PORTAL_1 },
		{ ID::REACTOR_PORTAL_2,			&SFactory::create_REACTOR_PORTAL_2 },
		{ ID::REACTOR_SPRING,			&SFactory::create_REACTOR_SPRING },
		{ ID::REACTOR_SPRING_UP_LEFT,	&SFactory::create_REACTOR_SPRING_UP_LEFT },
		{ ID::REACTOR_SPRING_UP_RIGHT,	&SFactory::create_REACTOR_SPRING_UP_RIGHT },
		
		{ ID::TRAP_HOLE,				&SFactory::create_TRAP_HOLE },

		{ ID::TRAP_SHURIKEN_TYPE1_X,	&SFactory::create_TRAP_SHURIKEN_TYPE1_X },
		{ ID::TRAP_SHURIKEN_TYPE1_Y,	&SFactory::create_TRAP_SHURIKEN_TYPE1_Y },

		{ ID::TRAP_SHURIKEN_TYPE2_X,	&SFactory::create_TRAP_SHURIKEN_TYPE2_X },
		{ ID::TRAP_SHURIKEN_TYPE2_Y,	&SFactory::create_TRAP_SHURIKEN_TYPE2_Y },

		{ ID::TRAP_SPIKE_50,			&SFactory::create_TRAP_SPIKE_50 }
		})
		();
}

#pragma region Create Specific Object Methods
EObject*	SFactory::create_PLAYER()
{
	EObject*					object = nullptr;
	vector<CControllerKeySet>	keyMap;
	vector<CBase*>				componentBoard;
	map<int, State*>			stateMap;

	b2Filter					collideBitmask;
	b2Body*						body			= nullptr;
	static float				bodyWidth		= 25.0f;
	static float				bodyHeight		= 76.0f;
	static int					hp				= 235;
	static b2Vec2				shootSpot		= b2Vec2(bodyWidth, 40);

	static EventKeyboard::KeyCode	keyUp = EventKeyboard::KeyCode::KEY_UP_ARROW;
	static EventKeyboard::KeyCode	keyDown = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	static EventKeyboard::KeyCode	keyLeft = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	static EventKeyboard::KeyCode	keyRight = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;

	static EventKeyboard::KeyCode	keyShoot = EventKeyboard::KeyCode::KEY_C;
	static EventKeyboard::KeyCode	keyJump = EventKeyboard::KeyCode::KEY_X;
	static EventKeyboard::KeyCode	keyFamiliarShoot = EventKeyboard::KeyCode::KEY_V;
	static EventKeyboard::KeyCode	keyFamiliarNext = EventKeyboard::KeyCode::KEY_S;
	static EventKeyboard::KeyCode	keyFamiliarPrev = EventKeyboard::KeyCode::KEY_A;


	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(NAME::PLAYER);
	// =====================================================================
	
	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::PLAYER;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::PLAYER ^ BITMASK::OBJECT::PLAYER_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_CHARACTER, collideBitmask, b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CStats::addComponentTo(object, hp, 0, hp, 0, 3);
	CMove::addComponentTo(object, CHAR_MOVE_VELOCITY, CHAR_JUMP_VELOCITY, CHAR_CLIMB_VELOCITY);
	CAnimation::addComponentTo(object, XML_PLAYER_RESOURCE);
	CAttack::addComponentTo(object, ID::BULLET_POISON_BOOM, shootSpot);
	CFamiliar::addComponentTo(object);

	//object->setProperty(TAG::COMPONENT::FAMILIAR, UPARAM_TYPE_EOBJECT, FamiliarDark::create(object));
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));

	SFactory::setCalledMethodAtCommand(
		object,
		&SFactory::calledAtPlayerDie,
		(int)COMMAND::ENTITY::DIE);
	// =====================================================================

#pragma region Eric	- Create States

#pragma region Climb State
	keyMap.push_back(CControllerKeySet(keyUp, KS_PRESS, COMMAND::COMPONENT::MOVE::UP));
	keyMap.push_back(CControllerKeySet(keyUp, KS_DOWN, COMMAND::COMPONENT::MOVE::UP));
	keyMap.push_back(CControllerKeySet(keyUp, KS_RELEASE, COMMAND::COMPONENT::MOVE::STOP_ALL));

	keyMap.push_back(CControllerKeySet(keyDown, KS_PRESS, COMMAND::COMPONENT::MOVE::DOWN));
	keyMap.push_back(CControllerKeySet(keyDown, KS_DOWN, COMMAND::COMPONENT::MOVE::DOWN));
	keyMap.push_back(CControllerKeySet(keyDown, KS_RELEASE, COMMAND::COMPONENT::MOVE::STOP_ALL));

	keyMap.push_back(CControllerKeySet(keyLeft, KS_DOWN, COMMAND::COMPONENT::ANIMATION::FACE_LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_DOWN, COMMAND::COMPONENT::ANIMATION::FACE_RIGHT));

	keyMap.push_back(CControllerKeySet(keyFamiliarShoot, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::ACTIVATE_ABILLITY));
	keyMap.push_back(CControllerKeySet(keyFamiliarNext, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::NEXT));
	keyMap.push_back(CControllerKeySet(keyFamiliarPrev, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::PREVIOUS));

	keyMap.push_back(CControllerKeySet(keyJump, KS_PRESS, COMMAND::COMPONENT::MOVE::JUMP));
	auto controllerComponent = CController::create(object, keyMap);
	componentBoard.push_back(controllerComponent);
	auto climbState = new StateClimb(object, componentBoard);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::CLIMB, climbState));
	keyMap.clear();
	componentBoard.clear();
#pragma endregion

#pragma region StateJump
	keyMap.push_back(CControllerKeySet(keyUp,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyUp,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));

	keyMap.push_back(CControllerKeySet(keyLeft,	KS_PRESS,	COMMAND::COMPONENT::MOVE::LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_PRESS,	COMMAND::COMPONENT::MOVE::RIGHT));
	keyMap.push_back(CControllerKeySet(keyLeft,	KS_DOWN,	COMMAND::COMPONENT::MOVE::LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_DOWN,	COMMAND::COMPONENT::MOVE::RIGHT));

	keyMap.push_back(CControllerKeySet(keyFamiliarShoot, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::ACTIVATE_ABILLITY));
	keyMap.push_back(CControllerKeySet(keyFamiliarNext, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::NEXT));
	keyMap.push_back(CControllerKeySet(keyFamiliarPrev, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::PREVIOUS));

	controllerComponent = CController::create(object, keyMap);
	componentBoard.push_back(controllerComponent);
	auto jumpState = new StateJump(object, componentBoard);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::JUMP, jumpState));
	keyMap.clear();
	componentBoard.clear();
#pragma endregion

#pragma region StateMove
	keyMap.push_back(CControllerKeySet(keyUp,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyUp,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));

	keyMap.push_back(CControllerKeySet(keyLeft, KS_DOWN, COMMAND::COMPONENT::MOVE::LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_DOWN, COMMAND::COMPONENT::MOVE::RIGHT));

	keyMap.push_back(CControllerKeySet(keyFamiliarShoot, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::ACTIVATE_ABILLITY));
	keyMap.push_back(CControllerKeySet(keyFamiliarNext, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::NEXT));
	keyMap.push_back(CControllerKeySet(keyFamiliarPrev, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::PREVIOUS));

	keyMap.push_back(CControllerKeySet(keyJump, KS_PRESS, COMMAND::COMPONENT::MOVE::JUMP));
	keyMap.push_back(CControllerKeySet(keyShoot, KS_PRESS, COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));

	controllerComponent = CController::create(object, keyMap);
	componentBoard.push_back(controllerComponent);
	auto moveState = new StateMove(object, componentBoard);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::MOVE, moveState));
	keyMap.clear();
	componentBoard.clear();
#pragma endregion

#pragma region StateStand
	keyMap.push_back(CControllerKeySet(keyUp,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyUp,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_PRESS,	COMMAND::ENTITY::CLIMB_START));
	keyMap.push_back(CControllerKeySet(keyDown,	KS_DOWN,	COMMAND::ENTITY::CLIMB_START));

	keyMap.push_back(CControllerKeySet(keyLeft,	KS_PRESS,	COMMAND::COMPONENT::MOVE::LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_PRESS,	COMMAND::COMPONENT::MOVE::RIGHT));
	keyMap.push_back(CControllerKeySet(keyLeft,	KS_DOWN,	COMMAND::COMPONENT::MOVE::LEFT));
	keyMap.push_back(CControllerKeySet(keyRight, KS_DOWN,	COMMAND::COMPONENT::MOVE::RIGHT));

	keyMap.push_back(CControllerKeySet(keyFamiliarShoot, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::ACTIVATE_ABILLITY));
	keyMap.push_back(CControllerKeySet(keyFamiliarNext, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::NEXT));
	keyMap.push_back(CControllerKeySet(keyFamiliarPrev, KS_PRESS, COMMAND::COMPONENT::FAMILIAR::PREVIOUS));

	keyMap.push_back(CControllerKeySet(keyJump, KS_PRESS, COMMAND::COMPONENT::MOVE::JUMP));
	keyMap.push_back(CControllerKeySet(keyShoot, KS_RELEASE, COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));
	
	controllerComponent = CController::create(object, keyMap);
	componentBoard.push_back(controllerComponent);
	StandState* standState = new StandState(object, componentBoard);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::STAND, standState));
	keyMap.clear();
	componentBoard.clear();
#pragma endregion

#pragma region StateHit
	auto hitState = new StateHit(object, STATE_HIT_DURATION, STATE_HIT_LOST_CONTROL_TIME);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::HIT, hitState));
#pragma endregion

#pragma region StateDie
	auto dieState = new StateDie(object);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::DIE, dieState));
#pragma endregion

#pragma region StateAttack
	StateAttack* attackState = new StateAttack(object);
	stateMap.insert(std::pair<int, State*>(STATE_INDEX::ATTACK, attackState));
#pragma endregion

	auto state = CState::addComponentTo(object, stateMap);
	state->commandProcess((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND), 0.0f);

#pragma endregion
	
	return object;
}
#pragma endregion

#pragma region Create Reactors
EObject* SFactory::create_REACTOR_PORTAL_1()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Portal//Portal_1");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setContentSize(Size(100, 100));
	l_Object->setName(ID::REACTOR_PORTAL_1);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_PORTAL_2()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Portal//Portal_2");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setContentSize(Size(100, 100));
	l_Object->setName(ID::REACTOR_PORTAL_1);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_GUIDE_UP()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Guide//Guide Up");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::REACTOR_GUIDE_UP);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_GUIDE_RIGHT()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Guide//Guide Right");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::REACTOR_GUIDE_RIGHT);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_GUIDE_JUMP()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Guide//Jump Button");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::REACTOR_GUIDE_JUMP);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_GUIDE_SHOOT()
{
	EObject* l_Object = nullptr;
	static const char* path = RESOURCE_PATH_REACTOR("Guide//Guide Shoot");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::REACTOR_GUIDE_SHOOT);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_REACTOR_SPRING()
{
	EObject* object = nullptr;
	b2Body* body = nullptr;

	static int categoryBitmask = BITMASK::OBJECT::OBSTACLE;
	static int filterBitmask = BITMASK::OBJECT::BOTH_ALLY;

	static b2Vec2 bouncingVelo(0.0f, 45.0f);
	static int	bodyWidth = 40.0f;
	static int	bodyHeight = 70.0f;
	static int	bodyPosFixY = 70;
	static const char* path = RESOURCE_PATH_REACTOR("spring_stone");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::REACTOR_SPRING);
	// =====================================================================

	// Set bitmask and create body==========================================
	b2Filter collideBitmask;
	collideBitmask.categoryBits = categoryBitmask;
	collideBitmask.maskBits = filterBitmask;

	body = SFactory::createBody(object,
		object->getPositionX(),
		object->getPositionY() + bodyPosFixY,
		bodyHeight,
		bodyWidth,
		PHYSIC_MATERIAL_OBSTACLE,
		collideBitmask, b2BodyType::b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	auto action = CallFunc::create(std::bind(
	[](EObject* object, b2Vec2 velocity)
	{
		for (EObject* toucher : object->getLinkedObjects())
		{
			auto body = (b2Body*)toucher->getUserData();
			if (body != nullptr)
			{
				body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, velocity.y));
			}
		}

		object->removeAllLinkedObject();
	}
	, object, bouncingVelo));

	CAnimation::addComponentTo(object, path);
	CReact::addComponentTo(object, action);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::SPRING::STRAIGHT));
	// =====================================================================

	return object;
}

EObject* SFactory::create_REACTOR_SPRING_UP_RIGHT()
{
	EObject* object = nullptr;
	b2Body* body = nullptr;

	static int categoryBitmask = BITMASK::OBJECT::OBSTACLE;
	static int filterBitmask = BITMASK::OBJECT::BOTH_ALLY;

	static b2Vec2 bouncingVelo(32.0f, 32.0f);
	static int	bodyWidth = 60.0f;
	static int	bodyHeight = 70.0f;
	static const char* path = RESOURCE_PATH_REACTOR("spring_stone");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::REACTOR_SPRING);
	// =====================================================================

	// Set bitmask and create body==========================================
	b2Filter collideBitmask;
	collideBitmask.categoryBits = categoryBitmask;
	collideBitmask.maskBits = filterBitmask;

	body = SFactory::createBody(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		PHYSIC_MATERIAL_OBSTACLE,
		collideBitmask, b2BodyType::b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	auto action = CallFunc::create(std::bind(
		[](EObject* object, b2Vec2 velocity)
	{
		for (EObject* toucher : object->getLinkedObjects())
		{
			auto body = (b2Body*)toucher->getUserData();
			if (body != nullptr)
			{
				body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
			}
		}

		object->removeAllLinkedObject();
	}
	, object, bouncingVelo));

	CAnimation::addComponentTo(object, path);
	CReact::addComponentTo(object, action);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::SPRING::CROSS);
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
	// =====================================================================

	return object;
}

EObject* SFactory::create_REACTOR_SPRING_UP_LEFT()
{
	EObject* object = nullptr;
	b2Body* body = nullptr;

	static int categoryBitmask = BITMASK::OBJECT::OBSTACLE;
	static int filterBitmask = BITMASK::OBJECT::BOTH_ALLY;

	static b2Vec2 bouncingVelo(-32.0f, 32.0f);
	static int	bodyWidth = 60.0f;
	static int	bodyHeight = 70.0f;
	static const char* path = RESOURCE_PATH_REACTOR("spring_stone");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::REACTOR_SPRING);
	// =====================================================================

	// Set bitmask and create body==========================================
	b2Filter collideBitmask;
	collideBitmask.categoryBits = categoryBitmask;
	collideBitmask.maskBits = filterBitmask;

	body = SFactory::createBody(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		PHYSIC_MATERIAL_OBSTACLE,
		collideBitmask, b2BodyType::b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	auto action = CallFunc::create(std::bind(
		[](EObject* object, b2Vec2 velocity)
	{
		for (EObject* toucher : object->getLinkedObjects())
		{
			auto body = (b2Body*)toucher->getUserData();
			if (body != nullptr)
			{
				body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
			}
		}

		object->removeAllLinkedObject();
	}
	, object, bouncingVelo));

	CAnimation::addComponentTo(object, path);
	CReact::addComponentTo(object, action);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::SPRING::CROSS);
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);
	// =====================================================================

	return object;
}

EObject* SFactory::create_REACTOR_PLATFORM_SPAWNER_Y()
{
	EObject* l_Object = nullptr;
	float respawntime = 2.5f;
	//static const char* path = RESOURCE_PATH_REACTOR("Guide//Guide Shoot");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::REACTOR_PLATFORM_SPAWNER_Y);
	// =====================================================================

	// Set bitmask and create body==========================================
	// =====================================================================

	// Add components=======================================================
	//CAnimation::addComponentTo(l_Object, path);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	//l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));

	auto spawnAction = CallFunc::create(std::bind(
		[](EObject* spawner){
		auto platform = SFactory::create_PLATFORM_DYNAMIC_TYPE2_Y();
		platform->setBodyPosition(spawner->getPositionX(), spawner->getPositionY());
		auto parent = spawner->getParent();
		if (parent != nullptr)
		{
			parent->addChild(platform);
		}
	}
	, l_Object));
	auto waitAction = DelayTime::create(respawntime);
	auto AIAction = Sequence::create(waitAction, spawnAction, nullptr);
	l_Object->runAction(RepeatForever::create(AIAction));
	// =====================================================================

	return l_Object;
}
#pragma endregion

#pragma region Create Platforms
EObject* SFactory::create_PLATFORM_DYNAMIC_TYPE1_X()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;
	float		bodyHeight = 30;
	float		bodyWidth = 132;
	static const char* path = RESOURCE_PATH_PLATFORM_DYNAMIC("Type1");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_DYNAMIC);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::TILE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, bodyHeight, bodyWidth, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CMove::addComponentTo(l_Object);
	CAnimation::addComponentTo(l_Object, path);
	CAIMove::addComponentTo(l_Object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_X);
	l_Object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, bodyHeight / 2)));
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_DYNAMIC_TYPE1_Y()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;
	static const char* path = RESOURCE_PATH_PLATFORM_DYNAMIC("Type1");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_DYNAMIC);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::TILE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 30.0f, 132.0f, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CMove::addComponentTo(l_Object);
	CAnimation::addComponentTo(l_Object, path);
	CAIMove::addComponentTo(l_Object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_Y);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	//l_Object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, bodyHeight / 2)));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_DYNAMIC_TYPE2_X()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;
	static const char* path = RESOURCE_PATH_PLATFORM_DYNAMIC("Type2");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_DYNAMIC);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::TILE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 30.0f, 132.0f, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CMove::addComponentTo(l_Object);
	CAnimation::addComponentTo(l_Object, path);
	CAIMove::addComponentTo(l_Object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_X);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_DYNAMIC_TYPE2_Y()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;
	float		bodyHeight = 30;
	float		bodyWidth = 132;

	static const char* path = RESOURCE_PATH_PLATFORM_DYNAMIC("Type1");

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_DYNAMIC);
	SFactory::setDestroyOnTime(l_Object,30.0f);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::TILE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, bodyHeight, bodyWidth, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CMove::addComponentTo(l_Object, b2Vec2(0, 2.5f));
	CAnimation::addComponentTo(l_Object, path);
	//CAIMove::addComponentTo(l_Object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_Y);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	l_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	l_Object->pushCommand(COMMAND::COMPONENT::MOVE::DOWN);
	l_Object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, bodyHeight / 2)));
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_STATIC()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_STATIC);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::TILE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 1.0f, 1.0f, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_POLYGON()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_POLYGON);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 1.0f, 1.0f, PHYSIC_MATERIAL_PLATFORM, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_LADDER()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_LADDER);
	// =====================================================================

	// Set bitmask and create body==========================================		
	collideBitmask.categoryBits = BITMASK::OBJECT::LADDER_ROPE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createSensor(l_Object, 0, 0, 1.0f, 1.0f, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_OBSTACLE()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_OBSTACLE);
	// =====================================================================

	// Set bitmask and create body==========================================		
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 1.0f, 1.0f, PHYSIC_MATERIAL_OBSTACLE, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_ROPE()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_ROPE);
	// =====================================================================

	// Set bitmask and create body==========================================		
	collideBitmask.categoryBits = BITMASK::OBJECT::LADDER_ROPE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createSensor(l_Object, 0, 0, 1.0f, 1.0f, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}

EObject* SFactory::create_PLATFORM_HITCH()
{
	EObject*	l_Object;
	b2Filter	collideBitmask;

	// Create entity and set it's name======================================
	l_Object = EObject::create();
	l_Object->setName(ID::PLATFORM_HITCH);
	// =====================================================================

	// Set bitmask and create body==========================================		
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::BOTH_ALLY | BITMASK::OBJECT::GENERAL;

	SFactory::createBody(l_Object, 0, 0, 1.0f, 1.0f, PHYSIC_MATERIAL_OBSTACLE, collideBitmask, b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return l_Object;
}
#pragma endregion

#pragma region Create Mobs
EObject* SFactory::create_MOB_COPPER_DRAKE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.0f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 300;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorWidth = 450;
	static float	sensorHeight = 20;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 62;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 0.5f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("Copper Drake");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_COPPER_DRAKE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_MALADY()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(5.0f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 300;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorRadius = 300;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 62;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 0.5f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("Malady");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_MALADY);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET
		^ BITMASK::OBJECT::TILE ^ BITMASK::OBJECT::OBSTACLE;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetGravityScale(0.0f);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorRadius, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase, CAIMOVE_MODE_BOTH_AXISTS);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_STRIGE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(4.0f, 4.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 200;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorRadius = 150;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 62;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 0.5f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("Strige");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_STRIGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET 
		^ BITMASK::OBJECT::TILE ^ BITMASK::OBJECT::OBSTACLE;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetGravityScale(0.0f);
	//body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorRadius, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity, b2Vec2(velocity), B2VEC2_ZERO);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, true, Rect::ZERO, true, CAIMOVE_MODE_BOTH_AXISTS);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_CURSE_EYE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.5f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 375;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorWidth = 500;
	static float	sensorHeight = 20;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 42;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 14;
	static float	bodyHeight = 49;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("CurseEye");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_CURSE_EYE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorHeight,sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_DARK_DRAKE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.0f, 5.0f);
	static float	moveRate = 0.8f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 375;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static int		sensorRange = 100;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 85;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("Dark Drake");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_DARK_DRAKE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorRange, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_DRAGON()
{
	CCLOG("Unable to create Dragon");
	return nullptr;
}

EObject* SFactory::create_MOB_EVIL_EYE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.0f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 300;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorWidth = 500;
	static float	sensorHeight = 20;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 35;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 14;
	static float	bodyHeight = 49;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("EvilEye");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_EVIL_EYE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_FAUST()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(5.0f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 4000;
	static int		maxMP = 50;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static int		sensorRange = 1000;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 97;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 75;
	static float	bodyHeight = 160;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime	= 4.0f;
	static const char* bulletID		= ID::BULLET_BANANA_SET;
	static const char* path = RESOURCE_PATH_MOB("Faust");
	static b2Vec2	shootSpot(-17, 133);

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_FAUST);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorRange, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::addComponentTo(object, bulletID, shootSpot);
	CMove::addComponentTo(object, velocity);
	CHPBar::addComponentTo(object, Vec2(-bodyWidth/2, bodyHeight + 50), Size(bodyWidth * 1.5f, bodyHeight));

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, comAIAttack);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	SFactory::setCalledMethodAtCommand(
		object,
		&SFactory::calledAtBossDie,
		(int)COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_HORNY_MUSHROOM()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(4.0f, 0.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 200;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 400;
	static float	sensorHeight	= 10;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 24;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 34;
	static float	bodyHeight		= 52;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path			= RESOURCE_PATH_MOB("HornyMushroom");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_HORNY_MUSHROOM);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;
	
	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::  addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_ICE_DRAKE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.0f, 5.0f);
	static float	moveRate = 0.8f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 375;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static int		sensorRange = 100;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 80;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path = RESOURCE_PATH_MOB("Ice Drake");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_ICE_DRAKE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorRange, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::		addComponentTo(object, bulletID);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_KING_SMILE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(5.5f, 0.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 2200;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static int		sensorRange		= 1000;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 60;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 154;
	static float	bodyHeight		= 110;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime	= 2.5f;
	static b2Vec2	shootSpot		= b2Vec2(0, 0);
	static const char* bulletID		= ID::BULLET_EARTH_WAKE;
	static const char* path			= RESOURCE_PATH_MOB("KingSmile");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_KING_SMILE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorRange, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::		addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);
	CHPBar::		addComponentTo(object, Vec2(-bodyWidth/2, bodyHeight + 50), Size(bodyWidth,bodyHeight));

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, comAIAttack);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	SFactory::setCalledMethodAtCommand(
		object,
		&SFactory::calledAtBossDie,
		(int)COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_LUPIN()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(4.0f, 5.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 125;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 600;
	static float	sensorHeight	= 70;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 45;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 37;
	static float	bodyHeight		= 54;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime	= 3.0f;
	static b2Vec2	shootSpot		= b2Vec2(bodyWidth, 30);
	static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path			= "Monster//Lupin";

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_LUPIN);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::		addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, comAIAttack);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_PIG()
{
	
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(4.0f, 5.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 200;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 500;
	static float	sensorHeight	= 40;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 31;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 58;
	static float	bodyHeight		= 45;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime	= 5.0f;
	static b2Vec2	shootSpot		= b2Vec2(bodyWidth, bodyHeight);
	static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path			= "Monster//Pig";

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_PIG);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::		addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_RED_DRAKE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity = b2Vec2(6.0f, 5.0f);
	static float	moveRate = 1.0f;
	static float	moveRound = 2.0f;
	static bool		randomMove = true;
	static bool		autoChase = true;
	static int		maxHP = 300;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	static float	sensorWidth = 800;
	static float	sensorHeight = 40;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 71;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 43;
	static float	bodyHeight = 69;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime = 3.0f;
	static b2Vec2	shootSpot = b2Vec2(59, 31);
	static const char* bulletID = ID::BULLET_RED_DRAKE;
	static const char* path = RESOURCE_PATH_MOB("Red Drake");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_RED_DRAKE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CSensor::addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::		addComponentTo(object, bulletID, shootSpot);
	CMove::addComponentTo(object, velocity);

	auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, comAIAttack);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_RIBBON()
{
	
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(5.0f, 5.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 250;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 500;
	static float	sensorHeight	= 40;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 37;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 59;
	static float	bodyHeight		= 42;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path			= RESOURCE_PATH_MOB("Ribbon");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_RIBBON);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::  addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_SNAIL()
{
	
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(3.0f, 0.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.0f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 125;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 400;
	static float	sensorHeight	= 10;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 19;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 37;
	static float	bodyHeight		= 50;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	//static float	reAttackTime	= 5.0f;
	//static const char* bulletID		= ID::BULLET_BANANA;
	static const char* path			= RESOURCE_PATH_MOB("Snail");

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_SNAIL);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	//CAttack::  addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_ZOMBIE_LUPIN()
{
	
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	static b2Vec2	velocity		= b2Vec2(4.0f, 5.0f);
	static float	moveRate		= 1.0f;
	static float	moveRound		= 2.5f;
	static bool		randomMove		= true;
	static bool		autoChase		= true;
	static int		maxHP			= 200;
	static int		maxMP			= 0;
	static int		mp				= maxMP;
	static int		hp				= maxHP;
	static float	sensorWidth		= 600;
	static float	sensorHeight	= 70;
	static int		sensorBit		= BITMASK::OBJECT::PLAYER;
	static int		touchDamage		= 53;
	static int		touchBit		= BITMASK::OBJECT::PLAYER;
	static float	bodyWidth		= 37;
	static float	bodyHeight		= 52;
	static float	hitFlashTime	= 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime	= 3.0f;
	static const char* bulletID		= ID::BULLET_GREEN_BANANA;
	static const char* path			= "Monster//ZombieLupin";
	static b2Vec2	shootSpot = b2Vec2(bodyWidth, bodyHeight);

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_ZOMBIE_LUPIN);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::	addComponentTo(object, path);
	CSensor::		addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::		addComponentTo(object, touchDamage, touchBit);
	CStats::		addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::		addComponentTo(object, bulletID, shootSpot);
	CMove::			addComponentTo(object, velocity);

	auto comAIMove		= CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	auto comAIAttack	= CAIAttack::createComponent(object, reAttackTime);
	State* stateHit		= new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, comAIMove, comAIAttack);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_STATIC_LUPIN_LEFT()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	//static b2Vec2	velocity = b2Vec2(4.0f, 5.0f);
	//static float	moveRate = 1.0f;
	//static float	moveRound = 2.0f;
	//static bool		randomMove = true;
	//static bool		autoChase = true;
	static int		maxHP = 100;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	//static float	sensorWidth = 600;
	//static float	sensorHeight = 70;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 45;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 37;
	static float	bodyHeight = 54;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime = 1.5f;
	static b2Vec2	shootSpot = b2Vec2(bodyWidth, bodyHeight);
	static const char* bulletID = ID::BULLET_BANANA;
	static const char* path = "Monster//Lupin";

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_STATIC_LUPIN_LEFT);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	//CSensor::addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::addComponentTo(object, bulletID, shootSpot);
	//CMove::addComponentTo(object, velocity);

	//auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack = CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, nullptr, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);

	auto attackAction = CallFunc::create(std::bind(&EObject::pushCommand, object, COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));
	auto waitAction = DelayTime::create(reAttackTime);
	auto AIAction = Sequence::create(waitAction, attackAction, nullptr);
	object->runAction(RepeatForever::create(AIAction));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);
	// =====================================================================

	return object;
#pragma endregion
}

EObject* SFactory::create_MOB_STATIC_LUPIN_RIGHT()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;

	//static b2Vec2	velocity = b2Vec2(4.0f, 5.0f);
	//static float	moveRate = 1.0f;
	//static float	moveRound = 2.0f;
	//static bool		randomMove = true;
	//static bool		autoChase = true;
	static int		maxHP = 100;
	static int		maxMP = 0;
	static int		mp = maxMP;
	static int		hp = maxHP;
	//static float	sensorWidth = 600;
	//static float	sensorHeight = 70;
	static int		sensorBit = BITMASK::OBJECT::PLAYER;
	static int		touchDamage = 45;
	static int		touchBit = BITMASK::OBJECT::PLAYER;
	static float	bodyWidth = 37;
	static float	bodyHeight = 54;
	static float	hitFlashTime = 1.0f;
	static float	hitLostCtrlTime = 1.0f;
	static float	reAttackTime = 1.5f;
	static b2Vec2	shootSpot = b2Vec2(bodyWidth, bodyHeight);
	static const char* bulletID = ID::BULLET_BANANA;
	static const char* path = "Monster//Lupin";

#pragma region Source Code
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::MOB_STATIC_LUPIN_LEFT);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::MOB ^ BITMASK::OBJECT::MOB_BULLET;

	body = SFactory::createBody(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, PHYSIC_MATERIAL_MOB, collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	//CSensor::addComponentTo(object, sensorHeight, sensorWidth, sensorBit);
	CDamage::addComponentTo(object, touchDamage, touchBit);
	CStats::addComponentTo(object, maxHP, maxMP, hp, mp);
	CAttack::addComponentTo(object, bulletID, shootSpot);
	//CMove::addComponentTo(object, velocity);

	//auto comAIMove = CAIMove::createComponent(object, moveRate, moveRound, randomMove, Rect::ZERO, autoChase);
	//auto comAIAttack = CAIAttack::createComponent(object, reAttackTime);
	State* stateHit = new StateHit(object, hitFlashTime, hitLostCtrlTime);

	addStateComponent(object, stateHit, true, nullptr, nullptr);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::DIE);

	auto attackAction = CallFunc::create(std::bind(&EObject::pushCommand, object, COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));
	auto waitAction = DelayTime::create(reAttackTime);
	auto AIAction = Sequence::create(waitAction, attackAction, nullptr);
	object->runAction(RepeatForever::create(AIAction));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
	// =====================================================================

	return object;
#pragma endregion
}
#pragma endregion

#pragma region Create Bullets
EObject* SFactory::create_BULLET_DARK_BALL()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth	= 30;
	static float		bodyHeight	= 30;
	static b2Vec2		velocity	(50.0f, 0.0f);	// move straight 5 m/s
	static int			hpDamage	= 10;
	static int			mpDamage	= 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(20, 0);

	static const char* path = RESOURCE_PATH_BULLET("DarkBall");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_DARK_BALL);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object, 
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight, 
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_END, true, COMMAND::ENTITY::ACTIVATE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_START, true, COMMAND::ENTITY::DEACTIVATE);

	effect = SFactoryEffect::create(path, ANIMATION_INDEX::BULLET::EFFECT_START, true);
	effectAnimationComponent = (CAnimation*)effect->getChildByTag(TAG::COMPONENT::ANIMATION);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	object->pushCommand(COMMAND::ENTITY::DEACTIVATE);
	object->setActiveSlow(true, effectAnimationComponent->getPreventingAnimationDuration());
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_GREEN_BANANA()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth	= 30;
	static float		bodyHeight	= 30;
	static b2Vec2		velocity	(25.0f, 5.0f);	// move straight 5 m/s
	static int			hpDamage	= 60;
	static int			mpDamage	= 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(0, 0);

	static const char* path = RESOURCE_PATH_BULLET("Banana");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_GREEN_BANANA);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);

	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB;
	collideBitmask.maskBits = BITMASK::OBJECT::TILE | BITMASK::OBJECT::OBSTACLE;

	body = SFactory::createBody(object, 
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight, 
		bodyWidth,
		PHYSIC_MATERIAL_BULLET,
		collideBitmask, b2BodyType::b2_dynamicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, velocity);
	CDamage::addComponentTo(object, hpDamage, 
		BITMASK::OBJECT::BOTH_ALLY | 
		BITMASK::OBJECT::TILE |
		BITMASK::OBJECT::OBSTACLE, 
		damageTimes);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_BANANA()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth = 30;
	static float		bodyHeight = 30;
	static b2Vec2		velocity(10.0f, 5.0f);	// move straight 5 m/s
	static int			hpDamage = 40;
	static int			mpDamage = 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(32, 0);

	static const char* path = RESOURCE_PATH_BULLET("Banana");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_BANANA);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	SFactory::setDestroyOnTime(object, 3.0f);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	//CSensor::addComponentTo(object, 200, BITMASK::OBJECT::MOB);
	//CAIMove::addComponentTo(object, 1.0f, 1.0f, false, MY_ZERO_RECT, true, CAIMOVE_MODE_BOTH_AXISTS);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_POISON_BOOM()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth	= 30;
	static float		bodyHeight	= 30;
	static b2Vec2		velocity	(50.0f, 5.0f);	// move straight 5 m/s
	static int			hpDamage	= 100;
	static int			mpDamage	= 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(32, 0);

	static const char* path = RESOURCE_PATH_BULLET("PoisonBomb");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_POISON_BOOM);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object, 
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight, 
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	//CSensor::addComponentTo(object, 200, BITMASK::OBJECT::MOB);
	//CAIMove::addComponentTo(object, 1.0f, 1.0f, false, MY_ZERO_RECT, true, CAIMOVE_MODE_BOTH_AXISTS);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_DARK_BAT()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth = 30;
	static float		bodyHeight = 30;
	static b2Vec2		velocity(20.0f, 5.0f);	// move straight 5 m/s
	static int			hpDamage = 100;
	static int			mpDamage = 0;
	static unsigned int	damageTimes = 1;
	static float		liveTime = 20.0f;
	static Vec2			startPosfix = Vec2(0, 0);

	static const char* path = RESOURCE_PATH_BULLET("dark_bat");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_POISON_BOOM);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnTime(object, liveTime);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	CSensor::addComponentTo(object, 500, BITMASK::OBJECT::MOB);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, MY_ZERO_RECT, true, CAIMOVE_MODE_BOTH_AXISTS);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_BANANA_SET()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth	= 30;
	static float		bodyHeight	= 30;
	static b2Vec2		velocity	(8.5f, 8.0f);	// move straight 5 m/s
	static int			hpDamage	= 60;
	static int			mpDamage	= 0;
	static unsigned int	damageTimes = 1;
	static float		lifeTime	= 4.0f;
	static Vec2			startPosfix = Vec2(32, 0);

	static const char* path = RESOURCE_PATH_BULLET("Faust Attack");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_BANANA_SET);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	SFactory::setDestroyOnTime(object, lifeTime);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object, 
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight, 
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	CSensor::addComponentTo(object, 1000, BITMASK::OBJECT::PLAYER);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, MY_ZERO_RECT, true, CAIMOVE_MODE_BOTH_AXISTS);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_EARTH_WAKE()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth = 2000;
	static float		bodyHeight = 30;
	//static b2Vec2		velocity(300.0f, 0.0f);	// move straight 5 m/s
	static int			hpDamage = 72;
	static int			mpDamage = 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(20, 0);

	static const char* path = RESOURCE_PATH_BULLET("Earth Wake");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_EARTH_WAKE);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	SFactory::setDestroyOnTime(object, 1.1f);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	//CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	// =====================================================================

	// Create effects=======================================================
	/*effect = SFactoryEffect::create(path, ANIMATION_INDEX::BULLET::HIT, true);
	SFactory::setEffectAtCommand(object, effect, COMMAND::ENTITY::MAKE_DAMAGE);*/

	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_END, true, COMMAND::ENTITY::ACTIVATE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_START, true, COMMAND::ENTITY::DEACTIVATE);

	//effectAnimationComponent = (CAnimation*)effect->getChildByTag(TAG::COMPONENT::ANIMATION);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::ENTITY::DEACTIVATE);
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	object->setActiveSlow(true, 1.0f);
	// =====================================================================

	// Dung 30/6, shake screen
	GamePlayScene::get()->runAction(Sequence::create(DelayTime::create(0.2f), CCShake::actionWithDuration(1.0f, 10.0f), nullptr));
	//
	return object;
}

EObject* SFactory::create_BULLET_FIRE_BALL()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth = 100;
	static float		bodyHeight = 50;
	static b2Vec2		velocity(50.0f, 0.0f);	// move straight 5 m/s
	static int			hpDamage = 125;
	static int			mpDamage = 0;
	static unsigned int	damageTimes = 4096;
	static Vec2			startPosfix = Vec2(120, 0);

	static const char* path = RESOURCE_PATH_BULLET("FireBall");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_FIRE_BALL);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes, 3.0f);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT,			true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_END,		true, COMMAND::ENTITY::ACTIVATE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_START,	true, COMMAND::ENTITY::DEACTIVATE);

	effect = SFactoryEffect::create(path, ANIMATION_INDEX::BULLET::EFFECT_START, true);
	effectAnimationComponent = (CAnimation*)effect->getChildByTag(TAG::COMPONENT::ANIMATION);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	object->pushCommand(COMMAND::ENTITY::DEACTIVATE);
	object->setActiveSlow(true, effectAnimationComponent->getPreventingAnimationDuration());
	// =====================================================================

	return object;
}

EObject* SFactory::create_BULLET_RED_DRAKE()
{
	EObject*			object = nullptr;
	EObject*			effect = nullptr;
	CAnimation*			effectAnimationComponent = nullptr;
	b2Filter			collideBitmask;
	b2Body*				body = nullptr;
	static float		bodyWidth = 40;
	static float		bodyHeight = 40;
	static b2Vec2		velocity(10.0f, 0.0f);	// move straight 5 m/s
	static int			hpDamage = 10;
	static int			mpDamage = 0;
	static unsigned int	damageTimes = 1;
	static Vec2			startPosfix = Vec2(20, 0);

	static const char* path = RESOURCE_PATH_BULLET("Red Drake Attack");

	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::BULLET_RED_DRAKE);
	object->setPosition(startPosfix);

	SFactory::setDestroyOnOutOfViewPort(object);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = 0;
	collideBitmask.maskBits = 0;

	body = SFactory::createSensor(object,
		object->getPositionX(),
		object->getPositionY(),
		bodyHeight,
		bodyWidth,
		collideBitmask, b2BodyType::b2_kinematicBody);
	body->SetSleepingAllowed(false);
	body->SetBullet(true);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CMove::addComponentTo(object, velocity, B2VEC2_ZERO);
	CDamage::addComponentTo(object, hpDamage, BITMASK::OBJECT::BOTH_ALLY, damageTimes);
	// =====================================================================

	// Create effects=======================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::HIT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_END, true, COMMAND::ENTITY::ACTIVATE);
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::BULLET::EFFECT_START, true, COMMAND::ENTITY::DEACTIVATE);

	effect = SFactoryEffect::create(path, ANIMATION_INDEX::BULLET::EFFECT_START, true);
	effectAnimationComponent = (CAnimation*)effect->getChildByTag(TAG::COMPONENT::ANIMATION);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::BULLET::BALL);
	object->pushCommand(COMMAND::ENTITY::DEACTIVATE);
	object->setActiveSlow(true, effectAnimationComponent->getPreventingAnimationDuration());
	// =====================================================================

	return object;
}
#pragma endregion

#pragma region Create Trap
EObject* SFactory::create_TRAP_HOLE()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 10000;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SHURIKEN_TYPE1_X);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CDamage::addComponentTo(object, 999999, -1, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// =====================================================================

	return object;
}

EObject* SFactory::create_TRAP_SHURIKEN_TYPE1_X()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SHURIKEN_TYPE1_X);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB_BULLET;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_X);
	CMove::addComponentTo(object, b2Vec2(0.0f, 5.0f));
	//CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	CDamage::addComponentTo(object, 30, -1, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::TRAP::TRAP6);
	// =====================================================================

	return object;
}

EObject* SFactory::create_TRAP_SHURIKEN_TYPE1_Y()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SHURIKEN_TYPE1_Y);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB_BULLET;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER | BITMASK::OBJECT::TRAP;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	CAnimation::addComponentTo(object, path);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_Y);
	CMove::addComponentTo(object, b2Vec2(0.0f, 5.0f));
	CDamage::addComponentTo(object, 30, BITMASK::OBJECT::BOTH_ALLY, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::TRAP::TRAP6);
	// =====================================================================

	return object;
}

EObject* SFactory::create_TRAP_SHURIKEN_TYPE2_X()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SHURIKEN_TYPE2_X);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::GENERAL;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER | BITMASK::OBJECT::MOB;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	//CAnimation::addComponentTo(object, path);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_Y);
	CMove::addComponentTo(object, b2Vec2(0.0f, 5.0f));
	CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::MOVE);
	// =====================================================================

	return object;
}

EObject* SFactory::create_TRAP_SHURIKEN_TYPE2_Y()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SHURIKEN_TYPE2_Y);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::GENERAL;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER | BITMASK::OBJECT::MOB;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	//CAnimation::addComponentTo(object, path);
	CAIMove::addComponentTo(object, 1.0f, 1.0f, false, Rect::ZERO, false, CAIMOVE_MODE_Y);
	CMove::addComponentTo(object, b2Vec2(0.0f, 5.0f));
	CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	object->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::MOVE);
	// =====================================================================

	return object;
}

EObject* SFactory::create_TRAP_SPIKE_50()
{
	EObject*	object = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 25;
	float		bodyHeight = 12;

	static const char* path = RESOURCE_PATH_TRAP;
	// Create entity and set it's name======================================
	object = EObject::create();
	object->setName(ID::TRAP_SPIKE_50);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::MOB_BULLET;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, object->getPositionX(), object->getPositionY(),
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_staticBody);
	// =====================================================================

	// Add components=======================================================
	//CAnimation::addComponentTo(object, path);
	CDamage::addComponentTo(object, 50, BITMASK::OBJECT::BOTH_ALLY, 4096);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	// object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	// =====================================================================

	return object;
}
#pragma endregion

#pragma region Create Item
EObject* SFactory::create_ITEM_BONUS_HP()
{
	EObject*	object = nullptr;
	EObject*	effect = nullptr;
	EObject*	actEffect = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_ITEM;
	// Create entity and set it's name======================================
	object = EObject::create();
	effect = EObject::create();
	object->setName(ID::ITEM_BONUS_HP);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, 0, 0,
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	// WARNING THING
	auto image = CAnimation::addComponentTo(effect, path);
	auto aura = CAnimation::addComponentTo(effect, path);
	//CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	CDamage::addComponentTo(object, 27, 0, 27, 0);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	//object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	image->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::BONUS_HP, 0.0f);
	aura->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::BONUS_HP_EFFECT, 0.0f);
	auto actionMoveUp	= EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, 15.0f, 0.0f)), 2.0f);
	auto actionMoveDown = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, -15.0f, 0.0f)), 2.0f);
	auto floatAction = RepeatForever::create(Sequence::create(actionMoveUp, actionMoveDown, nullptr));
	effect->runAction(floatAction);
	// =====================================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::ITEM::BONUS_HP_ACT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	object->addChild(effect);
	return object;
}

EObject* SFactory::create_ITEM_RESTORE_HP_100()
{
	EObject*	object = nullptr;
	EObject*	effect = nullptr;
	EObject*	actEffect = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_ITEM;
	// Create entity and set it's name======================================
	object = EObject::create();
	effect = EObject::create();
	object->setName(ID::ITEM_RESTORE_HP_100);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, 0, 0,
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	// WARNING THING
	auto image = CAnimation::addComponentTo(effect, path);
	auto aura = CAnimation::addComponentTo(effect, path);
	//CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	CDamage::addComponentTo(object, 0, 0, 100, 0);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	//object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	image->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::HP1, 0.0f);
	aura->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::HP1_EFFECT, 0.0f);
	auto actionMoveUp = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, 15.0f, 0.0f)), 2.0f);
	auto actionMoveDown = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, -15.0f, 0.0f)), 2.0f);
	auto floatAction = RepeatForever::create(Sequence::create(actionMoveUp, actionMoveDown, nullptr));
	effect->runAction(floatAction);
	// =====================================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::ITEM::HP1_ACT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	object->addChild(effect);
	return object;
}

EObject* SFactory::create_ITEM_RESTORE_HP_250()
{
	EObject*	object = nullptr;
	EObject*	effect = nullptr;
	EObject*	actEffect = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_ITEM;
	// Create entity and set it's name======================================
	object = EObject::create();
	effect = EObject::create();
	object->setName(ID::ITEM_RESTORE_HP_250);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, 0, 0,
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	// WARNING THING
	auto image = CAnimation::addComponentTo(effect, path);
	auto aura = CAnimation::addComponentTo(effect, path);
	//CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	CDamage::addComponentTo(object, 0, 0, 250, 0);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	//object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	image->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::HP2, 0.0f);
	aura->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::HP2_EFFECT, 0.0f);
	auto actionMoveUp = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, 15.0f, 0.0f)), 2.0f);
	auto actionMoveDown = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, -15.0f, 0.0f)), 2.0f);
	auto floatAction = RepeatForever::create(Sequence::create(actionMoveUp, actionMoveDown, nullptr));
	effect->runAction(floatAction);
	// =====================================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::ITEM::HP2_ACT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	object->addChild(effect);
	return object;
}

EObject* SFactory::create_ITEM_RESTORE_MP_50()
{
	EObject*	object = nullptr;
	EObject*	effect = nullptr;
	EObject*	actEffect = nullptr;
	b2Filter	collideBitmask;
	b2Body*		body = nullptr;
	float		bodyWidth = 20;
	float		bodyHeight = 20;

	static const char* path = RESOURCE_PATH_ITEM;
	// Create entity and set it's name======================================
	object = EObject::create();
	effect = EObject::create();
	object->setName(ID::ITEM_RESTORE_MP_50);
	// =====================================================================

	// Set bitmask and create body==========================================
	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;

	body = SFactory::createSensor(object, 0, 0,
		bodyHeight, bodyWidth, collideBitmask, b2BodyType::b2_kinematicBody);
	// =====================================================================

	// Add components=======================================================
	// WARNING THING
	auto image = CAnimation::addComponentTo(effect, path);
	auto aura = CAnimation::addComponentTo(effect, path);
	//CDamage::addComponentTo(object, 10, BITMASK::OBJECT::BOTH_ALLY, 4096);
	CDamage::addComponentTo(object, 0, 0, 0, 50);
	// =====================================================================

	// Push the very first setting (commands / properties / active)=========
	//object->setProperty(TAG::COMPONENT::ANIMATION, UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -bodyHeight / 2)));
	image->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::MP1, 0.0f);
	aura->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ITEM::MP1_EFFECT, 0.0f);
	auto actionMoveUp = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, 15.0f, 0.0f)), 2.0f);
	auto actionMoveDown = EaseInOut::create(MoveBy::create(1.2f, Vec3(0.0f, -15.0f, 0.0f)), 2.0f);
	auto floatAction = RepeatForever::create(Sequence::create(actionMoveUp, actionMoveDown, nullptr));
	effect->runAction(floatAction);
	// =====================================================================
	SFactory::setEffectAtCommand(object, path, ANIMATION_INDEX::ITEM::MP1_ACT, true, COMMAND::ENTITY::MAKE_DAMAGE);
	SFactory::setDestroyOnCommand(object, COMMAND::ENTITY::EXPIRED_DAMAGE);
	object->addChild(effect);
	return object;
}
#pragma endregion

void SFactory::addStateComponent(EObject* object, 
	State* stateHit /*= nullptr*/, 
	bool isDieStateInitialized /*= true*/,
	CBase* AIMoveComponent /*= nullptr*/, 
	CBase* AIAttackComponent /*= nullptr*/)
{
	map<int, State*> result;
	vector<CBase*>	standComponentBoard;
	vector<CBase*>	moveComponentBoard;

	// IMPORTANT: The Move state are initialized right before Attack state for attack direction auto-correction

	// Be able to Move while Stand
	if (object->getChildByTag(TAG::COMPONENT::MOVE) != nullptr)
	{
		// Only have the AI and state of move if it's able to move
		if (AIMoveComponent != nullptr)
		{
			moveComponentBoard.push_back(AIMoveComponent);
			standComponentBoard.push_back(AIMoveComponent);
		}
	}

	// Be able to Attack while Stand or Move
	if (object->getChildByTag(TAG::COMPONENT::ATTACK) != nullptr)
	{

		// Only have the AI and state of attack if it's able to attack
		if (AIAttackComponent != nullptr)
		{
			moveComponentBoard.push_back(AIAttackComponent);
			standComponentBoard.push_back(AIAttackComponent);
		}

		State* attackState = new StateAttack(object);
		result.insert(pair<int, State*>(STATE_INDEX::ATTACK, attackState));
	}

	if (stateHit != nullptr)
	{
		result.insert(pair<int, State*>(STATE_INDEX::HIT, stateHit));

		// The only way to die is to be hit
		if (isDieStateInitialized)
		{
			State* stateDie = new StateDie(object);
			result.insert(pair<int, State*>(STATE_INDEX::DIE, stateDie));
		}
	}

	State* stateStand	= new StandState(object, standComponentBoard);
	State* moveState	= new StateMove(object, moveComponentBoard);
	State* jumpState	= new StateJump(object, moveComponentBoard);
	result.insert(pair<int, State*>(STATE_INDEX::MOVE, moveState));
	result.insert(pair<int, State*>(STATE_INDEX::JUMP, jumpState));
	result.insert(pair<int, State*>(STATE_INDEX::STAND, stateStand));

	auto comState = CState::addComponentTo(object, result);
	comState->commandProcess((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND), 0.0f);
}


void SFactory::setCalledMethodAtCommand(EObject* object, void(*method)(EObject*), int command)
{
	auto action = CallFunc::create(std::bind(method, object));
	action->retain();

	object->setProperty(TAG::COMPONENT::NONE, UPARAM_TYPE_ACTION_PAIR, UParam(command, action));
}

void SFactory::calledAtBossDie(EObject* boss)
{
	int mapIndex = SMapManager::getInstance()->getCrIndex();
	auto l_Familiar = EObject::create();
	b2Filter collideBitmask;
	b2Body* body;

	collideBitmask.categoryBits = BITMASK::OBJECT::OBSTACLE;
	collideBitmask.maskBits = BITMASK::OBJECT::PLAYER;
	body = SFactory::createSensor(l_Familiar, boss->getPositionX(), boss->getPositionY(),
		10, 10, collideBitmask, b2BodyType::b2_kinematicBody);

	switch (mapIndex)
	{
	// King Smile
	case 4:
		CAnimation::addComponentTo(l_Familiar, RESOURCE_PATH_FAMILIAR("FamiliarFire"));
		CReact::addComponentTo(l_Familiar, CallFunc::create(std::bind(
		[](EObject* familiarItem)
		{
			if (familiarItem->getLinkedObjects().size() > 0)
			{
				auto owner = familiarItem->getLinkedObjects().at(0);
				owner->setProperty(TAG::COMPONENT::FAMILIAR, UPARAM_TYPE_EOBJECT, FamiliarFire::create(owner));
				// Dung 3/7 Reset Stats Character
				owner->pushCommand(COMMAND::COMPONENT::STATS::RESET);
				SFactory::addToDestroyer(familiarItem);

				EObject* l_Portal = create_REACTOR_PORTAL_1();
				l_Portal->setPosition(Vec2(420, 320));
				SMapManager::getInstance()->getCrMap()->addChild(l_Portal, Z_ORDER::Map);
			}
		}
		, l_Familiar)));
		break;

	// Faust
	case 9:
		CAnimation::addComponentTo(l_Familiar, RESOURCE_PATH_FAMILIAR("FamiliarDark"));
		CReact::addComponentTo(l_Familiar, CallFunc::create(std::bind(
			[](EObject* familiarItem)
		{
			if (familiarItem->getLinkedObjects().size() > 0)
			{
				auto owner = familiarItem->getLinkedObjects().at(0);
				owner->setProperty(TAG::COMPONENT::FAMILIAR, UPARAM_TYPE_EOBJECT, FamiliarDark::create(owner));
				// Dung 3/7 Reset Stats Character
				owner->pushCommand(COMMAND::COMPONENT::STATS::RESET);
				SFactory::addToDestroyer(familiarItem);

				EObject* l_Portal = create_REACTOR_PORTAL_2();
				SMapManager::getInstance()->getCrMap()->addChild(l_Portal, Z_ORDER::Map);
				l_Portal->setPosition(Vec2(490, 680));
			}
		}
		, l_Familiar)));
		break;
	default:
		break;
	}

	l_Familiar->pushCommand(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND);
	SMapManager::getInstance()->getCrMap()->addChild(l_Familiar, Z_ORDER::Map);
}

void SFactory::calledAtPlayerDie(EObject* player)
{
	if (GameOverScene::get() == nullptr)
		GameOverScene::initialize();
	auto scene = GameOverScene::get();

	SMapManager::getInstance()->switchMap(0);
	player->pushCommand(COMMAND::COMPONENT::STATS::RESET);

	// run
	Director::getInstance()->pushScene(scene);
}

void SFactory::setEffectAtCommand(EObject* object, const char* path, int aniIndex, bool noLoop, int command)
{
	auto action = CallFunc::create(std::bind(&SFactory::addEffect, object, path, aniIndex, noLoop));
	action->retain();

	object->setProperty(TAG::COMPONENT::NONE, UPARAM_TYPE_ACTION_PAIR, UParam(command, action));
}

void SFactory::addEffect(EObject* object, const char* path, int aniIndex, bool noLoop)
{
	auto parent = object->getParent();
	if (parent != nullptr)
	{
		EObject* effect = SFactoryEffect::create(path, aniIndex, noLoop);
		effect->setScaleX(object->getScaleX());
		effect->setPosition(object->getPosition() + effect->getPosition() * effect->getScaleX());

		parent->addChild(effect, Z_ORDER::GAMEWORLD::FrontAllEffect);
	}
}