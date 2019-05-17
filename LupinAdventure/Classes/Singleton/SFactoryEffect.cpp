#include "SFactoryEffect.h"
#include "Component/CAnimation.h"

SFactoryEffect* SFactoryEffect::m_Instance = nullptr;

EObject* SFactoryEffect::create(const char* resourceLink, vector<int> listOfAnimationIndex/*, EObject* storedEffectObject*/)
{
	EObject* result = get()->createRawEffect(resourceLink, true);

	FOR(listOfAnimationIndex.size())
		result->pushCommand(listOfAnimationIndex.at(i));

	result->update(0.0f);

	/*result->setPosition(storedEffectObject->getPosition());
	result->setScaleX(storedEffectObject->getScaleX());
	storedEffectObject->getParent()->addChild(result);*/

	return result;
}

EObject* SFactoryEffect::create(const char* resourceLink, int animationIndex/*, EObject* storedEffectObject*/, bool noLoop)
{
	EObject* result = get()->createRawEffect(resourceLink, noLoop);

	result->pushCommand(animationIndex);
	result->update(0.0f);

	//result->setPosition(storedEffectObject->getPosition());
	//result->setScaleX(storedEffectObject->getScaleX());
	//storedEffectObject->getParent()->addChild(result);

	return result;
}

SFactoryEffect::SFactoryEffect()
{
}

SFactoryEffect::~SFactoryEffect()
{
}

SFactoryEffect* SFactoryEffect::get()
{
	return m_Instance;
}

void SFactoryEffect::release()
{
	CC_SAFE_DELETE(m_Instance);
}

void SFactoryEffect::init()
{
	if (m_Instance == nullptr)
		m_Instance = new SFactoryEffect();
}

EObject* SFactoryEffect::createRawEffect(const char* resourceLink, bool noLoop)
{
	EObject* result = EObject::create();
	CAnimation::addComponentTo(result, resourceLink);

	if (noLoop)
	{
		result->pushCommand(COMMAND::COMPONENT::ANIMATION::STOP_LOOP);
		result->setProperty(TAG::COMPONENT::NONE, UPARAM_TYPE_ACTION_PAIR, UParam(COMMAND::COMPONENT::ANIMATION::DONE, RemoveSelf::create(true)));
	}
	
	return result;
}
