#include "CSensor.h"
#include "Singleton\SFactory.h"

CSensor* CSensor::addComponentTo(EObject* object, int radius, int filtedCategoryBitmask)
{
	CSensor* component = CSensor::create();

	component->m_Object = object;
	component->m_FiltedCategoryBitmask = filtedCategoryBitmask;

	b2Filter filter;
	filter.categoryBits = BITMASK::OBJECT::ALL;
	filter.maskBits = filtedCategoryBitmask;
	
	component->m_SensorBody = SFactory::createSensor(
		component->m_Sensor,
		object->getPositionX(),
		object->getPositionY(),
		radius, filter, b2_kinematicBody);
	component->m_SensorBody->SetSleepingAllowed(true);

	component->m_Sensor->setUserData(component->m_SensorBody);

	object->addChild(component);

	return component;
}

CSensor* CSensor::addComponentTo(EObject* object, float sensorHeight, float sensorWidth, int filtedCategoryBitmask)
{
	CSensor* component = CSensor::create();

	component->m_Object = object;
	component->m_FiltedCategoryBitmask = filtedCategoryBitmask;

	b2Filter filter;
	filter.categoryBits = BITMASK::OBJECT::ALL;
	filter.maskBits = filtedCategoryBitmask;

	component->m_SensorBody = SFactory::createSensor(
		component->m_Sensor,
		object->getPositionX(),
		object->getPositionY(),
		sensorHeight, sensorWidth, filter, b2_kinematicBody);
	component->m_SensorBody->SetSleepingAllowed(true);

	component->m_Sensor->setUserData(component->m_SensorBody);

	object->addChild(component);

	return component;
}

void CSensor::commandProcess(int command, float deltaTime)
{
}

// Set Property of component.
void CSensor::setProperty(int parameterTypeFlag, UParam parameter)
{
}

bool CSensor::init()
{
	if (!CBase::init())
	{
		return false;
	}

	m_Object = nullptr;
	m_Sensor = EObject::create();
	m_Sensor->retain();

	this->setTag(TAG::COMPONENT::SENSOR);

	scheduleUpdate();

	return true;
}

void CSensor::update(float delta)
{
	m_Sensor->setBodyPosition(m_Object->getPositionX(), m_Object->getPositionY());
}

vector<EObject*>	CSensor::getObjectInSensor()
{
	return m_Sensor->getLinkedObjects();
}

EObject* CSensor::getNearestObjectInSensor()
{
	EObject* result = nullptr;

	for (EObject* object : getObjectInSensor())
	{
		if (result == nullptr)
			result = object;
		else
		{
			auto distanceToCurrentResult = m_Object->getPosition().distance(result->getPosition());
			auto distanceToCurrentObject = m_Object->getPosition().distance(object->getPosition());

			if (distanceToCurrentObject < distanceToCurrentResult)
				result = object;
		}
	}

	return result;
}

CBase* CSensor::clone(EObject* object)
{
	throw std::logic_error("The method or operation is not implemented.");
}

CSensor::~CSensor()
{
	//m_Sensor->release();
	//m_SensorBody->SetUserData(nullptr);
	//m_SensorBody->GetWorld()->DestroyBody(m_SensorBody);
	//SFactory::addToDestroyer(m_Sensor);
	//m_SensorBody->SetActive(false);
	//m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::SENSOR::REMOVED);
}

bool CSensor::checkIfObjectInSensor(EObject* object)
{
	for (EObject* detectedOne : getObjectInSensor())
	{
		if (detectedOne == object)
		{
			return true;
		}
	}

	return false;
}
