#include "StateAttack.h"
#include "Entity/EObject.h"
#include "Component/CAttack.h"
#include "Singleton/SFactoryEffect.h"

StateAttack::StateAttack()
{
	m_Object = nullptr;
	m_CurrentState = STATE_ATTACK_START;
	m_componentBoard.clear();
	m_StateIndex = STATE_INDEX::ATTACK;
	m_ReadyToChangeState = false;
}

StateAttack::StateAttack(EObject* object)
{
	m_Object = object;
	m_CurrentState = STATE_ATTACK_START;
	m_Body = (b2Body*)object->getUserData();
	m_StateIndex = STATE_INDEX::ATTACK;
	m_ReadyToChangeState = false;
}

StateAttack::~StateAttack()
{
	m_Object = nullptr;
}

void StateAttack::update(float delta)
{
	State::update(delta);
}

void StateAttack::enter()
{
	//CCLOG("STATE ATTACK = %d");
	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::STOP_LOOP);
	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ATTACK_BEFORE));
	m_CurrentState = STATE_ATTACK_START;
	m_ReadyToChangeState = false;
}

void StateAttack::exit()
{
	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::START_LOOP);
}

int StateAttack::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::COMPONENT::ANIMATION::DONE:
	{
		if (STATE_ATTACK_START == m_CurrentState)
		{
			m_Object->pushCommand(COMMAND::COMPONENT::ATTACK::ACTIVATE);
			m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ATTACK_AFTER));
			m_CurrentState = STATE_ATTACK_END;
		}
		else
		{
			m_ReadyToChangeState = true;
			m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));
		}
		break;
	}
	default:
		break;
	}

	return command;
}

State* StateAttack::clone(EObject* object)
{
	StateAttack* cloneOne = new StateAttack();
	State::cloneInfo(cloneOne, object);

	return cloneOne;
}

bool StateAttack::checkIfStateChangeable(int newStateIndex)
{
	if (newStateIndex == STATE_INDEX::DIE)
	{
		return true;
	}
	return m_ReadyToChangeState;
}
