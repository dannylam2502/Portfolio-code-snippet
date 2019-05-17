#include "EObject.h"
#include "Config.h"
#include <exception>

bool EObject::init()
{
	if (!Node::init())
	{
		return false;
	}

	m_CommandQueue.clear();
	m_Components.clear();
	m_StateManager = nullptr;
	m_IsActive = true;

	scheduleUpdate();

	return true;
}

void EObject::update(float delta)
{
	if (!m_IsActive)
	{
		return;
	}

	int command = popCommand();

	while (command != 0)
	{
		commandProcess(command);

		command = popCommand();
	}
}

void EObject::pushCommand(int command)
{
	m_CommandQueue.push_back(command);
}

int EObject::popCommand()
{
	if (m_CommandQueue.size() == 0)
	{
		return 0;
	}

	int result = m_CommandQueue.front();
	m_CommandQueue.erase(m_CommandQueue.begin());

	return result;
}

void EObject::addChild(CBase * child)
{
	this->Node::addChild((Node*)child);

	m_Components.push_back(child);
	addPropertiesSetableComponent(child);
}

void EObject::addChild(Node * child)
{
	this->Node::addChild(child);
}

void EObject::addChild(CState *child)
{
	this->Node::addChild((Node*)child);

	m_Components.push_back(child);
	m_StateManager = child;
	addPropertiesSetableComponent(child);

	for (pair<int, State*> componentPair : child->getStateMap())
		for (CBase* component : componentPair.second->getComponentBoard())
		{
			addPropertiesSetableComponent(component);
		}
}

void EObject::setProperty(int componentTypeFlag, int parameterTypeFlag, UParam parameter)
{
	CBase* targetComponent = nullptr;
	
	if (componentTypeFlag == TAG::COMPONENT::NONE)
	{
		switch (parameterTypeFlag)
		{
		case UPARAM_TYPE_COMMAND_PAIR:
			m_AutoCommandByCommandMap.push_back(std::pair<int, int>(*parameter.uCommandPair));
			break;

		case UPARAM_TYPE_ACTION_PAIR:
			m_AutoActionByCommandMap.push_back(std::pair<int, Action*>(*parameter.uActionPair));
			parameter.uActionPair->second->retain();
			break;
		}
	}

	for (CBase* component : m_PropertiesSetableComponents)
		if (component->getTag() == componentTypeFlag)
		{
			targetComponent = component;
			break;
		}

	if (targetComponent != nullptr)
	{
		targetComponent->setProperty(parameterTypeFlag, parameter);
	}
}

void EObject::setFlip(Vec2 newState)
{
	setScale(GET_SIGN(newState.x), GET_SIGN(newState.y));
}

void EObject::setLinkedObject(EObject* object)
{
	for (EObject* obj : m_RelatedObjects)
	{
		if (object == obj)
		{
			return;
		}
	}

	m_RelatedObjects.push_back(object);

	if (isStringEqual(object->getName().c_str(), NAME::PLATFORM_LADDER) ||
		isStringEqual(object->getName().c_str(), NAME::PLATFORM_ROPE))
	{
		pushCommand(COMMAND::ENTITY::CLIMB_READY);
	}
}

vector<EObject*> EObject::getLinkedObjects()
{
	return m_RelatedObjects;
}

void EObject::removeLinkedObject(EObject* object)
{
	FOR(m_RelatedObjects.size())
		if (m_RelatedObjects.at(i) == object)
		{
			m_RelatedObjects.erase(m_RelatedObjects.begin() + i);
			return;
		}
}

void EObject::setBodyPosition(float posX, float posY)
{
	b2Body* body = (b2Body*)getUserData();
	if (body != nullptr)
	{
		body->SetTransform(b2Vec2(posX / BOX2D_RATIO, posY / BOX2D_RATIO), body->GetAngle());
	}

	this->setPosition(posX, posY);
}

void EObject::removeAllLinkedObject()
{
	if (!m_RelatedObjects.empty())
	{
		m_RelatedObjects.clear();
	}
}

EObject::~EObject()
{
	for (pair<int, Action*> actionPair : m_AutoActionByCommandMap)
	{
		actionPair.second->release();
	}
	m_AutoActionByCommandMap.clear();
	m_AutoCommandByCommandMap.clear();
}

void EObject::changeBodyShape(float posX, float posY, float height, float width)
{
	b2Body* body = (b2Body*)getUserData();

	if (body == nullptr)	return;

	setBodyPosition(posX, posY);

	b2PolygonShape shape;
	b2FixtureDef bodyMaterial;

	// Box2D get 1/2 of width and height to set the object bounding box.
	shape.SetAsBox(width / BOX2D_RATIO / 2, height / BOX2D_RATIO / 2);
	auto fixture = body->GetFixtureList();
	bodyMaterial.shape = &shape;
	bodyMaterial.density = fixture->GetDensity();
	bodyMaterial.friction = fixture->GetFriction();
	bodyMaterial.restitution = fixture->GetRestitution();
	bodyMaterial.filter = fixture->GetFilterData();
	bodyMaterial.isSensor = fixture->IsSensor();

	body->DestroyFixture(body->GetFixtureList());
	body->CreateFixture(&bodyMaterial);
	
	setContentSize(Size(width, height));
}

void EObject::setActive(bool isActive)
{
	auto body	= (b2Body*)getUserData();
	m_IsActive	= isActive;

	if (isActive)
	{
		if (body != nullptr)	body->SetActive(true);
		pushCommand(COMMAND::COMPONENT::ANIMATION::VISIBLE);
	}
	else
	{
		if (body != nullptr)	body->SetActive(false);
		pushCommand(COMMAND::COMPONENT::ANIMATION::INVISIBLE);
	}
}

void EObject::setActiveSlow(bool isActive, float countDown)
{
	int command = (isActive ? COMMAND::ENTITY::ACTIVATE : COMMAND::ENTITY::DEACTIVATE);

	auto waitAction = DelayTime::create(countDown);
	auto activeAction = CallFunc::create(std::bind(&EObject::commandProcess, this, command));
	runAction(Sequence::create(waitAction, activeAction, nullptr));
}

void EObject::addPropertiesSetableComponent(CBase* component)
{
	for (CBase* com : m_PropertiesSetableComponents)
	{
		if (component == com)
		{
			return;
		}
	}

	m_PropertiesSetableComponents.push_back(component);
}

void EObject::pushPrioritiedCommand(int command)
{
	m_CommandQueue.insert(m_CommandQueue.begin(), command);
}

void EObject::commandProcess(int command)
{
	for (pair<int, int> commandPair : m_AutoCommandByCommandMap)
	{
		if (commandPair.first == command)
			pushCommand({ commandPair.second });
	}

	for (pair<int, Action*> actionPair : m_AutoActionByCommandMap)
	{
		if (actionPair.first == command)
			runAction(actionPair.second->clone());
	}

	switch (command)
	{
	case COMMAND::ENTITY::ACTIVATE:
		setActive(true);
		break;

	case COMMAND::ENTITY::DEACTIVATE:
		setActive(false);

	default:
		for (CBase* component : m_Components)
			component->commandProcess(command, 0.0f);
		break;
	}
}

vector<CBase*>	EObject::getComponent()
{
	return this->m_Components;
}

void EObject::clearAllCommand()
{
	m_CommandQueue.clear();
}