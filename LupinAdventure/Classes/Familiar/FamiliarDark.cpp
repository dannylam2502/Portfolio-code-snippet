#include "FamiliarDark.h"
#include "Singleton/SFactory.h"
#include "Component/CAnimation.h"
#include "Component/CStats.h"

FamiliarDark* FamiliarDark::create(EObject* owner)
{
	auto obje = FamiliarDark::create();

	obje->m_Owner = owner;
	m_FamiliarWorld->addChild(obje, Z_ORDER::GAMEWORLD::Familiar);

	return obje;
}

bool FamiliarDark::init()
{
	if (!Familiar::init())
	{
		return false;
	}

	m_Animation = CAnimation::addComponentTo(this, RESOURCE_PATH_FAMILIAR("FamiliarDark"));
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

void FamiliarDark::update(float delta)
{
	Familiar::update(delta);
	setScaleX(m_Owner->getScaleX());
	setPosition(m_Owner->getPosition() + Vec2(30 * getScaleX(), 0));
}

void FamiliarDark::abillity(EObject* owner)
{
	auto scale = owner->getScaleX() * FLIP_FACE_RIGHT.x;

	EObject* bullet[3];
	FOR(3)
	{
		bullet[i] = SFactory::create(ID::BULLET_DARK_BAT);

		auto body = (b2Body*)bullet[i]->getUserData();
		body->GetFixtureList()->SetFilterData(m_Filter);

		bullet[i]->setScaleX(scale);
		if (scale > 0)
			bullet[i]->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
		else
			bullet[i]->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);

		bullet[i]->pushCommand(COMMAND::COMPONENT::MOVE::FLY);
		m_FamiliarWorld->Node::addChild(bullet[i]);
	}

	bullet[0]->setBodyPosition(
		getPositionX() + bullet[0]->getPositionX(),
		getPositionY() + bullet[0]->getPositionY() - 30);

	bullet[1]->setBodyPosition(
		getPositionX() + bullet[1]->getPositionX() + 60 * scale,
		getPositionY() + bullet[1]->getPositionY() + 30);

	bullet[2]->setBodyPosition(
		getPositionX() + bullet[2]->getPositionX(),
		getPositionY() + bullet[2]->getPositionY() + 90);
}
