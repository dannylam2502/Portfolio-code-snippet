#ifndef __EFFECTS_FACTORY_H__ 
#define __EFFECTS_FACTORY_H__

#include <vector>
#include <cocos2d.h>
#include "Config.h"
#include "Entity/EObject.h"

class SFactoryEffect
{
public:
	// Create a sequence of Effect
	static EObject*	create(const char* resourceLink, vector<int> listOfAnimationIndex/*, EObject* storedEffectObject*/);

	// Create a single effect
	static EObject* create(const char* resourceLink, int animationIndex/*, EObject* storedEffectObject*/, bool noLoop);

	static void init();
	static void release();

private:
	SFactoryEffect();
	~SFactoryEffect();
	static SFactoryEffect* get();

	EObject* createRawEffect(const char* resourceLink, bool noLoop);

private:
	static SFactoryEffect* m_Instance;
	
	Action*	m_AutoreleaseAction;
};


#endif
