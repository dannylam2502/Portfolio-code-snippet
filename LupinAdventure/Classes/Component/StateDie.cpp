#include "StateDie.h"
#include "Entity/EObject.h"

StateDie::StateDie()
{
	m_Object = nullptr;
	m_componentBoard.clear();
	m_Body = nullptr;
	m_StateIndex = STATE_INDEX::DIE;
	m_ReadyToChangeToOtherState = true;
}

StateDie::StateDie(EObject* object)
{
	m_Object = object;
	m_Body = (b2Body*)object->getUserData();
	m_ObjectFilter = m_Body->GetFixtureList()->GetFilterData();
	m_StateIndex = STATE_INDEX::DIE;
	m_ReadyToChangeToOtherState = true;
}

StateDie::~StateDie()
{
	m_Object = nullptr;
}

State* StateDie::clone(EObject* object)
{
	CCLOG("StateDie did not implemented");
	return nullptr;
}

int StateDie::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::COMPONENT::ANIMATION::DONE:
	{
		m_Object->pushCommand(COMMAND::ENTITY::DIE);
		m_ReadyToChangeToOtherState = true;
		break;
	}
	}
	return command;
}

void StateDie::exit()
{
	m_Body->SetGravityScale(1.0f);
	m_Body->GetFixtureList()->SetFilterData(m_ObjectFilter);

	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::START_LOOP);
	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::VISIBLE);
}

void StateDie::enter()
{
	//CCLOG("STATE DIE");
	m_ReadyToChangeToOtherState = false;
	b2Filter filterNone;
	filterNone.categoryBits = 0;
	m_Body->GetFixtureList()->SetFilterData(filterNone);
	m_Body->SetGravityScale(0.0f);
	m_Body->SetLinearVelocity(B2VEC2_ZERO);
	//Dung 2/7
	m_Object->clearAllCommand();
	//
	m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::STOP_LOOP);
	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::DIE));

}

bool	StateDie::checkIfStateChangeable(int newStateIndex)
{
	return m_ReadyToChangeToOtherState;
}