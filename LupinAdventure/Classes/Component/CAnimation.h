// Author: Frederick Tran
// Created: 4/5/2015
// Animation Component, used to managing and control the object's animate.

#ifndef CAnimation_h__
#define CAnimation_h__

#include "cocos2d.h"
#include "Component/CBase.h"
#include "Entity/EObject.h"
#include <map>
#include "Utils.h"

USING_NS_CC;
using namespace std;

class CAnimation : public CBase
{
public:
	// Create a whole new Animation component then add it to [object].
	// The Animation in the component are read from the [xml] file defined in [infoFileLink]
	static CAnimation* addComponentTo(EObject* object, const char* infoFileLink);

	// command:
	//	+ COMMAND::COMPONENT::ANIMATION::(A): set animation by it's index
	//	+ A = [int::FIRST ~ int::LAST_PLUS_1]: formed as [int::...]	- Config.h
	virtual void commandProcess(int command, float deltaTime);

	// parameterTypeFlag:
	//	+ UPARAM_TYPE_VEC2: set the translated distance for the animation, base on object's pos
	virtual void setProperty(int parameterTypeFlag, UParam parameter);		

	virtual bool init();

	virtual void update(float delta);

	float getPreventingAnimationDuration();
	bool	isAnimationDone();
	int		getCurrentAnimationIndex();

	virtual CBase* clone(EObject* object);
	static void releaseCache();

private:
	CREATE_FUNC(CAnimation);
	~CAnimation();
	void	runAnimation(int animationIndex);
	void	checkForNextAnimation();


private:
	map<int, ActionInfomation*>				m_AnimationBoard;
	static map<const char*, CAnimation*>	m_Cache;
	int										m_CurrentAnimationIndex;
	ActionInterval*							m_CurrentAnimation;
	Sprite*									m_PreventingAnimation;
	ActionInfomation*						m_PreventingActionInformation;
	bool									m_IsLoop;

	vector<int>								m_AniIndexQueue;
};

#endif // CAnimation_h__