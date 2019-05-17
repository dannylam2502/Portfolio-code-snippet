#include "CState.h"
#include "Entity/EObject.h"

CState*	CState::addComponentTo(EObject* object, const map<int, State*>& stateMap)
{
	auto component = CState::create();

	component->m_Object = object;
	component->m_stateMap.operator=(stateMap);
	try
	{
		component->m_StateDie = stateMap.at(STATE_INDEX::DIE);
	}
	catch (exception e)
	{
		component->m_StateDie = nullptr;
	}

	object->addChild(component);

	return component;
}

void	CState::commandProcess(int command, float deltaTime)
{
	if (command < (COMMAND::COMPONENT::STATE::CHANGE_TO::LAST_PLUS_1) &&
		command >= (COMMAND::COMPONENT::STATE::CHANGE_TO::FIRST))
	{
		//CCLOG("%s, %d", m_Object->getName().c_str(), command);
		changeState(command);
	}
	else
	{
		switch (command)
		{
		case COMMAND::ENTITY::CLIMB_READY:
			m_ReadyToClimb = true;
			return;

		case COMMAND::ENTITY::CLIMB_UNREADY:
			m_ReadyToClimb = false;
			return;

		case COMMAND::ENTITY::CLIMB_START:
		{
			if (!m_ReadyToClimb)
				return;
			break;
		}

		default:
			break;
		}

		m_CurrentState->commandProcess(command);
	}
}

void	CState::setProperty(int parameterTypeFlag, UParam parameter)
{
}

void	CState::update(float delta)
{
	m_CurrentState->update(delta);
}

bool	CState::init()
{
	if (!CBase::init())
	{
		return false;
	}

	m_CurrentState = nullptr;
	m_stateMap.clear();
	m_ReadyToClimb = false;
	// Dung 29/6
	m_LastStateIndex = 0;
	//
	setTag(TAG::COMPONENT::STATE);
	
	scheduleUpdate();

	return true;
}

void	CState::changeState(int index)
{
	if (!m_stateMap.at(index)->checkIfStateReadyToChangeTo())
		return;

	if (m_CurrentState == m_StateDie)
	{
		if (!isStringEqual(m_Object->getName().c_str(), NAME::PLAYER))
			return;
	}

	if (m_CurrentState != nullptr)
	{
		if (!m_CurrentState->checkIfStateChangeable(index))
		{
			m_stateMap.at(index)->doStateEffect();
			return;
		}

		m_CurrentState->exit();
	}
	// Dung 29/6
	if (m_CurrentState != nullptr)
	{
		/*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
		{
			CCLOG("%d", m_LastStateIndex);
		}*/
		m_LastStateIndex = m_CurrentState->m_StateIndex;
	}
	//
	m_CurrentState = m_stateMap.at(index);

	m_CurrentState->enter();
}

CBase* CState::clone(EObject* object)
{
	State* currentState = nullptr;
	map<int, State*> map;

	for (std::pair<int, State*> statePair : m_stateMap)
	{
		State* clonnedState = statePair.second->clone(object);
		int clonnedIndex = statePair.first;

		if (statePair.second == m_CurrentState)
		{
			currentState = clonnedState;
		}
	}

	return CState::addComponentTo(object, map);
}

int		CState::getLastStateIndex()
{
	return m_LastStateIndex;
}

map<int, State*> CState::getStateMap()
{
	return m_stateMap;
}

CState::~CState()
{
	for (pair<int, State*> statePair : m_stateMap)
	{
		statePair.second->release();
	}
}