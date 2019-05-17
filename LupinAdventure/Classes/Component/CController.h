// Author: Frederick Tran
// Created: 30/4/2015
// Controller Component for Character.

#ifndef CController_h__
#define CController_h__

#include "cocos2d.h"
#include "CBase.h"
#include <vector>
#include "Config.h"
#include "Singleton/SKeyboard.h"
#include "Entity/EObject.h"
USING_NS_CC;
using namespace std;

class CController : public CBase
{
public:
	static	void	addComponentTo(EObject* object, vector<CControllerKeySet> keyCommandMap);
	static	CController* create(EObject* object, vector<CControllerKeySet> keyCommandMap);

	virtual void	commandProcess(int command, float deltaTime);

	// parameterTypeFlag:
	//	+ UPARAM_TYPE_KEYSET: add new control key to the map.
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

	virtual bool init();

	virtual CBase* clone(EObject* object);

private:
	CREATE_FUNC(CController);


private:
	vector<CControllerKeySet>	m_KeyCommandMap;
};

#endif // CController_h__