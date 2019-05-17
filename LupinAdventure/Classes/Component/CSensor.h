#ifndef __C_SENSOR_H__
#define __C_SENSOR_H__

#include "cocos2d.h"
#include "CBase.h"
#include "Entity\EObject.h"
#include "Component\CAIAttack.h"
#include "Component\CAIMove.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

enum	SensorMode
{
	SENSOR_MOVE,
	SENSOR_ATTACK
};

class CSensor : public CBase
{
public:
	// Create core of Object then add into Object.
	// Fred - 20/5/2015: result is added component.
	// Parameter:
	//	+ radius:	The radius of the circle that sensor scan.
	//	+ filtedCategoryBitmask: Detected object is which is own a category bitmask that "and" with this filter are not zero
	static CSensor* addComponentTo(EObject* object, int radius, int filtedCategoryBitmask);
	static CSensor* addComponentTo(EObject* object, float sensorHeight,float sensorWidth, int filtedCategoryBitmask);

	// command:
	virtual void commandProcess(int command, float deltaTime);

	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual bool init();

	virtual void update(float delta);

	// Fred - 20/5/2015:
	// To get the object by its tag.
	vector<EObject*>	getObjectInSensor();

	// Fred - 20/5/2015:
	// To get the nearest EObject in sensor with the tag.
	EObject*	getNearestObjectInSensor();

	CREATE_FUNC(CSensor);
	~CSensor();

	virtual CBase* clone(EObject* object);

	bool	checkIfObjectInSensor(EObject* object);


private:
	int					m_FiltedCategoryBitmask;
	EObject*			m_Sensor;
	b2Body*				m_SensorBody;
};

#endif // !__C_SENSOR_H__
