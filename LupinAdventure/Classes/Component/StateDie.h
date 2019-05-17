#ifndef StateDie_h__
#define StateDie_h__

#include "Component/State.h"

class StateDie : public State
{
public:
	StateDie();
	StateDie(EObject* object);
	~StateDie();

	virtual void enter();

	virtual void exit();

	virtual int commandProcess(int command);

	virtual State* clone(EObject* object);

	virtual bool	checkIfStateChangeable(int newStateIndex);

private:
	b2Filter	m_ObjectFilter;
	b2Body*		m_Body;
	bool		m_ReadyToChangeToOtherState;
};

#endif // StateDie_h__
