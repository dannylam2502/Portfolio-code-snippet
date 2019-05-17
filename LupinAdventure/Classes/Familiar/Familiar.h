#ifndef Familiar_h__
#define Familiar_h__

#include "cocos2d.h"
#include "Entity/EObject.h"
#include "Box2D/Box2D.h"

class CAnimation;
class CStats;

class Familiar : public EObject
{
public:
	static void initialize(Layer* world);
	virtual void activate(EObject* owner);

	void initBeginAttack(EObject* owner);
	void initCompleteAttack();

	virtual void abillity(EObject* owner) = 0;

protected:
	static Layer* m_FamiliarWorld;

	CAnimation* m_Animation;
	CStats*		m_Stats;
	EObject*	m_Owner;
	b2Filter	m_Filter;
	int			m_MPcost;
};
#endif // Familiar_h__
