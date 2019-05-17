#ifndef __MOVE_STATE_H__
#define __MOVE_STATE_H__

#include "State.h"

class MoveState : public State
{
public:
	MoveState();
	MoveState(EObject* object, const vector<CBase*>& components);
	~MoveState();

	virtual void enter();

	virtual void exit();

	virtual void commandProcess(int command);

};

#endif // !__MOVE_STATE_H__