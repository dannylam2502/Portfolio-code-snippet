#include "SContactListener.h"
#include "Entity/EObject.h"
#include "Config.h"
#include "Entity/EObject.h"
#include "Utils.h"
#include "Component\CDamage.h"

SContactListener* SContactListener::m_Instance = nullptr;

void SContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	
}

void SContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Body	*bodyP1 = contact->GetFixtureA()->GetBody();
	b2Body	*bodyP2 = contact->GetFixtureB()->GetBody();
	EObject	*objectP1 = (EObject*)bodyP1->GetUserData();
	EObject	*objectP2 = (EObject*)bodyP2->GetUserData();

	int		collideTypeFlag = 0;	// 0000 0000
	collideTypeFlag = computeCollideFlag(contact->GetFixtureA()->GetFilterData().categoryBits,
										 contact->GetFixtureB()->GetFilterData().categoryBits);
	if (!collideTypeFlag)
	{
		collideTypeFlag = computeCollideFlag(contact->GetFixtureB()->GetFilterData().categoryBits,
											 contact->GetFixtureA()->GetFilterData().categoryBits);
		if (!collideTypeFlag)
			return;
		else
		{
			SWAP(bodyP1, bodyP2, b2Body);
			SWAP(objectP1, objectP2, EObject);
		}
	}

	//if (isStringEqual(objectP1->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("collide %s.", objectP2->getName().c_str());
	//}
	//if (isStringEqual(objectP2->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("collide %s.", objectP1->getName().c_str());
	//}

	switch (collideTypeFlag)
	{
	case BITMASK::COLLIDE::PLAYER | BITMASK::COLLIDE::WITH_TILE:
	case BITMASK::COLLIDE::ENEMY | BITMASK::COLLIDE::WITH_TILE:
	case BITMASK::COLLIDE::GENERAL_OBJECT | BITMASK::COLLIDE::WITH_TILE:
		collideProcessObjectTouchTile(contact, objectP1, objectP2);
		break;
	default:
		break;
	}

	//if (!contact->IsEnabled())
	//{
	//	CCLOG("Presolve passed contact %s and %s.", objectP1->getName().c_str(), objectP2->getName().c_str());
	//}
}

