#include "CReact.h"
#include "Entity/EObject.h"

CReact* CReact::addComponentTo(EObject* object, Action* actionOnTouch)
{
	auto component = CReact::create();
	component->m_Object = object;
	component->m_ActionOnTouch = actionOnTouch;
	component->m_ActionOnTouch->retain();
	component->scheduleUpdate();

	object->addChild(component);
	return component;
}

CBase* CReact::clone(EObject* object)
{
	CCLOG("CReact does not implement clone method.");
	return nullptr;
}

void CReact::setProperty(int parameterTypeFlag, UParam parameter)
{
	
}

void CReact::commandProcess(int command, float deltaTime)
{
	
}

void CReact::update(float delta)
{
	m_Object->runAction(m_ActionOnTouch->clone());
}
