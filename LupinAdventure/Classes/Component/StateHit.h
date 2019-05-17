#ifndef StateHit_h__
#define StateHit_h__

#include "Component/State.h"

class StateHit : public State
{
public:
	StateHit();
	StateHit(EObject* object, float immuneDuration, float lostControlDuration);
	~StateHit();
	virtual void enter();

	virtual void exit();

	virtual int commandProcess(int command);

	void	stopImmune();

	virtual State* clone(EObject* object);
	void	doStateEffect();

private:
	void initializeEffect(EObject* object);

private:
	RepeatForever*		m_FlashEffect;
	RepeatForever*		m_RunningFlashEffect;
	Sequence*			m_ImmortalEffect;
	Sequence*			m_LostcontrolAction;
	b2Body*				m_Body;
	b2Filter			m_Filter;

	float		m_ImmuneDuration;
	float		m_LostControlDuration;
};

#endif // StateHit_h__
