#include "CAnimation.h"
#include "tinyxml2/tinyxml2.h"

map<const char*, CAnimation*> CAnimation::m_Cache;

void CAnimation::commandProcess(int command, float deltaTime)
{
	switch (command)
	{
	case COMMAND::COMPONENT::ANIMATION::START_SHADOW:
		resumeSchedulerAndActions();
		return;
	case COMMAND::COMPONENT::ANIMATION::STOP_SHADOW:
		pauseSchedulerAndActions();
		return;
	case COMMAND::COMPONENT::ANIMATION::VISIBLE:
		m_PreventingAnimation->setVisible(true);
		return;
	case COMMAND::COMPONENT::ANIMATION::INVISIBLE:
		m_PreventingAnimation->setVisible(false);
		return;
	case COMMAND::COMPONENT::ANIMATION::STOP_LOOP:
		m_IsLoop = false;
		m_AniIndexQueue.clear();
		m_PreventingAnimation->stopAllActions();
		m_CurrentAnimationIndex = 0;
		return;
	case COMMAND::COMPONENT::ANIMATION::START_LOOP:
		m_IsLoop = true;
		return;
	case COMMAND::COMPONENT::ANIMATION::FACE_LEFT:
		m_Object->setFlip(FLIP_FACE_LEFT);
		return;
	case COMMAND::COMPONENT::ANIMATION::FACE_RIGHT:
		m_Object->setFlip(FLIP_FACE_RIGHT);
		return;
	case COMMAND::COMPONENT::STATS::RESET:
		m_Object->commandProcess(COMMAND::COMPONENT::ANIMATION::VISIBLE);
		break;
	default:
		break;
	}
	// If the passed animation index in [command] are available:
	if (command >= (COMMAND::COMPONENT::ANIMATION::CHANGE_TO::FIRST) &&
		command < (COMMAND::COMPONENT::ANIMATION::CHANGE_TO::LAST_PLUS_1))
	{
		// Run object by the action suit with the passed index in [command]
		if (m_CurrentAnimationIndex != command)
		{
			if (m_IsLoop ||
				m_PreventingAnimation->getNumberOfRunningActions() == 0)
			{
				runAnimation(command);
			}
			else
			{
				m_AniIndexQueue.push_back(command);
			}
		}
	}
}

void CAnimation::setProperty(int parameterTypeFlag, UParam parameter)
{
	switch (parameterTypeFlag)
	{
		case UPARAM_TYPE_VEC2:
		{
			b2Vec2 vec = *parameter.uVec2Ptr;
			m_PreventingAnimation->setPositionX(vec.x);
			m_PreventingAnimation->setPositionY(vec.y);
			break;
		}
	}
}

CAnimation::~CAnimation()
{
	for each (pair<int, ActionInfomation*> animation in m_AnimationBoard)
	{
		animation.second->animate->release();
	}

	m_AnimationBoard.clear();
}

CAnimation* CAnimation::addComponentTo(EObject* object, const char* infoFileLink)
{
	try
	{
		return (CAnimation*)CAnimation::m_Cache.at(infoFileLink)->clone(object);
	}
	catch (exception e)
	{
		CAnimation* component = CAnimation::create();

		component->m_Object = nullptr;
		component->commandProcess(COMMAND::COMPONENT::ANIMATION::STOP_SHADOW, 0.0f);

		// Read animation file for setting up
		component->m_AnimationBoard = createActionInformation(infoFileLink);

		m_Cache.insert(std::pair<const char*, CAnimation*>(infoFileLink, component));
		component->retain();

		return (CAnimation*)CAnimation::m_Cache.at(infoFileLink)->clone(object);
	}

}

bool CAnimation::init()
{
	if (!CBase::init())
	{
		return false;
	}
	this->setTag(TAG::COMPONENT::ANIMATION);

	m_IsLoop = true;
	m_AnimationBoard.clear();
	m_PreventingAnimation = Sprite::create();
	this->addChild(m_PreventingAnimation);
	m_CurrentAnimationIndex = 0;
	m_CurrentAnimation = nullptr;

	this->setAnchorPoint(Point::ANCHOR_MIDDLE);
	m_PreventingAnimation->setAnchorPoint(Point::ANCHOR_MIDDLE);
	
	scheduleUpdate();

	return true;
}

void CAnimation::update(float delta)
{
	//auto frame = m_PreventingAnimation->getSpriteFrame();
	//auto sprite = Sprite::createWithSpriteFrame(frame);
	//
	//sprite->setAnchorPoint(m_PreventingActionInformation->anchorPoint);
	//sprite->Node::setPosition(m_PreventingAnimation->getPosition() + m_Object->getPosition());
	//sprite->setScale(m_Object->getScaleX(), m_Object->getScaleY());
	//
	//auto fade = FadeOut::create(1.0f);
	//auto removeChild = RemoveSelf::create(true);
	//Sequence* newAction = Sequence::create(fade, removeChild, nullptr);
	//sprite->runAction(newAction);
	//this->m_Object->getParent()->addChild(sprite, zBackEffect);

	//auto frame = m_PreventingAnimation->getSpriteFrame();
	//auto sprite = SpriteFrameCache::getInstance()->getSpriteFrameByName("DrukenLupin Move (1).png");

	/*if (frame->_ID == sprite->_ID)
		return;*/
}

