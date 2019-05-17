#include "CFamiliar.h"


CFamiliar* CFamiliar::addComponentTo(EObject* owner)
{
	auto component = CFamiliar::create();
	component->m_Object = owner;
	component->m_FamiliarList.clear();
	component->m_CrFamiliar = nullptr;
	component->m_CrIndex = -1;
	component->m_Crbody = nullptr;
	component->setTag(TAG::COMPONENT::FAMILIAR);

	owner->addChild(component);

	return component;
}

void CFamiliar::commandProcess(int command, float deltaTime)
{
	switch (command)
	{
	case COMMAND::COMPONENT::FAMILIAR::ACTIVATE_ABILLITY:
		if (m_CrFamiliar != nullptr)
			m_CrFamiliar->activate(m_Object);
		break;

	case COMMAND::COMPONENT::FAMILIAR::NEXT:
		if (m_FamiliarList.size() > 1)
			swapToFamiliar((m_CrIndex + 1) % m_FamiliarList.size());
		break;

	case COMMAND::COMPONENT::FAMILIAR::PREVIOUS:
		if (m_FamiliarList.size() > 1)
			swapToFamiliar(m_CrIndex > 0 ? m_CrIndex-- : m_FamiliarList.size() - 1);
		break;

	case COMMAND::COMPONENT::STATS::RESET:
		if (m_CrFamiliar != nullptr)
			m_CrFamiliar->commandProcess(command);
		break;

	default:
		break;
	}
}

void CFamiliar::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_EOBJECT:
	{
		parameter.uEObjectPtr->retain();
		m_FamiliarList.push_back((Familiar*)parameter.uEObjectPtr);
		//m_Object->getParent()->Node::addChild(parameter.uEObjectPtr, -1);
		swapToFamiliar(m_FamiliarList.size() - 1);
		m_Object->commandProcess(COMMAND::COMPONENT::FAMILIAR::NEW_FAMILIAR);
		break;
	}

	case UPARAM_TYPE_CSTATS:
	{
		m_CrFamiliar->setProperty(TAG::COMPONENT::STATS, UPARAM_TYPE_CSTATS, parameter);
		break;
	}
	}
}

CBase* CFamiliar::clone(EObject* object)
{
	CCLOG("CFamiliar have not implement clone method");
	return nullptr;
}

string CFamiliar::getCrFamiliarName()
{
	return	m_CrFamiliar->getName();
}

CFamiliar::~CFamiliar()
{
	for (EObject* object : m_FamiliarList)
	{
		object->removeFromParentAndCleanup(true);
	}
}

float CFamiliar::getMPPercent()
{
	if (m_CrFamiliar != nullptr)
	{
		return (float)m_Stats->m_MP / m_Stats->m_MaxMP;
	}

	return 1;
}

int CFamiliar::getMP()
{
	if (m_CrFamiliar != nullptr)
	{
		return m_Stats->m_MP;
	}
	return 0;
}

void CFamiliar::swapToFamiliar(int index)
{
	if (m_FamiliarList.size() == 0)
	{
		return;
	}
	m_CrIndex = index;
	//m_CrFamiliar->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::DIE);
	if (m_CrFamiliar != nullptr)
		m_CrFamiliar->pushCommand(COMMAND::COMPONENT::ANIMATION::INVISIBLE);
	m_CrFamiliar = m_FamiliarList.at(m_CrIndex);
	m_CrFamiliar->pushCommand(COMMAND::COMPONENT::ANIMATION::VISIBLE);
	m_Crbody = (b2Body*)m_CrFamiliar->getUserData();
	//m_CrFamiliar->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::SPAWN);
	m_Stats = (CStats*)m_CrFamiliar->getChildByTag(TAG::COMPONENT::STATS);
	m_CrFamiliar->pushPrioritiedCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::STAND);
}

bool CFamiliar::init()
{
	if (!CBase::init())
	{
		return false;
	}

	scheduleUpdate();
	return true;
}

Familiar* CFamiliar::getCrFamiliar()
{
	return m_CrFamiliar;
}
