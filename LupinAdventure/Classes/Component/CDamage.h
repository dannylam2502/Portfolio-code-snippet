// Author: Dung
// Created: 11/5/2015
// Damage Component for Character and Mob.

#ifndef CDamage_H_
#define CDamage_H_

#include "cocos2d.h"
#include "CBase.h"
#include <vector>
#include "Config.h"
#include "Entity/EObject.h"
#include "Component/CStats.h"

USING_NS_CC;
using namespace std;

class CDamage : public CBase
{
public:
	static void addComponentTo(EObject* object, int damage, int advanceFilter = ~0, int damageTimes = 4096, float timeBetweenDamage = 1.0f);

	static void addComponentTo(EObject* object, int bonusHP, int bonusLife, int restoreHP, int restoreMP, int advanceFilter = -1, int damageTimes = 1);

	virtual void commandProcess(int command, float deltaTime);

	// parameterTypeFlag:
	//	+ UPARAM_TYPE_NODE: The EObject attacked
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

	virtual bool init();

	virtual void release();

private:
	CREATE_FUNC(CDamage);

	virtual CBase* clone(EObject* object);

	map<EObject*, float*>	m_AttackedList;
	float*					m_TimeBetweenDamage;
	CStats*					m_AttackEffect;
	int						m_AdvanceFilter;
	int						m_DamageTimes;
};

#endif // CDamage_H_