float CAnimation::getPreventingAnimationDuration()
{
	if (m_CurrentAnimation == nullptr)
	{
		checkForNextAnimation();
		if (m_CurrentAnimation == nullptr)
		{
			return 0.0f;
		}
	}

	return (m_CurrentAnimation->getDuration() - m_CurrentAnimation->getElapsed());
}

bool CAnimation::isAnimationDone()
{
	return	m_PreventingAnimation->getNumberOfRunningActions() == 0;
}

void CAnimation::runAnimation(int animationIndex)
{
	if (animationIndex == 0)
		return;

	try
	{
		if (m_IsLoop)
		{
			m_CurrentAnimationIndex = animationIndex;
			m_PreventingActionInformation = m_AnimationBoard.at(animationIndex);
			m_CurrentAnimation = RepeatForever::create(m_PreventingActionInformation->animate);
		}
		else
		{
			m_AniIndexQueue.push_back(animationIndex);
			checkForNextAnimation();
		}

		m_PreventingAnimation->stopAllActions();
		auto newAnimateVeryFirstFrame = m_PreventingActionInformation->animate->getAnimation()->getFrames().front()->getSpriteFrame();
		m_PreventingAnimation->setDisplayFrame(newAnimateVeryFirstFrame);
		m_PreventingAnimation->runAction(m_CurrentAnimation);

		//this->setAnchorPoint(m_PreventingActionInformation->anchorPoint);
		m_PreventingAnimation->setAnchorPoint(m_PreventingActionInformation->anchorPoint);
	}
	catch (exception e)
	{ }
}

void CAnimation::checkForNextAnimation()
{
	if (m_AniIndexQueue.empty())
	{
		m_Object->pushCommand(COMMAND::COMPONENT::ANIMATION::DONE);
		return;
	}

	m_PreventingAnimation->stopAllActions();

	m_CurrentAnimationIndex = m_AniIndexQueue.front();
	m_AniIndexQueue.erase(m_AniIndexQueue.begin());

	m_PreventingActionInformation = m_AnimationBoard.at((int)m_CurrentAnimationIndex);

	auto animate = m_PreventingActionInformation->animate;
	auto waitAction = DelayTime::create(animate->getDuration());
	auto callCheckAction = CallFunc::create(std::bind(&CAnimation::checkForNextAnimation, this));
	auto recheckAction = Sequence::create(waitAction, callCheckAction, nullptr);

	m_CurrentAnimation = Spawn::create(animate, recheckAction, nullptr);

	//m_PreventingAnimation->runAction(m_CurrentAnimation);
	//m_PreventingAnimation->setAnchorPoint(m_PreventingActionInformation->anchorPoint);

}

CBase* CAnimation::clone(EObject* object)
{
	auto cloneOne = create();
	
	// Copy the animations
	cloneOne->m_AniIndexQueue.operator=(m_AniIndexQueue);
	for (pair<int, ActionInfomation*> infoPair : m_AnimationBoard)
	{
		ActionInfomation* newInfo = new ActionInfomation(
			infoPair.second->animateKey,
			infoPair.second->animate->clone(),
			infoPair.second->anchorPoint);

		newInfo->animate->retain();
		cloneOne->m_AnimationBoard.insert(pair<int, ActionInfomation*>(infoPair.first, newInfo));
	}
	cloneOne->m_IsLoop = m_IsLoop;
	// ================================

	cloneOne->m_Object = object;
	object->addChild(cloneOne);
	
	//cloneOne->m_PreventingAnimation->setPositionY(-cloneOne->m_Object->getContentSize().height / 2.0f);

	cloneOne->m_CurrentAnimationIndex = m_CurrentAnimationIndex;
	if (cloneOne->m_CurrentAnimationIndex != 0)
	{
		cloneOne->m_PreventingActionInformation = cloneOne->m_AnimationBoard.at((int)cloneOne->m_CurrentAnimationIndex);
		cloneOne->m_CurrentAnimation = RepeatForever::create(cloneOne->m_PreventingActionInformation->animate);
	}

	return cloneOne;
}

int CAnimation::getCurrentAnimationIndex()
{
	return m_CurrentAnimationIndex;
}

void CAnimation::releaseCache()
{
	for (pair<const char*, CAnimation*> actionInfo : m_Cache)
	{
		CC_SAFE_RELEASE(actionInfo.second);
	}

	m_Cache.clear();
}