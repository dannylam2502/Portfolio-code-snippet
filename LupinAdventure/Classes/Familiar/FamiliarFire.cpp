#include "FamiliarFire.h"
#include "Singleton/SFactory.h"
#include "Component/CAnimation.h"
#include "Component/CStats.h"

bool FamiliarFire::init()
{
	if (!Familiar::init())
	{
		return false;
	}

	m_Animation = CAnimation::addComponentTo(this, RESOURCE_PATH_FAMILIAR("FamiliarFire"));
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::START_LOOP, 0.0f);
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND, 0.0f);
	m_Animation->setProperty(UPARAM_TYPE_VEC2, UParam(b2Vec2(0.0f, -15.0f)));
	CStats::addComponentTo(this, 0, 150, 0, 150);
	m_Stats = (CStats*)getChildByTag(TAG::COMPONENT::STATS);

	m_Filter.categoryBits = BITMASK::OBJECT::PLAYER_BULLET;
	m_Filter.maskBits = BITMASK::OBJECT::MOB;
	m_MPcost = 10;

	scheduleUpdate();

	return true;
}

void FamiliarFire::update(float delta)
{
	Familiar::update(delta);
	setScaleX(m_Owner->getScaleX());
	setPosition(m_Owner->getPosition() + Vec2(30 * getScaleX(), 0));
}

FamiliarFire* FamiliarFire::create(EObject* owner)
{
	auto obje = FamiliarFire::create();

	obje->m_Owner = owner;
	m_FamiliarWorld->addChild(obje, Z_ORDER::GAMEWORLD::Familiar);

	return obje;
}

void FamiliarFire::abillity(EObject* owner)
{
	auto bullet = SFactory::create(ID::BULLET_FIRE_BALL);
	auto scale = owner->getScaleX() * FLIP_FACE_RIGHT.x;

	bullet->setBodyPosition(
		getPositionX() + bullet->getPositionX() * scale,
		getPositionY() + bullet->getPositionY() + 30);

	auto body = (b2Body*)bullet->getUserData();
	body->GetFixtureList()->SetFilterData(m_Filter);

	bullet->setScaleX(scale);
	if (scale > 0)
		bullet->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
	else
		bullet->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);

	bullet->pushCommand(COMMAND::COMPONENT::MOVE::FLY);
	m_FamiliarWorld->Node::addChild(bullet);
}