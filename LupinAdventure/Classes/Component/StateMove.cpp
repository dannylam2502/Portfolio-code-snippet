#include "StateMove.h"
#include "Entity/EObject.h"

StateMove::StateMove()
{
	m_Object = nullptr;
	m_Body = nullptr;
	m_componentBoard.clear();
	m_StateIndex = STATE_INDEX::MOVE;
}

StateMove::StateMove(EObject* object, const vector<CBase*>& components)
{
	m_Object = object;
	m_componentBoard = components;
	m_Body = (b2Body*)object->getUserData();

	for (CBase* component : m_componentBoard)
		component->retain();

	m_StateIndex = STATE_INDEX::MOVE;
}

StateMove::~StateMove()
{
	m_Object = nullptr;
	for (CBase* component : m_componentBoard)
		component->release();
	m_componentBoard.clear();
}

void StateMove::enter()
{
	/*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
	{
		CCLOG("STATE MOVE");
	}*/

	m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::MOVE));
	/*auto body = (b2Body*)m_Object->getUserData();
	body->SetGravityScale(1.0f);*/
}

void StateMove::exit()
{
}

int StateMove::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::COMPONENT::MOVE::LEFT:
		// Change flip to facing to left side
		m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);
		m_MoveFlag = true;
		break;

	case COMMAND::COMPONENT::MOVE::RIGHT:
		// Change flip to facing to right side
		m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);
		m_MoveFlag = true;
		break;

	case COMMAND::ENTITY::CLIMB_START:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::CLIMB));
		break;

	case COMMAND::COMPONENT::MOVE::JUMP:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP));
		break;

	case COMMAND::COMPONENT::ATTACK::ACTIVATE:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::ATTACK));
		break;
	default:
		break;
	}
	return command;
}

void StateMove::update(float delta)
{
	State::update(delta);

	if (m_Body->GetLinearVelocity().y != 0.0f)
	{
		for (EObject* object : m_Object->getLinkedObjects())
		{
			if (isStringEqual(object->getName().c_str(), NAME::PLATFORM_POLYGON) ||
				isStringEqual(object->getName().c_str(), NAME::PLATFORM_DYNAMIC))
			{
				return;
			}
		}

		if (m_Object->getLinkedObjects().size() == 0)
		{
			m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP);
		}
		return;
	}

	if (!m_MoveFlag)
	{
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));
	}

	m_MoveFlag = false;
}

State* StateMove::clone(EObject* object)
{
	StateMove* cloneOne = new StateMove();
	State::cloneInfo(cloneOne, object);

	cloneOne->m_Body = (b2Body*)object->getUserData();

	return cloneOne;
}