void SContactListener::EndContact(b2Contact* contact)
{
	EObject	*objectP1 = (EObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	EObject	*objectP2 = (EObject*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (objectP2 == nullptr || objectP1 == nullptr)
			return;

	objectP1->removeLinkedObject(objectP2);
	objectP2->removeLinkedObject(objectP1);

	if (isStringEqual(objectP1->getName().c_str(), NAME::PLATFORM_LADDER) ||
		isStringEqual(objectP1->getName().c_str(), NAME::PLATFORM_ROPE))
	{
		objectP2->removeLinkedObject(objectP1);
		objectP2->pushCommand(COMMAND::ENTITY::CLIMB_STOP);
		objectP2->pushCommand(COMMAND::ENTITY::CLIMB_UNREADY);
	}


	if (isStringEqual(objectP2->getName().c_str(), NAME::PLATFORM_LADDER) ||
		isStringEqual(objectP2->getName().c_str(), NAME::PLATFORM_ROPE))
	{
		objectP1->removeLinkedObject(objectP2);
		objectP1->pushCommand(COMMAND::ENTITY::CLIMB_STOP);
		objectP1->pushCommand(COMMAND::ENTITY::CLIMB_UNREADY);
	}	
	
	//if (isStringEqual(objectP1->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("Player out %s", objectP2->getName().c_str());
	//}

	//if (isStringEqual(objectP2->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("Player out %s", objectP1->getName().c_str());
	//}
}

void SContactListener::BeginContact(b2Contact* contact)
{
	EObject	*objectP1 = (EObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	EObject	*objectP2 = (EObject*)contact->GetFixtureB()->GetBody()->GetUserData();

	objectP1->setLinkedObject(objectP2);
	objectP2->setLinkedObject(objectP1);

	//if (isStringEqual(objectP1->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("Player contact %s", objectP2->getName().c_str());
	//}

	//if (isStringEqual(objectP2->getName().c_str(), NAME::PLAYER))
	//{
	//	CCLOG("Player contact %s", objectP1->getName().c_str());
	//}
}

void SContactListener::init()
{
	m_Instance = new SContactListener();
}

void SContactListener::release()
{
	CC_SAFE_DELETE(m_Instance);
}

SContactListener* SContactListener::get()
{
	if (m_Instance == nullptr)
	{
		init();
	}

	return m_Instance;
}

SContactListener::SContactListener()
{

}

SContactListener::~SContactListener()
{

}

int SContactListener::computeCollideFlag(int objectP1Category, int objectP2Category)
{
	int result = 0;	// 0000 0000

	switch (objectP1Category)
	{
	case BITMASK::OBJECT::PLAYER:
		result = result | BITMASK::COLLIDE::PLAYER;
#pragma region Player collide with...
		switch (objectP2Category)
		{
		case BITMASK::OBJECT::MOB_BULLET:
			result = result | BITMASK::COLLIDE::WITH_OPPONENT_BULLET;
			break;
		case BITMASK::OBJECT::MOB:
			result = result | BITMASK::COLLIDE::WITH_OPPONENT;
			break;
		case BITMASK::OBJECT::GENERAL:
		case BITMASK::OBJECT::TILE:
			result = result | BITMASK::COLLIDE::WITH_TILE;
			break;
		case BITMASK::OBJECT::LADDER_ROPE:
			result = result | BITMASK::COLLIDE::WITH_CLIMBABLE_OBJECT;
			break;
		default:
			result = 0;
			break;
		}
#pragma endregion
		break;

	case BITMASK::OBJECT::MOB:
		result = result | BITMASK::COLLIDE::ENEMY;
#pragma region Mob collide with...
		switch (objectP2Category)
		{
		case BITMASK::OBJECT::PLAYER_BULLET:
			result = result | BITMASK::COLLIDE::WITH_OPPONENT_BULLET;
			break;
		case BITMASK::OBJECT::TILE:
		case BITMASK::OBJECT::GENERAL:
			result = result | BITMASK::COLLIDE::WITH_TILE;
			break;
		case BITMASK::OBJECT::LADDER_ROPE:
			result = result | BITMASK::COLLIDE::WITH_CLIMBABLE_OBJECT;
			break;
		default:
			result = 0;
			break;
		}
#pragma endregion
		break;

	case BITMASK::OBJECT::GENERAL:
		result = result | BITMASK::COLLIDE::GENERAL_OBJECT;
#pragma region General object collide with...
		switch (objectP2Category)
		{
		case BITMASK::OBJECT::MOB_BULLET:
		case BITMASK::OBJECT::PLAYER_BULLET:
			result = result | BITMASK::COLLIDE::WITH_OPPONENT_BULLET;
			break;
		case BITMASK::OBJECT::TILE:
			result = result | BITMASK::COLLIDE::WITH_TILE;
			break;
		default:
			result = 0;
			break;
		}
#pragma endregion
		break;

	default:
		break;
	}
	return result;
}

void SContactListener::collideProcessObjectTouchTile(b2Contact* contact, EObject* object, EObject* theTile)
{
	if ((contact->GetManifold()->localNormal.x != 0.0f) ||
		(contact->GetManifold()->localNormal.y > 0.0f
		&& isStringEqual(object->getName().c_str(), ID::PLAYER)))
	{
		contact->SetEnabled(false);
	}
	/*if (contact->GetManifold()->localNormal.y < 0.0f)*/
	else
	{
		/*if (object->getPositionY() < theTile->getPositionY())
		{
			return;
		}*/
		object->pushCommand(COMMAND::ENTITY::LANDING);
	}
}