#include "Component/CAIAttack.h"
#include "Config.h"
#include "Component/CSensor.h"
#include "Entity/EObject.h"

void CAIAttack::commandProcess(int command, float deltaTime)
{
	switch (command)
	{
	case COMMAND::COMPONENT::AI_ATTACK::GIVE_UP_TARGET:
		m_Target = nullptr;
		break;
	}
}

void CAIAttack::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_NODE:
		m_Target = (EObject*)parameter.uNodePtr;
		break;
	default:
		break;
	}
}

void CAIAttack::update(float delta)
{
	m_TimeCounter += delta;
	updateSensor();

	if (m_Target == nullptr)
		return;

	if (m_TimeCounter >= m_ReloadTime)
	{
		m_TimeCounter = 0.0f;
		m_Object->pushCommand(COMMAND::COMPONENT::MOVE::CHASE);
		m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK);
	}
}

bool CAIAttack::init()
{
	if (!CBase::init())
	{
		return false;
	}
	setTag(TAG::COMPONENT::AI_ATTACK);

	m_Object = nullptr;
	m_Target = nullptr;	
	scheduleUpdate();

	return true;
}

void CAIAttack::addComponentTo(EObject* object, float reloadTime)
{
	auto component = createComponent(object, reloadTime);

	if (component != nullptr)
	{
		component->m_Object->addChild(component);
	}
}

CAIAttack* CAIAttack::createComponent(EObject* object, float reloadTime)
{
	CAIAttack* component = CAIAttack::create();

	component->m_Object = object;
	component->m_ReloadTime = reloadTime;
	component->m_TimeCounter = reloadTime;

	component->m_Sensor = (CSensor*)object->getChildByTag(TAG::COMPONENT::SENSOR);

	return component;
}


CBase* CAIAttack::clone(EObject* object)
{
	CCLOG("CAIAttack::clone did not implemented");
	return nullptr;
}

void CAIAttack::updateSensor()
{
	if (m_Target == nullptr)
		m_Target = m_Sensor->getNearestObjectInSensor();
	else
		if (!m_Sensor->checkIfObjectInSensor(m_Target))
		{
			m_Target = nullptr;
		}
}