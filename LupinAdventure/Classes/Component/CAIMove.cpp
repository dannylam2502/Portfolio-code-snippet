#include "Component/CAIMove.h"
#include "Singleton/SFactory.h"
#include "Component/CMove.h"
#include "Component/CSensor.h"

void CAIMove::commandProcess(int command, float deltaTime)
{
	// Head to target
	switch (command)
	{
	case COMMAND::COMPONENT::AI_MOVE::STOP_CHASE:
	{
		m_Object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_NODE, UParam((Node*)nullptr));
		break;
	}
	}
}

void CAIMove::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
	case UPARAM_TYPE_RECT:
		auto bitmask = m_MoveRange->GetFixtureList()->GetFilterData();
		//auto bodyType = m_MoveRange->GetFixtureList()->GetBody()->GetType();
		//m_MoveRange->GetWorld()->DestroyBody(m_MoveRange);
		m_MoveRange->DestroyFixture(m_MoveRange->GetFixtureList());
		m_MoveRangeInRect.operator=(*parameter.uRect);

		b2PolygonShape shape;
		b2FixtureDef bodyMaterial;
		float width = m_MoveRangeInRect.size.width / BOX2D_RATIO;
		float height = m_MoveRangeInRect.size.height / BOX2D_RATIO;
		
		// Set Position
		m_MoveRange->SetTransform(b2Vec2(
			m_MoveRangeInRect.origin.x / BOX2D_RATIO, 
			m_MoveRangeInRect.origin.y / BOX2D_RATIO), 
			m_MoveRange->GetAngle());

		// Create new range as fixture
		shape.SetAsBox(width / 2, height / 2);
		bodyMaterial.shape = &shape;
		bodyMaterial.isSensor = true;
		bodyMaterial.filter = bitmask;

		m_MoveRange->CreateFixture(&bodyMaterial);
		m_MoveRange->SetFixedRotation(true);

		m_MoveRangeInRect.origin.x -= m_MoveRangeInRect.size.width / 2;
		m_MoveRangeInRect.origin.y -= m_MoveRangeInRect.size.height / 2;
		break;
	}
}

void CAIMove::addComponentTo(EObject* object, float activateRate, float roundDuration/* = 1.0f*/, bool randomMove /*= false*/,
	Rect moveRange /*= MY_ZERO_RECT*/, bool autoChase /*= false*/, int mode /*= CAIMOVE_MODE_BOTH_AXISTS*/)
{
	auto component = createComponent(object, activateRate, roundDuration, randomMove, moveRange, autoChase, mode);

	if (component == nullptr)
	{
		return;
	}

	component->m_Object->addChild(component);
	component->giveMoveCommandForCurrentPosition();
}

CAIMove* CAIMove::createComponent(EObject* object, 
	float activateRate, 
	float roundDuration /*= 1.0f*/, 
	bool randomMove /*= false*/, 
	Rect moveRange /*= Rect::ZERO*/, 
	bool autoChase /*= false*/,
	int mode /*= CAIMOVE_MODE_X*/)
{
	if (object == nullptr)
	{
		return nullptr;
	}

	CAIMove* component = CAIMove::create();
	component->m_Object = object;
	component->m_IsAutoChasingEnemy = autoChase;
	component->m_ActivateRate = activateRate;
	component->m_RoundDuration = roundDuration;
	component->m_RandomMove = randomMove;
	component->m_Body = (b2Body*)object->getUserData();
	component->m_MoveRangeInRect = moveRange;
	component->m_Mode = mode;
	component->m_IsOutOfMoveRange = false;

	// Auto chasing initialize if activated
	// If there are no sensor in [object], it will be automatically deactivated.
	if (component->m_IsAutoChasingEnemy)
	{
		component->m_Sensor = (CSensor*)component->m_Object->getChildByTag(TAG::COMPONENT::SENSOR);

		if (component->m_Sensor == nullptr)
			component->m_IsAutoChasingEnemy = false;
	}

	if (component->m_MoveRangeInRect.equals(MY_ZERO_RECT))
	{
		return component;
	}

	b2Filter filter;
	filter.categoryBits = 0;
	filter.groupIndex = 0;
	filter.maskBits = 0;

	component->m_MoveRange = SFactory::createSensor(nullptr,
		moveRange.getMidX(),
		moveRange.getMidY(),
		moveRange.size.height,
		moveRange.size.width,
		filter, b2_staticBody);

	return component;
}

