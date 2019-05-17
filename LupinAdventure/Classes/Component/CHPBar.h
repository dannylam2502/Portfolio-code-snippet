#ifndef __CHPBAR_H__
#define __CHPBAR_H__

#include "cocos2d.h"
#include "Entity/EObject.h"
#include "CBase.h"
#include "Component\CStats.h"
USING_NS_CC;

class CHPBar : public CBase
{
public:
	static void addComponentTo(EObject* object);
	static void addComponentTo(EObject* object, Vec2 offset, Size bodySize);

	// command:
	//	+ COMMAND_CO_STATS_RESET: set the HP and MP values to max
	virtual void commandProcess(int command, float deltaTime);

	// The current stats will be directly increased by this method.
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual bool init();

	virtual void update(float delta);

private:
	CREATE_FUNC(CHPBar);

public:
	Sprite* m_HPBar;
	Sprite* m_HPBackground;
	float	m_RectWidth;
	CStats* m_Stats;
	Vec2	m_OffSet;
	~CHPBar();
private:
	virtual CBase* clone(EObject* object);

};
#endif // !___CHPBar____
