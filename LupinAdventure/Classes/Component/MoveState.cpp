#include "MoveState.h"
#include "Entity/EObject.h"

MoveState::MoveState()
{
	m_Object = nullptr;
	m_componentBoard.clear();
}

MoveState::MoveState(EObject* object, const vector<CBase*>& components)
{
	m_Object = object;
	m_componentBoard = components;
}

MoveState::~MoveState()
{
	m_Object = nullptr;
	for (CBase* component : m_componentBoard)
		component->release();
	m_componentBoard.clear();
}

void MoveState::enter()
{
	m_Object->pushCommand(COMMAND_CO_ANI_(ANI_INDEX_MOVE));
}

void MoveState::exit()
{
}

void MoveState::commandProcess(int command)
{
	
}
