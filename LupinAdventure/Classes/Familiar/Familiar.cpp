#include "Familiar.h"
#include "Component/CAnimation.h"
#include "Component/CStats.h"

Layer* Familiar::m_FamiliarWorld = nullptr;

void Familiar::initialize(Layer* world)
{
	Familiar::m_FamiliarWorld = world;
}

void Familiar::initBeginAttack(EObject* owner)
{
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ATTACK_AFTER, 0.0f);
	auto action = Sequence::create(DelayTime::create(m_Animation->getPreventingAnimationDuration()),
		CallFunc::create(std::bind(&Familiar::initCompleteAttack, this)), nullptr);

	runAction(action);

	abillity(owner);
}

void Familiar::activate(EObject* owner)
{
	if (m_Stats->m_MP >= m_MPcost)
		m_Stats->m_MP -= m_MPcost;
	else
		return;

	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::STOP_LOOP, 0.0F);
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ATTACK_BEFORE, 0.0F);
	auto action = Sequence::create(DelayTime::create(m_Animation->getPreventingAnimationDuration()),
		CallFunc::create(std::bind(&Familiar::initBeginAttack, this, owner)), nullptr);

	runAction(action);
}

void Familiar::initCompleteAttack()
{
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::START_LOOP, 0.0f);
	m_Animation->commandProcess(COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND, 0.0f);
}