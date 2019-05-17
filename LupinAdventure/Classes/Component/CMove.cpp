#include "CMove.h"
#include "Config.h"
#include "UParam.h"

#define NUM_OF_ROUNDS_TO_TURN_BACK	25.0f

void CMove::commandProcess(int command, float deltaTime)
{
	Action* newAction = nullptr;

	switch (command)
	{
//	case COMMAND::COMPONENT::MOVE::BOTH:
//	{
//#pragma region Dung 2/7
//		if (m_TimeDelayCount > 0.0f)
//			return;
//		m_TimeDelayCount = m_TimeDelay;
//		m_Body->SetLinearVelocity(b2Vec2(m_Velocity.x * (-1.0f + rand() / RAND_MAX / 2.0f), m_JumpVelo.y * -1.0f + rand() / RAND_MAX / 2.0f));
//		/*auto randomX = (std::rand() % 60) - 60;
//		auto randomY = (std::rand() % 60) - 60;
//		if (m_Body != nullptr)
//		{
//			if (randomX >= 0.0f)
//			{
//				if (randomY)
//				m_Body->SetLinearVelocity(b2Vec2(m_Velocity.x, m_JumpVelo.y));
//			}
//			else
//			{
//				m_Body->SetLinearVelocity(b2Vec2(m_Velocity.x, m_JumpVelo.y));
//			}
//		}*/
//		break;
//	}
//#pragma endregion
	case COMMAND::COMPONENT::MOVE::LEFT:
	{
		if (m_Body != nullptr)
		{
			//if (m_Body->GetType() == b2BodyType::b2_kinematicBody)
			if (m_Body->GetLinearVelocity().x > -m_Velocity.x)
				m_Body->SetLinearVelocity(b2Vec2(-m_Velocity.x, m_Body->GetLinearVelocity().y));
			//else	m_Body->ApplyLinearImpulse(-m_Velocity, B2VEC2_ZERO, true);

			//if (m_Body->GetLinearVelocity().x < -m_Velocity.x)
			//	m_Body->SetLinearVelocity(b2Vec2(-m_Velocity.x, m_Body->GetLinearVelocity().y));
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::STOP_LEFT:
	{
		if (m_Body != nullptr)
		{
			if (m_Body->GetLinearVelocity().x < 0)
				m_Body->SetLinearVelocity(b2Vec2(0.0f, m_Body->GetLinearVelocity().y));
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::RIGHT:
	{
		/*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
		{
			CCLOG("MOVE RIGHT");
		}*/
		if (m_Body != nullptr)
		{
			//m_Body->ApplyLinearImpulse(m_Velocity, B2VEC2_ZERO, true);
			//if (m_Body->GetType() == b2BodyType::b2_kinematicBody)
			//{
			if (m_Body->GetLinearVelocity().x < m_Velocity.x)
				m_Body->SetLinearVelocity(b2Vec2(+m_Velocity.x, m_Body->GetLinearVelocity().y));
			//}
			//if (m_Body->GetLinearVelocity().x > m_Velocity.x)
			//{
			//	m_Body->SetLinearVelocity(b2Vec2(m_Velocity.x, m_Body->GetLinearVelocity().y));
			//}
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::STOP_RIGHT:
	{
		if (m_Body != nullptr)
		{
			if (m_Body->GetLinearVelocity().x > 0)
				m_Body->SetLinearVelocity(b2Vec2(0.0f, m_Body->GetLinearVelocity().y));
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::KNOCK_BACK:
		if (m_Body != nullptr)
		{
			m_Body->SetLinearVelocity(B2VEC2_ZERO);
			m_Body->SetLinearVelocity(b2Vec2(
				10.0f * m_Object->getScaleX(),
				10.0f));
		}
		break;

	case COMMAND::COMPONENT::MOVE::CHASE:
	{
		if (m_Target == nullptr)
		{
			//commandProcess(COMMAND::COMPONENT::MOVE::FLY, 0.0f);
			return;
		}

		// Process move X for landing and flying mobs
		int direction = m_Target->getPosition().x - m_Object->getPosition().x;

		if (direction * GET_SIGN(direction) > m_Velocity.x / 10 / BOX2D_RATIO)

			if (m_Body != nullptr)
				if (direction < 0)
				{
					m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::ANIMATION::FACE_LEFT);

					if (m_Body->GetType() == b2_kinematicBody || m_Body->GetGravityScale() == 0.0f)
					{
						if (m_Body->GetLinearVelocity().x > -m_Velocity.x)
							m_Body->SetLinearVelocity(b2Vec2(
							m_Body->GetLinearVelocity().x - m_Velocity.x / NUM_OF_ROUNDS_TO_TURN_BACK,
							m_Body->GetLinearVelocity().y));
					}
					else
						m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::MOVE::LEFT);
				}
				else
				{
					m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::ANIMATION::FACE_RIGHT);

					if (m_Body->GetType() == b2_kinematicBody || m_Body->GetGravityScale() == 0.0f)
					{
						if (m_Body->GetLinearVelocity().x < m_Velocity.x)
							m_Body->SetLinearVelocity(b2Vec2(
							m_Body->GetLinearVelocity().x + m_Velocity.x / NUM_OF_ROUNDS_TO_TURN_BACK,
							m_Body->GetLinearVelocity().y));
					}
					else
						m_Object->pushPrioritiedCommand(COMMAND::COMPONENT::MOVE::RIGHT);
				}

		if (m_TargetBody != nullptr)
			direction = m_TargetBody->GetPosition().y - m_Body->GetPosition().y;
		else	direction = (m_Target->getPosition().y - m_Object->getPosition().y) / BOX2D_RATIO;

		// Process move Y for flying mobs
		if (direction * GET_SIGN(direction) > m_Velocity.y / 10)
			if (m_Body->GetGravityScale() == 0.0f || m_Body->GetType() == b2_kinematicBody)
			{
				if (m_Body != nullptr)
				{
					if (direction < 0)
					{
						if (m_Body->GetLinearVelocity().y > -m_ClimbVelo.y)
							m_Body->SetLinearVelocity(b2Vec2(
							m_Body->GetLinearVelocity().x,
							m_Body->GetLinearVelocity().y - m_ClimbVelo.y / NUM_OF_ROUNDS_TO_TURN_BACK));
					}
					else
					{
						if (m_Body->GetLinearVelocity().y < m_ClimbVelo.y)
							m_Body->SetLinearVelocity(b2Vec2(
							m_Body->GetLinearVelocity().x,
							m_Body->GetLinearVelocity().y + m_ClimbVelo.y / NUM_OF_ROUNDS_TO_TURN_BACK));
					}
				}
			}
		break;
	}

	case COMMAND::COMPONENT::MOVE::UP:
	{
		if (m_Body != nullptr)
		{
			//m_Body->ApplyLinearImpulse(m_ClimbVelo, B2VEC2_ZERO, true);
			//if (m_Body->GetType() == b2BodyType::b2_kinematicBody)
			//{
				m_Body->SetLinearVelocity(m_ClimbVelo);
			//}
			//if (m_Body->GetLinearVelocity().y > m_ClimbVelo.y)
			//{
			//	m_Body->SetLinearVelocity(b2Vec2(m_Body->GetLinearVelocity().x, m_ClimbVelo.y));
			//}
		}
		break;
	}

	case COMMAND::COMPONENT::MOVE::STOP_UP:
	{
		if (m_Body != nullptr)
		{
			if (m_Body->GetLinearVelocity().y > 0)
				m_Body->SetLinearVelocity(b2Vec2(m_Body->GetLinearVelocity().x, 0.0f));
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::DOWN:
	{
		if (m_Body != nullptr)
		{
			//m_Body->ApplyLinearImpulse(-m_ClimbVelo, B2VEC2_ZERO, true);			
			//if (m_Body->GetType() == b2BodyType::b2_kinematicBody)
			//{
				m_Body->SetLinearVelocity(-m_ClimbVelo);
			//}
			//if (m_Body->GetLinearVelocity().y < -m_ClimbVelo.y)
			//{
			//	m_Body->SetLinearVelocity(b2Vec2(m_Body->GetLinearVelocity().x, -m_ClimbVelo.y));
			//}
		}
		break;
	}

	case COMMAND::COMPONENT::MOVE::STOP_DOWN:
	{
		if (m_Body != nullptr)
		{
			if (m_Body->GetLinearVelocity().y < 0)
				m_Body->SetLinearVelocity(b2Vec2(m_Body->GetLinearVelocity().x, 0.0f));
		}

		break;
	}

	case COMMAND::COMPONENT::MOVE::JUMP:
	{
		m_Body->SetLinearVelocity(m_JumpVelo);
		break;
	}

	case COMMAND::COMPONENT::MOVE::FLY:
	{
		if (m_Body != nullptr)
		{
			auto velo = b2Vec2(m_Velocity.x * m_Object->getScaleX() * FLIP_FACE_RIGHT.x, m_JumpVelo.y);
			m_Body->SetLinearVelocity(velo);
			//if (m_Body->GetType() == b2BodyType::b2_kinematicBody)
			//{
			//	m_Body->SetLinearVelocity(velo);
			//}
		}
		break;
	}

	case COMMAND::COMPONENT::MOVE::STOP_ALL:
	{
		if (m_Body != nullptr)
			m_Body->SetLinearVelocity(B2VEC2_ZERO);
		break;
	}
	default:
		break;
	}
}

CMove* CMove::addComponentTo(EObject* object, b2Vec2 moveVelo /*= B2VEC2_ZERO*/,
	b2Vec2 jumpVelo /*= B2VEC2_ZERO*/, b2Vec2 climbVelo /*= B2VEC2_ZERO*/, float timeDelay /*= 0.0f*/)
{
	auto component = createComponent(object, moveVelo, jumpVelo, climbVelo, timeDelay);
	if (component != nullptr)
	{
		object->addChild(component);
	}

	return component;
}

CMove* CMove::createComponent(EObject* object, b2Vec2 moveVelo /*= B2VEC2_ZERO*/, b2Vec2 jumpVelo /*= B2VEC2_ZERO*/, b2Vec2 climbVelo /*= B2VEC2_ZERO*/, float timeDelay /*= 0.0f*/)
{
	CMove* component = CMove::create();

	component->m_Object = object;
	component->m_Body = (b2Body*)object->getUserData();
	if (moveVelo.x != 0 || moveVelo.y != 0)
	{
		component->setProperty(UPARAM_TYPE_VEC2_CO_MOVE_VELO, UParam(moveVelo));
	}
	if (jumpVelo.x != 0 || jumpVelo.y != 0)
	{
		component->setProperty(UPARAM_TYPE_VEC2_CO_MOVE_JUMP_VELO, UParam(jumpVelo));
	}
	if (climbVelo.y != 0 || climbVelo.x != 0)
	{
		component->setProperty(UPARAM_TYPE_VEC2_CO_MOVE_CLIMB_VELO, UParam(climbVelo));
	}
	component->m_TimeDelay = timeDelay;

	return component;
}

bool CMove::init()
{
	if (!CBase::init())
	{
		return false;
	}

	this->setTag(TAG::COMPONENT::MOVE);

	m_Velocity			= B2VEC2_ZERO;
	m_ClimbVelo			= B2VEC2_ZERO;
	m_JumpVelo			= B2VEC2_ZERO;
	m_DirectedVelo		= 0.0f;
	m_Object			= nullptr;
	//m_TargetedActivate	= false;
	m_Target			= nullptr;
	m_BodyRealVelo		= B2VEC2_ZERO;
	m_TimeDelay			= 0.0f;
	m_TimeDelayCount	= 0.0f;

	scheduleUpdate();
	return true;
}

void CMove::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_VEC2_CO_MOVE_VELO:
		m_Velocity		= *parameter.uVec2Ptr;
		m_Velocity.y	= 0;
		m_ClimbVelo		= *parameter.uVec2Ptr;
		m_ClimbVelo.x	= 0;
		m_DirectedVelo	= GET_BIG(m_Velocity.x, m_Velocity.y);

		break;

	case UPARAM_TYPE_VEC2_CO_MOVE_CLIMB_VELO:
		m_ClimbVelo = *parameter.uVec2Ptr;
		break;

	case UPARAM_TYPE_VEC2_CO_MOVE_JUMP_VELO:
		m_JumpVelo = *parameter.uVec2Ptr;
		break;

	case UPARAM_TYPE_NODE:
		m_Target = parameter.uNodePtr;
		if (m_Target == nullptr)
			 m_TargetBody = nullptr;
		else m_TargetBody = (b2Body*)m_Target->getUserData();

		break;
	default:
		break;
	}
}

void CMove::update(float delta)
{
	//m_TimeDelayCount -= delta;
}

b2Vec2 CMove::getVelocity()
{
	b2Vec2 result = m_Velocity;
	result.x *= m_Object->getScaleX();

	return result;
}

CBase* CMove::clone(EObject* object)
{
	auto cloneOne = create();

	cloneOne->m_BodyRealVelo = m_BodyRealVelo;
	cloneOne->m_ClimbVelo = m_ClimbVelo;
	cloneOne->m_DirectedVelo = m_DirectedVelo;
	cloneOne->m_JumpVelo = m_JumpVelo;
	cloneOne->m_Target = m_Target;
	cloneOne->m_TargetBody = m_TargetBody;
	//cloneOne->m_TargetedActivate = m_TargetedActivate;
	cloneOne->m_Velocity = m_Velocity;

	cloneOne->m_Object = object;
	object->addChild(cloneOne);
	cloneOne->m_Body = (b2Body*)object->getUserData();

	return cloneOne;
}