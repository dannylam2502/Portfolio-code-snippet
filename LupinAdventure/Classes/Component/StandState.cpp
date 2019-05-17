#include "StandState.h"
#include "Entity/EObject.h"

StandState::StandState()
{
	m_Object = nullptr;
	m_componentBoard.clear();
	m_StateIndex = STATE_INDEX::STAND;
}

StandState::StandState(EObject* object, const vector<CBase*>& components)
{
	m_Object = object;
	m_componentBoard = components;

	for (CBase* component : m_componentBoard)
		component->retain();
	m_StateIndex = STATE_INDEX::STAND;
}

StandState::~StandState()
{
	m_Object = nullptr;
	for (CBase* component : m_componentBoard)
		component->release();
	m_componentBoard.clear();
}

void StandState::enter()
{
	/*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
	{
		CCLOG("STAND");
	}*/
	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::STAND));
	/*auto body = (b2Body*)m_Object->getUserData();
	if (isStringEqual(m_Object->getName().c_str(), ID::MOB_STRIGE)
		|| isStringEqual(m_Object->getName().c_str(), ID::MOB_MALADY))
	{
		body->SetGravityScale(1.0f);
	}*/
}

void StandState::exit()
{

}

int StandState::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::COMPONENT::MOVE::RIGHT:
	case COMMAND::COMPONENT::MOVE::LEFT:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE));
		break;

	case COMMAND::COMPONENT::MOVE::JUMP:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP));
		break;

	case COMMAND::COMPONENT::ATTACK::ACTIVATE:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));
		break;

	case COMMAND::ENTITY::CLIMB_START:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::CLIMB));
		break;

	default:
		break;
	}

	return command;
}

void StandState::update(float delta)
{
	State::update(delta);

	b2Body* body = (b2Body*)m_Object->getUserData();
	
	if (m_Object->getLinkedObjects().size() == 0 && body->GetLinearVelocity().y != 0)
	{
		m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP);
	}
}

State* StandState::clone(EObject* object)
{
	StandState* cloneOne = new StandState();
	State::cloneInfo(cloneOne, object);

	return cloneOne;
}