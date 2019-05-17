// Author: Frederick Tran
// Todo: create the suited entity by given ID

#ifndef __FACTORY_H__
#define __FACTORY_H__

#include "cocos2d.h"
#include <vector>
#include "Box2D/Box2D.h"
#include <map>

#include "Entity/EObject.h"
#include "Component/State.h"

USING_NS_CC;
using namespace std;

class SFactory
{
public:
	static void initialize(b2World* physicsWorld, Layer* gameworld);

	// Additional stuffs to do after create object
	//	+ For Player: do nothing
	//	+ For Platform: have to set entity body size and it's pos as you need.
	//	+ For Mobs: have to set the move range of the mob if it's have one.
	//	+ For Bullet: 
	//		+ Set the filter data to it's body.
	//		+ Push a command to it's own move component for flying direction.
	//		+ Set scale X for bullet equal with the owner.
	//		+ IMPORTANT: bullet->pos = bullet->pos * owner->scale + owner->pos.
	static EObject* create(const char* objectName);

	static	b2Body*	createBody(EObject* object, float posX, float posY,
		float height, float width, PhysicsMaterial material, b2Filter collistionBitmask, b2BodyType bodyType = b2BodyType::b2_dynamicBody, b2Vec2 center = b2Vec2(0.0f, 0.5f));
	static	b2Body*	createSensor(EObject* object, float posX, float posY,
		float height, float width, b2Filter collistionBitmask, b2BodyType bodyType, b2Vec2 center = B2VEC2_ZERO);
	static	b2Body* createSensor(EObject* object, float posX, float posY, float radius,
		b2Filter collistionBitmask, b2BodyType bodyType);
	//Overload
	static	b2Body*	createBody(EObject* object, float posX, float posY,
		float height, float width, PhysicsMaterial material, b2Filter collistionBitmask, ValueVector points, b2BodyType bodyType = b2BodyType::b2_dynamicBody);

	static	void	updateDestroyer();
	static	void	addToDestroyer(EObject* entity);

	static	void	setDestroyOnOutOfViewPort(EObject* object);
	static	void	setDestroyOnCommand(EObject* object, int command);
	static	void	setDestroyOnTime(EObject* object, float time);

	static	void	setCalledMethodAtCommand(EObject* object, void (*method)(EObject*), int command);
	static	void	setEffectAtCommand(EObject* object, const char* path, int aniIndex, bool noLoop, int command);
	static	void	addEffect(EObject* object, const char* path, int aniIndex, bool noLoop);

	static	void	calledAtBossDie(EObject* boss);
	static	void	calledAtPlayerDie(EObject* player);
private:
#pragma region Severer methods for create entities
	static EObject* create_PLAYER();

	static EObject* create_REACTOR_FLOWER();
	static EObject* create_REACTOR_FIRE();
	static EObject* create_REACTOR_FADE_FIRE();
	static EObject* create_REACTOR_DOWN_ARROW();
	static EObject* create_REACTOR_PORTAL_1();
	static EObject* create_REACTOR_PORTAL_2();
	static EObject* create_REACTOR_PLATFORM_SPAWNER_Y();
	static EObject* create_REACTOR_GUIDE_UP();
	static EObject* create_REACTOR_GUIDE_RIGHT();
	static EObject* create_REACTOR_GUIDE_JUMP();
	static EObject* create_REACTOR_GUIDE_SHOOT();
	static EObject* create_REACTOR_SPRING();
	static EObject* create_REACTOR_SPRING_UP_LEFT();
	static EObject* create_REACTOR_SPRING_UP_RIGHT();

	static EObject* create_PLATFORM_DYNAMIC_TYPE1_X();
	static EObject* create_PLATFORM_DYNAMIC_TYPE1_Y();
	
	static EObject* create_PLATFORM_DYNAMIC_TYPE2_X();
	static EObject* create_PLATFORM_DYNAMIC_TYPE2_Y();

	static EObject* create_PLATFORM_STATIC();
	static EObject* create_PLATFORM_POLYGON();
	static EObject* create_PLATFORM_LADDER();
	static EObject* create_PLATFORM_OBSTACLE();
	static EObject* create_PLATFORM_ROPE();
	static EObject* create_PLATFORM_HITCH();

	static EObject* create_MOB_EAGLE();
	static EObject* create_MOB_FAIRY();
	static EObject* create_MOB_FOX();
	static EObject* create_MOB_HORNY_MUSHROOM();
	static EObject* create_MOB_KING_SMILE();
	static EObject* create_MOB_LIZARD();
	static EObject* create_MOB_LUPIN();
	static EObject* create_MOB_STRIGE();
	static EObject* create_MOB_MALADY();
	static EObject* create_MOB_ZOMBIE_LUPIN();
	static EObject* create_MOB_STATIC_LUPIN_LEFT();
	static EObject* create_MOB_STATIC_LUPIN_RIGHT();
	static EObject* create_MOB_PIG();
	static EObject* create_MOB_RIBBON();
	static EObject* create_MOB_ROYAL_FAIRY();
	static EObject* create_MOB_SNAIL();
	static EObject* create_MOB_STONE_GOBLEM();
	static EObject* create_MOB_WATCH_HOG();
	static EObject* create_MOB_WILDBOAR();
	static EObject* create_MOB_YELLOW_FOX();
	static EObject* create_MOB_COPPER_DRAKE();
	static EObject* create_MOB_CURSE_EYE();
	static EObject* create_MOB_DARK_DRAKE();
	static EObject* create_MOB_DRAGON();
	static EObject* create_MOB_EVIL_EYE();
	static EObject* create_MOB_FAUST();
	static EObject* create_MOB_ICE_DRAKE();
	static EObject* create_MOB_RED_DRAKE();

	static EObject* create_TRAP_HOLE();
	static EObject* create_TRAP_SHURIKEN_TYPE1_X();
	static EObject* create_TRAP_SHURIKEN_TYPE1_Y();
	static EObject* create_TRAP_SHURIKEN_TYPE2_X();
	static EObject* create_TRAP_SHURIKEN_TYPE2_Y();
	static EObject* create_TRAP_SPIKE_50();

	static EObject* create_BULLET_DARK_BALL();
	static EObject* create_BULLET_BANANA();
	static EObject* create_BULLET_GREEN_BANANA();
	static EObject* create_BULLET_POISON_BOOM();
	static EObject* create_BULLET_BANANA_SET();
	static EObject* create_BULLET_EARTH_WAKE();
	static EObject* create_BULLET_FIRE_BALL();
	static EObject* create_BULLET_RED_DRAKE();
	static EObject* create_BULLET_DARK_BAT();

	static EObject*	create_TRAP_1();

	static EObject* create_ITEM_BONUS_HP();
	static EObject* create_ITEM_RESTORE_HP_100();
	static EObject* create_ITEM_RESTORE_HP_250();
	static EObject* create_ITEM_RESTORE_MP_50();
#pragma endregion

	// Support method for create the entities
	// IMPORTAINT: Never use this method for controlled entity
	static void addStateComponent(EObject* object, 
		State* stateHit				= nullptr, 
		bool isDieStateInitialized	= true,
		CBase* AIMoveComponent		= nullptr,
		CBase* AIAttackComponent	= nullptr);

	static	void	destroyEntity(EObject* entity);

private:
	SFactory();
	~SFactory();
private:
	static b2World* m_PhysicsWorld;
	static Layer*	m_GameWorld;

	static vector<EObject*>	m_DestroyedEntity;
};

#endif // !__FACTORY_H__