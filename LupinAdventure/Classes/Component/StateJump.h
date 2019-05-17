#ifndef __STATE_JUMP_H__
#define __STATE_JUMP_H__

#include "State.h"

class EObject;
class CState;

class StateJump : public State
{
public:
	StateJump();
	StateJump(EObject* object, const vector<CBase*>& components);
	~StateJump();

	virtual void enter();
	virtual void exit();
	virtual void update(float delta);
	virtual int	commandProcess(int command);

	virtual State* clone(EObject* object);

	virtual bool	checkIfStateChangeable(int newStateIndex);
private:
	// temp code to fix bug
	float	m_TimeJump;
};

#endif // !__STATE_JUMP_H__