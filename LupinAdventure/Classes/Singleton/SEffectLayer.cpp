#include "Singleton/SEffectLayer.h"
#include "iostream"
#include "Config.h"
SEffectLayer*		SEffectLayer::m_Instance = nullptr;
vector<EObject*>	SEffectLayer::m_vectorEffect;



SEffectLayer* SEffectLayer::get()
{
	if (m_Instance == nullptr)
		initialize();
	return m_Instance;
}

vector<EObject*> SEffectLayer::getVectorEffect()
{
	return m_vectorEffect;
}

void SEffectLayer::push(EObject* obs)
{ 
	m_vectorEffect.push_back(obs);
}

void SEffectLayer::initialize()
{
	m_Instance = SEffectLayer::create();
}

void SEffectLayer::release()
{
	/*if (m_Instance != nullptr)
		m_Instance->release();*/
}

void SEffectLayer::update(float delta)
{
	
}

bool SEffectLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	scheduleUpdate();
	return true;
}