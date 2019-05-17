#ifndef CAIMove_h__
#define CAIMove_h__

#include "cocos2d.h"
#include "Entity/EObject.h"

class CMove;
class CSensor;

enum CAIMoveMode
{
	CAIMOVE_MODE_X	= 1,
	CAIMOVE_MODE_Y	= 2,
	CAIMOVE_MODE_BOTH_AXISTS = 3
};

class CAIMove : public CBase
{
public:
	// The create method for this AI Component.
	// NOTE: This component's chasing feature can only used after [object] have had CSensor (component).
	// Parameters:
	//	+ object: The EObject which is added this component as child.
	//
	//	+ activateRate: The activate rate, if activated, the EObject will have a command to move, else do nothing.
	//					"Do nothing" meaning it will give a command to stop all object's move.
	//					The component will have a random-round to activate this component each [roundDuration].
	//
	//	+ roundDuration: The time between 2 [random activate] times.
	//	+ randomMove:
	//		- TRUE: it's move left and right randomly by it's own - for Mobs.
	//		- FALSE: it's only move back after get out of range	- for Platforms.
	//	+ moveRange:	The moving range, object will move back after get out of range.
	//					The [moveRange] is [ZERO], which mean no range limit.
	//
	//	+ autoChase:	Use [object]'s sensor to detect any enemy in range and chasing them.
	//
	//	+ @disabled chasingRange: The range to chasing enemy after detect.
	//					Object will give up and back to start point after out of chasing range.
	//					The [chasingRange] is [ZERO], which mean it will keep chasing until death.
	static	void addComponentTo(EObject* object, float activateRate, float roundDuration = 1.0f, bool randomMove = false,
		Rect moveRange = MY_ZERO_RECT, bool autoChase = false, int mode = CAIMOVE_MODE_X);

	static CAIMove*	createComponent(EObject* object, float activateRate, float roundDuration = 1.0f, bool randomMove = false,
		Rect moveRange = MY_ZERO_RECT, bool autoChase = false, int mode = CAIMOVE_MODE_X);

	virtual void commandProcess(int command, float deltaTime);

	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

	void updateSensor();

	virtual CBase* clone(EObject* object);

	~CAIMove();

private:
	CREATE_FUNC(CAIMove);

	virtual bool init();

	void	giveMoveCommandForCurrentPosition();

	// To manage and check if it's random round.
	// The result is telling if the update method can continue.
	bool	computeIfMayContinueUpdate(float delta);

	// To update if object are chasing enemy.
	// The result is telling if the object is chasing or not.
	bool	chasingUpdate();

	// To update the object's moving if it being calm.
	// Not to be called if the object is chasing something.
	void	movingUpdate();

	// Test if the object is now moved out of move range
	bool	isOutOfMoveRange();


private:
	b2Body*		m_MoveRange;
	//Rect		m_ChaseRange;
	float		m_ActivateRate;
	float		m_RoundDuration;
	Rect		m_MoveRangeInRect;

	bool		m_IsAutoChasingEnemy;
	CSensor*	m_Sensor;

	bool		m_RandomMove;
	int			m_CurrentCommand;
	float		m_PassedTime;
	EObject*	m_Target;
	int			m_SensorFilterTag;
	b2Body*		m_Body;
	int			m_Mode;

	bool		m_IsOutOfMoveRange;
};

#endif // CAIMove_h__
