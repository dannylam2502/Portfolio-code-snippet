 #include "CAttack.h"
#include "Singleton\SFactory.h"
#include "Entity/EObject.h"
#include "Singleton\SFactoryEffect.h"
#include "Component\CAnimation.h"

void CAttack::addComponentTo(EObject* object, const char* idBullet, b2Vec2 shootSpot)
{
	auto component = createComponent(object, idBullet, shootSpot);
	if (component != nullptr)
	{
		component->m_Object->addChild(component);
	}
}

CAttack* CAttack::createComponent(EObject* object, const char* idBullet, b2Vec2 shootSpot)
{
	CAttack* component = CAttack::create();

	component->m_Object = object;
	component->m_idBullet = idBullet;
	component->m_ShootSpot = shootSpot;
	component->m_Body = (b2Body*)object->getUserData();
	//component->m_vectorBulletInfo = createBulletInfoFromFile(idBullet);

	b2Body* body = (b2Body*)object->getUserData();

	if (body->GetFixtureList()->GetFilterData().categoryBits
		& BITMASK::OBJECT::PLAYER)
	{
		component->m_BulletFilter.categoryBits = BITMASK::OBJECT::PLAYER_BULLET;
		component->m_BulletFilter.maskBits = BITMASK::OBJECT::MOB | BITMASK::OBJECT::GENERAL;
	}
	else
	{
		component->m_BulletFilter.categoryBits = BITMASK::OBJECT::MOB_BULLET;
		component->m_BulletFilter.maskBits = BITMASK::OBJECT::PLAYER | BITMASK::OBJECT::GENERAL;
	}

	return component;
}

void CAttack::commandProcess(int command, float deltaTime)
{
	switch (command)
	{
		case COMMAND::COMPONENT::ATTACK::ACTIVATE:
		{
			EObject* bullet = SFactory::create(m_idBullet);
			auto body		= (b2Body*)bullet->getUserData();
			auto posX		= m_Body->GetPosition().x;
			auto posY		= m_Body->GetPosition().y;
			auto scaleX		= m_Object->getScaleX() * FLIP_FACE_RIGHT.x;
			
			auto filter = m_BulletFilter;
			filter.categoryBits = filter.categoryBits | body->GetFixtureList()->GetFilterData().categoryBits;
			filter.maskBits = filter.maskBits | body->GetFixtureList()->GetFilterData().maskBits;

			body->GetFixtureList()->SetFilterData(filter);
			bullet->setBodyPosition(
				(posX + body->GetPosition().x * scaleX) * BOX2D_RATIO + m_ShootSpot.x * scaleX,
				(posY + body->GetPosition().y) * BOX2D_RATIO + m_ShootSpot.y);

			//bullet->setScaleX(scaleX);

			if (scaleX > 0)
				bullet->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
			else
				bullet->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);

			bullet->pushCommand(COMMAND::COMPONENT::MOVE::FLY);

			m_PhysicWorld->addChild(bullet);
			break;
		}
	}
}

void CAttack::setProperty(int parameterTypeFlag, UParam parameter)
{

}

void CAttack::update(float delta)
{
	if (m_IsBoost)
	{
		m_TimeBoost += delta;
	}
	else
	{
		m_TimeBoost = 0.0f;
	}
}

bool CAttack::init()
{
	if (!CBase::init())
	{
		return false;
	}

	setTag(TAG::COMPONENT::ATTACK);
	m_TimeBoost = 0.0f;
	m_Object = nullptr;
	m_ShootSpot = B2VEC2_ZERO;
	m_Body = nullptr;

	scheduleUpdate();
	return true;
}

// Frederick - 16/5/2015
void CAttack::setWorld(Layer* gameWorldLayer)
{
	m_PhysicWorld = gameWorldLayer;
}

void	CAttack::makeBulletLive(EObject* l_Bullet)
{
	m_PhysicWorld->addChild(l_Bullet, Z_ORDER::GAMEWORLD::Bullet);
}

CBase* CAttack::clone(EObject* object)
{
	CCLOG("CAttack::clone did not implemented");
	return nullptr;
}

Layer* CAttack::m_PhysicWorld = nullptr;