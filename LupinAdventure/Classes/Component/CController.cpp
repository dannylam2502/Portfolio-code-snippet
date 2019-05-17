#include "CController.h"

void CController::commandProcess(int command, float deltaTime)
{
	// Do nothing
}

void CController::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_KEYSET:
	{
		this->m_KeyCommandMap.push_back(CControllerKeySet(*parameter.uKeySetPtr));
		break;
	}
	default:
		break;
	}
}

void CController::update(float delta)
{
	// For each key in the key map:
	FOR(m_KeyCommandMap.size())
	{
		// Get current key state.
		KeyState state = SKeyboard::getKeyState(m_KeyCommandMap.at(i).keyCode);

		// If the key is currently activated
		if (state == m_KeyCommandMap.at(i).keyState)
		{
			// Push the key's command in to the object's command queue.
			m_Object->pushCommand(m_KeyCommandMap.at(i).keyCommand);
		}
	}
}

bool CController::init()
{
	if (!CBase::init())
	{
		return false;
	}

	setTag(TAG::COMPONENT::CONTROLLER);

	m_Object = nullptr;
	m_KeyCommandMap.clear();

	scheduleUpdate();
	return true;
}

void CController::addComponentTo(EObject* object, vector<CControllerKeySet> keyCommandMap)
{
	CController* component = CController::create();

	component->m_Object = object;
	component->m_KeyCommandMap.operator=(keyCommandMap);

	object->addChild(component);
}

CController* CController::create(EObject* object, vector<CControllerKeySet> keyCommandMap)
{

	CController* component = CController::create();

	component->m_Object = object;
	component->m_KeyCommandMap.operator=(keyCommandMap);
	
	return component;
}

CBase* CController::clone(EObject* object)
{
	CCLOG("CController::clone did not implemented");
	return nullptr;
}
