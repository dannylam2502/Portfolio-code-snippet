#ifndef StateClimb_h__
#define StateClimb_h__

#include "State.h"
#include "Component/CBase.h"
#include <vector>
using namespace std;

class EObject;

class StateClimb : public State
{
public:
	StateClimb();
	StateClimb(EObject* object, vector<CBase*> components);
	~StateClimb();

	virtual void enter();

	virtual void exit();

	virtual int commandProcess(int command);

	virtual State* clone(EObject* object);

	virtual bool checkIfStateReadyToChangeTo();

};

#endif // StateClimb_h__