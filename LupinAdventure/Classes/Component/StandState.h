#ifndef __STAND_STATE_H__
#define __STAND_STATE_H__

#include "State.h"

class EObject;
class CState;

class StandState : public State
{
public:
	StandState();
	StandState(EObject* object, const vector<CBase*>& components);
	~StandState();

	virtual void enter();
	virtual void exit();

	virtual int	commandProcess(int command);

	virtual void update(float delta);

	virtual State* clone(EObject* object);

};

#endif // !__STAND_STATE_H__