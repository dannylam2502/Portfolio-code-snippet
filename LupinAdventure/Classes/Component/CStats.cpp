#include "Component/CStats.h"
#include "Utils.h"
#include "Component\CFamiliar.h"

void CStats::addComponentTo(EObject* object, int maxhp, int maxmp, int hp, int mp, int life)
{
	auto component = CStats::create();

	component->m_Object = object;
	component->m_HP = hp;
	component->m_MP = mp;
	component->m_MaxHP = maxhp;
	component->m_MaxMP = maxmp;
	component->m_life = life;

	object->addChild(component);
}

void CStats::commandProcess(int command, float deltaTime)
{
	switch (command)
	{
	case COMMAND::COMPONENT::STATS::RESET:
		m_HP = m_MaxHP;
		m_MP = m_MaxMP;
		m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::STAND);
		break;
	default:
		break;
	}
}

void CStats::update(float delta)
{

}

void CStats::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_CSTATS:
	{
		/*if (isStringEqual(m_Object->getName().c_str(), NAME::PLAYER))
		{
			CCLOG("Stats affected");
		}*/
		CStats* source = parameter.uStats;
		addStats(
			source->m_HP,
			source->m_MP,
			source->m_MaxHP,
			source->m_MaxMP,
			source->m_life);

		if (m_HP <= 0)
		{
			m_HP = 0;
			m_Object->commandProcess(COMMAND::COMPONENT::MOVE::STOP_ALL);
			m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::DIE);
			return;
		}

		if (source->m_HP < 0)
		{
			/*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
			{
				CCLOG("HIT = %d", source->m_HP);
			}*/
			//source->m_HP = 0;
			m_Object->commandProcess(COMMAND::COMPONENT::STATE::CHANGE_TO::HIT);
			m_Object->pushCommand(COMMAND::COMPONENT::MOVE::KNOCK_BACK);
		}
	}
		break;
	default:
		break;
	}
}

bool CStats::init()
{
	if (!CBase::init())
	{
		return false;
	}
	setTag(TAG::COMPONENT::STATS);

	m_MaxHP = 0;
	m_MaxMP = 0;
	m_HP = 0;
	m_MP = 0;

	scheduleUpdate();
	return true;
}

void CStats::addStats(int hp, int mp /*= 0*/, int maxHP /*= 0*/, int maxMP /*= 0*/, int life)
{
	m_HP += hp;
	m_MP += mp;
	m_MaxHP += maxHP;
	m_MaxMP += maxMP;
	m_life += life;

	m_HP > m_MaxHP ? m_HP = m_MaxHP : 0;
	m_MP > m_MaxMP ? m_MP = m_MaxMP : 0;

	/*if (m_HP < 0)
	{
		m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::DIE);
	}*/
}

CStats*	CStats::createStats(int damage)
{
	auto component = CStats::create();
	component->m_HP = damage;
	return component;
}

CBase* CStats::clone(EObject* object)
{
	auto cloneOne = CStats::create();

	cloneOne->m_HP = m_HP;
	cloneOne->m_MP = m_MP;
	cloneOne->m_MaxHP = m_MaxHP;
	cloneOne->m_MaxMP = m_MaxMP;

	cloneOne->m_Object = object;
	object->addChild(cloneOne);

	return cloneOne;
}

CStats* CStats::createStats(int bonusHP, int bonusLife, int restoreHP, int restoreMP)
{
	auto component = CStats::create();
	component->m_HP = restoreHP;
	component->m_MP = restoreMP;
	component->m_MaxHP = bonusHP;
	component->m_life = bonusLife;
	return component;
}

int CStats::getDeltaHP()
{
	return this->m_MaxHP - this->m_HP;
}

int CStats::getMaxHP()
{
	return m_MaxHP;
}

int CStats::getHP()
{
	return m_HP;
}

int CStats::getMP()
{
	return m_MP;
}
