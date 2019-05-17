#ifndef __CSTAT_H__
#define __CSTAT_H__

#include "cocos2d.h"
#include "Entity/EObject.h"
#include "CBase.h"
USING_NS_CC;

class CStats : public CBase
{
public:
	static void addComponentTo(EObject* object, int maxhp, int maxmp, int hp, int mp, int life = 0);

	// command:
	//	+ COMMAND_CO_STATS_RESET: set the HP and MP values to max
	virtual void commandProcess(int command, float deltaTime);

	// The current stats will be directly increased by this method.
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual bool init();

	virtual void update(float delta);
	
	static CStats* createStats(int damage);
	static CStats* createStats(int bonusHP,int bonusLife, int restoreHP, int restoreMP);

	int getMP();

	int getDeltaHP();
	int getHP();
	int getMaxHP();
private:
	CREATE_FUNC(CStats);

public:
	int			m_MaxHP;
	int			m_MaxMP;

	int			m_HP;
	int			m_MP;

	int			m_life;

private:
	void addStats(int hp, int mp = 0, int maxHP = 0, int maxMP = 0, int life = 0);

	virtual CBase* clone(EObject* object);

};
#endif // !__CSTAT_H__
