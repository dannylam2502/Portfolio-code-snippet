// Author: Frederick Tran
// Created: 30/4/2015
// Move Component.

#ifndef CMove_h__
#define CMove_h__

#include "cocos2d.h"
#include "Component/CBase.h"
#include "Entity/EObject.h"
#include <vector>
USING_NS_CC;
using namespace std;

class CMove : public CBase
{
public:
	// Create a new move component then add to this object.
	// Replace for: object->addChild(CMove::create(velocity));
	static CMove* addComponentTo(EObject* object, 
		b2Vec2 moveVelo = B2VEC2_ZERO, b2Vec2 jumpVelo = B2VEC2_ZERO, b2Vec2 climbVelo = B2VEC2_ZERO, float timeDelay = 0.0f);

	static CMove* createComponent(EObject* object,
		b2Vec2 moveVelo = B2VEC2_ZERO, b2Vec2 jumpVelo = B2VEC2_ZERO, b2Vec2 climbVelo = B2VEC2_ZERO, float timeDelay = 0.0f);

	// command:
	//	+ COMMAND::COMPONENT::MOVE::LEFT		:move command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::RIGHT		:move command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::UP		:move command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::DOWN		:move command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::JUMP		:jump command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::DASH		:dash command, can be multi processed.
	//	+ COMMAND::COMPONENT::MOVE::FLY		:infinity move by velocity.
	//
	//	+ COMMAND::COMPONENT::MOVE::STOP_LEFT	:move command, stop all move to the left.
	//	+ COMMAND::COMPONENT::MOVE::STOP_RIGHT:move command, stop all move to the right.
	//	+ COMMAND::COMPONENT::MOVE::STOP_UP	:move command, stop all move upward.
	//	+ COMMAND::COMPONENT::MOVE::STOP_DOWN	:move command, stop all move downward.
	//	+ COMMAND::COMPONENT::MOVE::STOP_JUMP	:jump command, stop all jump action.
	//
	//	+ COMMAND::COMPONENT::MOVE::STOP_ALL	:stop all object's move actions.
	
	virtual void commandProcess(int command, float deltaTime);

	// parameterTypeFlag:
	//	+ UPARAM_TYPE_VEC3: set new velocity for the move component.
	//	+ UPARAM_TYPE_NODE: setup new target to move to and activate the targeted move mode.
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

private:
	CREATE_FUNC(CMove);

	virtual bool init();

	// The method to get the object's real velocity by where's it facing to. (left or right)
	b2Vec2	getVelocity();

	virtual CBase* clone(EObject* object);

private:
	b2Vec2			m_Velocity;		// Velocity in x-y axis.
	b2Vec2			m_JumpVelo;		// Velocity for Jump.
	b2Vec2			m_ClimbVelo;	// Velocity for Climb.
	float			m_DirectedVelo;	// Velocity in straight line.
	b2Body*			m_Body;
	
	// Targeted moving type.
	Node*			m_Target;
	b2Body*			m_TargetBody;

	b2Vec2			m_BodyRealVelo;
	// Dung, to control random
	float			m_TimeDelay;
	float			m_TimeDelayCount;
};

#endif // CMove_h__