void CAIMove::update(float delta)
{
	m_Object->pushCommand(m_CurrentCommand);
	m_IsOutOfMoveRange = isOutOfMoveRange();
	if (m_IsAutoChasingEnemy)
	{
		if (chasingUpdate())
			return;
	}

	if (m_RandomMove)
		if (!computeIfMayContinueUpdate(delta))
			return;

	movingUpdate();
}

bool CAIMove::init()
{
	if (!CBase::init())
	{
		return false;
	}
	setTag(TAG::COMPONENT::AI_MOVE);

	m_PassedTime = 0.0f;
	m_Sensor = nullptr;
	m_IsAutoChasingEnemy = false;
	m_MoveRange = nullptr;
	m_MoveRangeInRect = MY_ZERO_RECT;
	m_Target = nullptr;
	m_CurrentCommand = COMMAND::ENTITY::NONE;
	m_IsOutOfMoveRange = false;

	scheduleUpdate();

	return true;
}

void CAIMove::giveMoveCommandForCurrentPosition()
{
	if (m_MoveRange == nullptr)
	{
		m_CurrentCommand = COMMAND::COMPONENT::MOVE::FLY;
		return;
	}

	if (m_Mode == CAIMOVE_MODE_X)
	{
		if (m_MoveRange->GetPosition().x < m_Body->GetPosition().x)
				m_CurrentCommand = COMMAND::COMPONENT::MOVE::LEFT;
		else	m_CurrentCommand = COMMAND::COMPONENT::MOVE::RIGHT;

		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE));
	}

	if (m_Mode == CAIMOVE_MODE_Y)
	{
		if (m_MoveRange->GetPosition().y < m_Body->GetPosition().y)
				m_CurrentCommand = COMMAND::COMPONENT::MOVE::DOWN;
		else	m_CurrentCommand = COMMAND::COMPONENT::MOVE::UP;

		//m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::JUMP));
		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE));
	}
#pragma region Dung 2/7
	if (m_Mode == CAIMOVE_MODE_BOTH_AXISTS)
	{
		//m_CurrentCommand = COMMAND::COMPONENT::MOVE::BOTH;

		/*if (!m_MoveRangeInRect.containsPoint(m_Object->getPosition()))
		{*/
			if (std::rand() % 2 > 0)
			{
				if (m_MoveRange->GetPosition().x < m_Body->GetPosition().x)
					m_CurrentCommand = COMMAND::COMPONENT::MOVE::LEFT;
				else	m_CurrentCommand = COMMAND::COMPONENT::MOVE::RIGHT;
			}
			else
			{
				if (m_MoveRange->GetPosition().y < m_Body->GetPosition().y)
					m_CurrentCommand = COMMAND::COMPONENT::MOVE::DOWN;
				else	m_CurrentCommand = COMMAND::COMPONENT::MOVE::UP;
			}
		//}

		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE));
	}
#pragma endregion
	//m_Object->pushCommand((COMMAND::COMPONENT::ANIMATION::CHANGE_TO::MOVE));
	//m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE);
	//m_Object->pushCommand(m_CurrentCommand);
}

