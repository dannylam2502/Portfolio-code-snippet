#include "State.h"

void State::update(float delta)
{
	for (CBase* component : m_componentBoard)
	{
		component->update(delta);
	}
}

void State::cloneInfo(State* dest, EObject* destObject)
{
	dest->m_Object = destObject;

	for (CBase* component : m_componentBoard)
	{
		dest->m_componentBoard.push_back(component->clone(dest->m_Object));
	}
}

vector<CBase*> State::getComponentBoard()
{
	return m_componentBoard;
}

void State::release()
{
	for (CBase* component : m_componentBoard)
	{
		component->release();
	}
}

bool State::checkIfStateChangeable(int newStateIndex)
{
	if (newStateIndex == m_StateIndex)
	{
		return false;
	}

	return true;
}

bool State::checkIfStateReadyToChangeTo()
{
	return true;
}

void State::doStateEffect()
{
	return;
}