// Author: Frederick Tran
// Created: 21/5/2015
// To do: Give command to CAttack component.

#ifndef CAIAttack_h__
#define CAIAttack_h__
#include <vector>
#include "cocos2d.h"
#include "Config.h"
#include "Component/CAttack.h"
USING_NS_CC;
using namespace std;

class EObject;
class CSensor;

class CAIAttack : public CBase
{
public:
	// To create a whole new component then add it into [object]
	// NOTE: The [auto attack enemy in range] feature can only activated after [object] have been initialized CSensor component.
	// Parameter:
	//	+ object: The object which is added this component as child.
	//	+ reloadTime: The waiting time before attack again.
	static void addComponentTo(EObject* object, float reloadTime);

	static CAIAttack* createComponent(EObject* object, float reloadTime);

	virtual void commandProcess(int command, float deltaTime);

	// To process commands
	// Parameter:
	//	+ parameterTypeFlag:
	//		- UPARAM_TYPE_NODE: set target to attack
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

	virtual bool init();

	virtual CBase* clone(EObject* object);

private:
	CREATE_FUNC(CAIAttack);

	void updateSensor();

private:
	CSensor*	m_Sensor;
	float		m_ReloadTime;
	float		m_TimeCounter;

	EObject*	m_Target;
};

#endif // CAIAttack_h__