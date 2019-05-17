// Author: Frederick Tran
// Created: 29/4/2015
// The base class for the game object's entities.

#ifndef EObject_h__
#define EObject_h__

#include "cocos2d.h"
#include <vector>
#include "Component/CBase.h"
#include "Component/CSensor.h"
#include "Component/CState.h"
#include "Box2D/Box2D.h"
USING_NS_CC;
using namespace std;	

class EObject : public Node
{
public:
	CREATE_FUNC(EObject);
	~EObject();

	// To give a command to this object.
	// The command is processed by the object's components.
	void pushCommand(int command);
	// The prioritied command will be process right after push.
	void pushPrioritiedCommand(int command);

	// To get a command and remove it from the command queue.
	// Return 0 if command queue is empty.
	int popCommand();

	// To set a parameter through a UParam struct by the flags.
	// parameterTypeFlag: UPARAM_TYPE_<TYPE>	- UParam.h
	// componentTypeFlag: TAG::COMPONENT::<TYPE> / TAG_EOBJECT	- Config.h
	void setProperty(int componentTypeFlag, int parameterTypeFlag, UParam parameter);
	
	void setFlip(Vec2 newState);

	virtual void update(float delta);

	virtual bool init();

	// Implement the addChild method for the Object's Components.
	virtual void addChild(CBase * child);

	virtual void addChild(CState *child);

	virtual void addChild(Node * child);

	// To suspend object, the suspended object will not be drawn, body update neither
	void	setActiveSlow(bool isActive, float countDown);
	
	void	setLinkedObject(EObject* object);
	void	removeLinkedObject(EObject* object);
	void	removeAllLinkedObject();

	void	setBodyPosition(float posX, float posY);
	void	changeBodyShape(float posX, float posY,	float height, float width);

	void	addPropertiesSetableComponent(CBase* component);

	vector<EObject*>	getLinkedObjects();

	void	commandProcess(int command);

	void	clearAllCommand();

	vector<CBase*>	getComponent();

private:
	void	setActive(bool isActive);

private:
	vector<int>		m_CommandQueue;
	vector<CBase*>	m_Components;
	vector<CBase*>	m_PropertiesSetableComponents;
	CState*			m_StateManager;

	vector<EObject*>	m_RelatedObjects;

	vector<std::pair<int, int>>	m_AutoCommandByCommandMap;
	vector<std::pair<int, Action*>> m_AutoActionByCommandMap;

	bool	m_IsActive;
};

#endif // EObject_h__