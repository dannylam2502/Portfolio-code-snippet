// Author: Frederick Tran
// Created: 29/4/2015
// The base class for the object's components.

#ifndef CBase_h__
#define CBase_h__

#include "cocos2d.h"
#include <vector>
#include <map>
#include "UParam.h"

USING_NS_CC;
using namespace std;

class EObject;
class CBase : public Node
{
public:
	// Used to processing the given command(s).
	// Called in the CBase::update method.
	virtual void commandProcess(int command, float deltaTime) = 0;

	// Used to set a random properties for the component.
	// Defined by the flag(s).
	// parameterTypeFlag: UPARAM_TYPE_<TYPE>	- UParam.h
	virtual void setProperty(int parameterTypeFlag, UParam parameter) = 0;

	// Used to clone the component into another one.
	virtual CBase* clone(EObject* object) = 0;

	void	setObject(Node* object);

protected:
	EObject*		m_Object;
};

#endif // CBase_h__
