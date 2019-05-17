#ifndef __STATE_ATTACK_H__
#define __STATE_ATTACK_H__

#include "State.h"

class EObject;
class CState;

enum
{
	STATE_ATTACK_START = 0,
	STATE_ATTACK_END
};

class StateAttack : public State
{
public:
	StateAttack();
	StateAttack(EObject* object);
	~StateAttack();

	virtual void enter();
	virtual void exit();
	virtual int	commandProcess(int command);
	virtual void update(float delta);
	State* clone(EObject* object);

	virtual bool checkIfStateChangeable(int newStateIndex);

private:
	b2Body*	m_Body;
	int		m_CurrentState;
	bool	m_ReadyToChangeState;
};

#endif // !__STATE_ATTACK_H__