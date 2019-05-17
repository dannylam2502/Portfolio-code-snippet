// Author: Frederick Tran
// Created: 20/5/2015
// To do: contain any type of Effect in the game.

#ifndef SEffectLayer_h__
#define SEffectLayer_h__

#include "cocos2d.h"
#include "Entity/EObject.h"

USING_NS_CC;

class SEffectLayer : public Layer
{
public:
	static	SEffectLayer*		get();
	static	vector<EObject*>	getVectorEffect();
	static	void				push(EObject*);
	static	void				initialize();
	static	void				release();
	virtual void				update(float delta);


private:
	CREATE_FUNC(SEffectLayer);

	virtual bool init();

private:
	static	SEffectLayer*		m_Instance;
	static	vector<EObject*>	m_vectorEffect;
};

#endif // __END_EFFECT_LAYER_H_