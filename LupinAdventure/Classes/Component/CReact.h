#ifndef CReact_h__
#define CReact_h__

#include "Component/CBase.h"

class CReact : public CBase
{
public:
	static CReact* addComponentTo(EObject* object, Action* actionOnTouch);

	virtual void commandProcess(int command, float deltaTime);

	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual CBase* clone(EObject* object);

	virtual void update(float delta);

private:
	CREATE_FUNC(CReact);

private:
	Action* m_ActionOnTouch;
};

#endif // CReact_h__
