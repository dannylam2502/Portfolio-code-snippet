#ifndef __STATE_MOVE_H__
#define __STATE_MOVE_H__

#include "State.h"
#include "Utils.h"

class StateMove : public State
{
public:
	StateMove();
	StateMove(EObject* object, const vector<CBase*>& components);
	~StateMove();

	virtual void enter();

	virtual void exit();

	virtual int commandProcess(int command);

	virtual void update(float delta);

	virtual State* clone(EObject* object);

private:
	b2Body*	m_Body;
	bool			m_MoveFlag;		// Turn on if object move Left or Right.
};

#endif // !__STATE_MOVE_H__