bool CAIMove::computeIfMayContinueUpdate(float delta)
{
	m_PassedTime += delta;

	if (m_Target != nullptr)
	{
		return false;
	}

	if (m_IsOutOfMoveRange)
	{
		// Reset time counter
		m_PassedTime = 0.0f;
		return true;
	}

	if (m_PassedTime >= m_RoundDuration)
	{
		// Reset time counter
		m_PassedTime = 0.0f;

		// If it's always active
		if (m_ActivateRate >= 1.0f)
			return true;

		// Random round process
		if (CCRANDOM_0_1() >= m_ActivateRate)
		{
			if (m_CurrentCommand != COMMAND::COMPONENT::MOVE::STOP_ALL)
			{
				m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));
				m_CurrentCommand = COMMAND::ENTITY::NONE;
			}
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool CAIMove::chasingUpdate()
{
	updateSensor();

	if (m_Target != nullptr)
	{
		if (m_IsOutOfMoveRange && !m_Sensor->checkIfObjectInSensor(m_Target))
		{
			m_Target = nullptr;
			m_Object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_NODE, UParam((EObject*)nullptr));
			m_CurrentCommand = COMMAND::ENTITY::NONE;
			return false;
		}

		m_Object->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::MOVE));
		m_Object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_NODE, UParam(m_Target));
		m_CurrentCommand = COMMAND::COMPONENT::MOVE::CHASE;
		return true;
	}
	else 
	{
		m_Object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_NODE, UParam((EObject*)nullptr));
		return false;
	}
}

void CAIMove::movingUpdate()
{
	if (m_IsOutOfMoveRange || m_CurrentCommand == COMMAND::ENTITY::NONE || m_CurrentCommand == COMMAND::COMPONENT::MOVE::CHASE)
	{
		giveMoveCommandForCurrentPosition();
	}
}

void CAIMove::updateSensor()
{
	m_Target = m_Sensor->getNearestObjectInSensor();
}

CBase* CAIMove::clone(EObject* object)
{
	auto cloneOne = create();
	
	cloneOne->m_ActivateRate = m_ActivateRate;
	//cloneOne->m_ChaseRange = m_ChaseRange;
	cloneOne->m_CurrentCommand = m_CurrentCommand;
	cloneOne->m_IsAutoChasingEnemy = m_IsAutoChasingEnemy;
	cloneOne->m_MoveRangeInRect = m_MoveRangeInRect;
	cloneOne->m_PassedTime = m_PassedTime;
	cloneOne->m_RandomMove = m_RandomMove;
	cloneOne->m_Target = m_Target;
	cloneOne->m_SensorFilterTag = m_SensorFilterTag;
	cloneOne->m_IsOutOfMoveRange = m_IsOutOfMoveRange;

	cloneOne->m_Object = object;
	object->addChild(cloneOne);
	cloneOne->m_Body = (b2Body*)object->getUserData();
	cloneOne->m_Sensor = (CSensor*)m_Sensor->clone(object);

	b2Filter filter;
	filter.categoryBits = 0;
	filter.groupIndex = 0;
	filter.maskBits = 0;

	cloneOne->m_MoveRange = SFactory::createSensor(nullptr, 
		m_MoveRangeInRect.getMidX(), 
		m_MoveRangeInRect.getMidY(),
		m_MoveRangeInRect.size.height,
		m_MoveRangeInRect.size.width, filter, b2_staticBody);

	return cloneOne;
}

CAIMove::~CAIMove()
{
	if (m_MoveRange != nullptr)
	{
		m_MoveRange->GetWorld()->DestroyBody(m_MoveRange);
	}
}

bool CAIMove::isOutOfMoveRange()
{
	if (m_MoveRangeInRect.equals(MY_ZERO_RECT))
	{
		return false;
	}
	switch (m_Mode)
	{
	case CAIMOVE_MODE_X:					   
		if (m_Object->getPositionX() < m_MoveRangeInRect.getMinX() || m_Object->getPositionX() > m_MoveRangeInRect.getMaxX())
		{
			return true;
		}
		break;
	case CAIMOVE_MODE_Y:
		if (m_Object->getPositionY() < m_MoveRangeInRect.getMinY() || m_Object->getPositionY() > m_MoveRangeInRect.getMaxY())
		{
			return true;
		}
		break;
	case CAIMOVE_MODE_BOTH_AXISTS:
	{
		if (m_MoveRange != nullptr)
		{
			return !b2TestOverlap(
				m_Body->GetFixtureList()->GetShape(), b2Shape::e_polygon,
				m_MoveRange->GetFixtureList()->GetShape(), b2Shape::e_polygon,
				m_Body->GetTransform(),
				m_MoveRange->GetTransform());
		}
	}
		break;
	default:
		break;
	}


	return false;
}
