#ifndef __STATE_H__
#define __STATE_H__

#include "cocos2d.h"
#include "CBase.h"
#include <vector>

USING_NS_CC;

class State
{
public:
	virtual void	update(float delta);

	virtual void	enter() = 0;
	virtual void	exit() = 0;
	virtual int		commandProcess(int command) = 0;
	virtual State*	clone(EObject* object) = 0;

	vector<CBase*>	getComponentBoard();
	void			release();

	virtual	bool	checkIfStateChangeable(int newStateIndex);
	virtual void	doStateEffect();
	virtual bool	checkIfStateReadyToChangeTo();

	// Dung Added 29/6
	int				m_StateIndex;

protected:
	void cloneInfo(State* dest, EObject* destObject);

protected:
	vector<CBase*>	m_componentBoard;
	EObject*		m_Object;

};
#endif // !__STATE_H__
