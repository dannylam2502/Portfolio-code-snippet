#include "StateJump.h"
#include "Entity/EObject.h"

StateJump::StateJump()
{
	m_Object = nullptr;
	m_componentBoard.clear();
	m_StateIndex = STATE_INDEX::JUMP;
	m_TimeJump = 0.0f;
}

// created date: 29/6 just temp code
void StateJump::update(float delta)
{
	State::update(delta);
	/*for (EObject* obj : m_Object->getLinkedObjects())
	{
		auto body = (b2Body*)obj->getUserData();
		if (body->GetFixtureList()->GetFilterData().categoryBits & BITMASK::OBJECT::TILE)
		{
			m_Object->pushCommand(COMMAND::ENTITY::LANDING);
			return;
		}
	}*/
	m_TimeJump -= delta;
}

bool StateJump::checkIfStateChangeable(int newStateIndex)
{
	if (newStateIndex == STATE_INDEX::DIE || newStateIndex == STATE_INDEX::HIT)
	{
		return true;
	}
	if (m_TimeJump <= 0.0f)
	{
		m_TimeJump = 0.25f;
		return true;
	}
	return false;
}

StateJump::StateJump(EObject* object, const vector<CBase*>& components)
{
	m_Object = object;
	m_componentBoard = components;

	for (CBase* component : m_componentBoard)
		component->retain();
	m_StateIndex = STATE_INDEX::JUMP;
	m_TimeJump = 0.0f;
}

StateJump::~StateJump()
{
	m_Object = nullptr;
	for (CBase* component : m_componentBoard)
		component->release();
	m_componentBoard.clear();
}

void StateJump::enter()
{
	m_TimeJump = 0.3f;
	//CCLOG("STATE JUMP");
	b2Body* body = (b2Body*)m_Object->getUserData();
 	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::JUMP));
	//// Add 29/6 
	//m_Object->removeAllLinkedObject();
	//body->SetGravityScale(1.0f);
	// Dung 29/6
	//auto cState = (CState*)m_Object->getChildByTag(TAG::COMPONENT::STATE);
	/*if (cState->getLastStateIndex() == STATE_INDEX::CLIMB)
	{
		m_TimeJump = 0.0f;
		return;
	}*/
	//
}

void StateJump::exit()
{
	m_TimeJump = 0.0f;
}

int StateJump::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::COMPONENT::MOVE::LEFT:
		// Change flip to facing to left side
		m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);
		break;

	case COMMAND::COMPONENT::MOVE::RIGHT:
		// Change flip to facing to right side
		m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
		break;

	case COMMAND::ENTITY::CLIMB_START:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::CLIMB));
		break;

	case COMMAND::ENTITY::LANDING:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));
		break;
	default:
		break;
	}

	return command;
}

State* StateJump::clone(EObject* object)
{
	StateJump* cloneOne = new StateJump();
	State::cloneInfo(cloneOne, object);

	return cloneOne;
}
