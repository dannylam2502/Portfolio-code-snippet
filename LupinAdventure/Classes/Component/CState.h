#ifndef __COMPONENT_STATE_H__
#define __COMPONENT_STATE_H__

#include "CBase.h"
#include "State.h"

class CState : public	CBase
{
public:
	static	CState*	addComponentTo(EObject* object, const map<int, State*>& stateMap);

	virtual void	commandProcess(int command, float deltaTime);

	virtual void	setProperty(int parameterTypeFlag, UParam parameter);

	virtual void	update(float delta);

	virtual CBase* clone(EObject* object);

	map<int, State*> getStateMap();

	~CState();

	int		getLastStateIndex();
private:
	void	changeState(int index);

	virtual bool	init();

	CREATE_FUNC(CState);

	int m_LastStateIndex;
private:
	map<int, State*>	m_stateMap;
	State*	m_CurrentState;
	State*	m_StateDie;
	bool m_ReadyToClimb;
};

#endif // !__COMPONENT_STATE_H__
