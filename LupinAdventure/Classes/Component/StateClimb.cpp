#include "StateClimb.h"
#include "Entity/EObject.h"
#include "Utils.h"

StateClimb::StateClimb()
{
	m_Object = nullptr;
	m_componentBoard.clear();
	m_StateIndex = STATE_INDEX::CLIMB;
}

StateClimb::StateClimb(EObject* object, vector<CBase*> components)
{
	m_Object = object;
	m_componentBoard = components;

	for (CBase* component : m_componentBoard)
		component->retain();
	m_StateIndex = STATE_INDEX::CLIMB;
}

StateClimb::~StateClimb()
{
	m_Object = nullptr;
	for (CBase* component : m_componentBoard)
		component->release();
	m_componentBoard.clear();
}

void StateClimb::enter()
{
	b2Body* body = (b2Body*)m_Object->getUserData();
	body->SetGravityScale(0.0f);
	body->SetLinearVelocity(B2VEC2_ZERO);
	auto filterData = body->GetFixtureList()->GetFilterData();
	filterData.maskBits = filterData.maskBits ^ BITMASK::OBJECT::TILE;
	body->GetFixtureList()->SetFilterData(filterData);

	for (EObject* object : m_Object->getLinkedObjects())
	{
		if (isStringEqual(object->getName().c_str(), NAME::PLATFORM_LADDER) ||
			isStringEqual(object->getName().c_str(), NAME::PLATFORM_ROPE))
		{
			if (isStringEqual(object->getName().c_str(), NAME::PLATFORM_LADDER))
			{
				m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::LADDER));
			}
			else
			{
				m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::ROPE));
			}

			// This not be confused between object and m_Object
			// Stay at center of the robe at object's height
			m_Object->setBodyPosition(object->getPosition().x, m_Object->getPosition().y);
		}
	}
}

void StateClimb::exit()
{
	b2Body* body = (b2Body*)m_Object->getUserData();
	body->SetGravityScale(1.0f);
	auto filterData = body->GetFixtureList()->GetFilterData();
	filterData.maskBits = filterData.maskBits | BITMASK::OBJECT::TILE;
	body->GetFixtureList()->SetFilterData(filterData);
}

int StateClimb::commandProcess(int command)
{
	switch (command)
	{
	case COMMAND::ENTITY::CLIMB_STOP:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));
		break;

	case COMMAND::COMPONENT::MOVE::UP:
	{
		break;
	}

	case COMMAND::COMPONENT::MOVE::DOWN:
	{
		break;
	}

	case COMMAND::COMPONENT::MOVE::JUMP:
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP));
		break;

	default:
		break;
	}

	return command;
}

State* StateClimb::clone(EObject* object)
{
	StateClimb* cloneOne = new StateClimb();
	State::cloneInfo(cloneOne, object);

	return cloneOne;
}

bool StateClimb::checkIfStateReadyToChangeTo()
{
	for (EObject* object : m_Object->getLinkedObjects())
	{
		if (isStringEqual(object->getName().c_str(), NAME::PLATFORM_LADDER) ||
			isStringEqual(object->getName().c_str(), NAME::PLATFORM_ROPE))
		{
			return true;
		}
	}

	return false;
}
