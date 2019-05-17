#include "CDamage.h"
USING_NS_CC;

void CDamage::addComponentTo(EObject* object, int damage, int advanceFilter, int damageTimes, float timeBetweenDamage)
{
	CDamage* component = CDamage::create();

	component->m_Object = object;
	component->m_Object->addChild(component);

	component->m_AttackEffect = CStats::createStats(-damage);
	component->m_AdvanceFilter = advanceFilter;
	component->m_DamageTimes = damageTimes;
	component->m_AttackEffect->retain();
	component->m_TimeBetweenDamage = new float(timeBetweenDamage);
	component->m_AttackedList.clear();

	component->setTag(TAG::COMPONENT::DAMAGE);
}

void CDamage::addComponentTo(EObject* object, int bonusHP, int bonusLife, int restoreHP, int restoreMP, int healableBitmask, int healingTimes)
{
	CDamage* component = CDamage::create();

	component->m_Object = object;
	component->m_Object->addChild(component);

	component->m_AttackEffect = CStats::createStats(bonusHP, bonusLife, restoreHP, restoreMP);
	component->m_AdvanceFilter = healableBitmask;
	component->m_DamageTimes = healingTimes;
	component->m_AttackEffect->retain();
	component->m_TimeBetweenDamage = new float(0.0f);
	component->m_AttackedList.clear();

	component->setTag(TAG::COMPONENT::DAMAGE);
}

void CDamage::commandProcess(int command, float deltaTime)
{
}

void CDamage::setProperty(int parameterTypeFlag, UParam parameter)
{
}

void CDamage::update(float delta)
{
	if (m_DamageTimes <= 0)
		return;

	for (std::map<EObject*, float*>::iterator attackPair = m_AttackedList.begin(); attackPair != m_AttackedList.end();)
	{
		*attackPair->second -= delta;

		if (*attackPair->second <= 0.0f)
		{
			//delete attackPair->second;
			m_AttackedList.erase(attackPair++);
		}
		else
		{
			++attackPair;
		}
	}

	for each (EObject* object in m_Object->getLinkedObjects())
	{
		for (std::pair<EObject*, float*> attackPair : m_AttackedList)
			if (object == attackPair.first)
				goto END;

		auto body = (b2Body*)m_Object->getUserData();
		int maskBits = body->GetFixtureList()->GetFilterData().maskBits;
		int category = body->GetFixtureList()->GetFilterData().categoryBits;

		//m_AttackedList.insert(std::pair<EObject*, float>(object, m_TimeBetweenDamage));
		auto bodyObject = (b2Body*)object->getUserData();
		int categoryBitmask = bodyObject->GetFixtureList()->GetFilterData().categoryBits;
		int maskBitsBitMask = bodyObject->GetFixtureList()->GetFilterData().maskBits;
		if (((categoryBitmask & maskBits) & m_AdvanceFilter) && (maskBitsBitMask & category))
		{
			m_DamageTimes--;
			object->setProperty(m_AttackEffect->getMP() == 0 ? TAG::COMPONENT::STATS : TAG::COMPONENT::FAMILIAR, UPARAM_TYPE_CSTATS, m_AttackEffect);
			m_AttackedList.insert(std::pair<EObject*, float*>(object, m_TimeBetweenDamage));
			m_Object->pushCommand(COMMAND::ENTITY::MAKE_DAMAGE);

			if (m_DamageTimes == 0)
			{
				m_Object->pushCommand(COMMAND::ENTITY::EXPIRED_DAMAGE);
				break;
			}
		}
	END:;
	}

	//m_AttackedList.clear();
}

bool CDamage::init()
{
	if (!CBase::init())
	{
		return false;
	}

	setTag(TAG::COMPONENT::DAMAGE);

	m_Object = nullptr;

	scheduleUpdate();
	return true;
}

void CDamage::release()
{
	m_AttackEffect->release();
}

CBase* CDamage::clone(EObject* object)
{
	CCLOG("CDamage did not implemented");
	return nullptr;
}
