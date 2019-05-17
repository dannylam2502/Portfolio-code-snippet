#include "StateHit.h"
#include "Entity/EObject.h"

StateHit::StateHit()
{
	m_Object = nullptr;
	m_FlashEffect = nullptr;
	m_Body = nullptr;
	m_ImmortalEffect = nullptr;
	m_StateIndex = STATE_INDEX::HIT;
}

StateHit::~StateHit()
{
	m_FlashEffect->release();
	m_LostcontrolAction->release();
	m_ImmortalEffect->release();
}


StateHit::StateHit(EObject* object, float immuneDuration, float lostControlDuration)
{
	m_Object = object;
	m_Body = (b2Body*)m_Object->getUserData();
	m_Filter = m_Body->GetFixtureList()->GetFilterData();
	if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
	{
		m_ImmuneDuration = 1.5f;
	}
	else
	{
		m_ImmuneDuration = 0.0f;
	}
	if (!isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
	{
		m_LostControlDuration = 0.2f;
	}
	/*else
	{
		m_LostControlDuration = 0.0f;
	}*/
	m_LostControlDuration = 0.0f;
	m_StateIndex = STATE_INDEX::HIT;

	initializeEffect(m_Object);
}

int StateHit::commandProcess(int command)
{
	return command;
}

void StateHit::exit()
{
	
}

void StateHit::enter()
{
	m_RunningFlashEffect = m_FlashEffect->clone();
	m_Object->runAction(m_RunningFlashEffect);
	m_Object->runAction(m_ImmortalEffect->clone());
	m_Object->runAction(m_LostcontrolAction->clone());
	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::HIT));

	auto newFilter = m_Filter;
	newFilter.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::BOTH_ALLY ^ BITMASK::OBJECT::PLAYER_BULLET ^ BITMASK::OBJECT::MOB_BULLET;
	m_Body->GetFixtureList()->SetFilterData(newFilter);
}

void StateHit::stopImmune()
{
	m_Body->GetFixtureList()->SetFilterData(m_Filter);
	m_Object->stopAction(m_RunningFlashEffect);
	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::VISIBLE);
}

State* StateHit::clone(EObject* object)
{
	StateHit* cloneOne = new StateHit(object, m_ImmuneDuration, m_LostControlDuration);

	return cloneOne;
}

void StateHit::initializeEffect(EObject* object)
{
	m_FlashEffect = RepeatForever::create(
		Sequence::create(
		CallFunc::create(std::bind(&EObject::pushCommand, object, COMMAND::COMPONENT::ANIMATION::INVISIBLE)),
		DelayTime::create(STATE_HIT_FLASH_TIME),
		CallFunc::create(std::bind(&EObject::pushCommand, object, COMMAND::COMPONENT::ANIMATION::VISIBLE)),
		DelayTime::create(STATE_HIT_FLASH_TIME * 2),
		nullptr));
	m_FlashEffect->retain();

	m_ImmortalEffect = Sequence::create(
		DelayTime::create(m_ImmuneDuration),
		CallFunc::create(std::bind(&StateHit::stopImmune, this)),
		nullptr);
	m_ImmortalEffect->retain();

	m_LostcontrolAction = Sequence::create(
		DelayTime::create(m_LostControlDuration),
		CallFunc::create(std::bind(&EObject::pushCommand, object, (COMMAND::COMPONENT::STATE::CHANGE_TO::STAND))),
		nullptr);
	m_LostcontrolAction->retain();
}

void StateHit::doStateEffect()
{
	m_RunningFlashEffect = m_FlashEffect->clone();
	m_Object->runAction(m_RunningFlashEffect);
	m_Object->runAction(m_ImmortalEffect->clone());
	m_Object->runAction(m_LostcontrolAction->clone());

	auto newFilter = m_Filter;
	newFilter.maskBits = BITMASK::OBJECT::ALL ^ BITMASK::OBJECT::BOTH_ALLY ^ BITMASK::OBJECT::PLAYER_BULLET ^ BITMASK::OBJECT::MOB_BULLET;
	m_Body->GetFixtureList()->SetFilterData(newFilter);
}