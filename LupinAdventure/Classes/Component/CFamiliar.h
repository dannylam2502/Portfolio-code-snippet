#ifndef CFamiliar_h__
#define CFamiliar_h__

#include "Component/CBase.h"
#include "Entity/EObject.h"
#include "Component/CStats.h"
#include "Familiar/Familiar.h"
#include <vector>

using namespace std;

class CFamiliar : public CBase
{
public:
	static CFamiliar*	addComponentTo(EObject* owner);

	virtual void commandProcess(int command, float deltaTime);

	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual CBase* clone(EObject* object);

	string getCrFamiliarName();

	float	getMPPercent();

	int		getMP();
	Familiar*	getCrFamiliar();

	~CFamiliar();

private:
	CREATE_FUNC(CFamiliar);

	void swapToFamiliar(int index);

	virtual bool init();

private:
	vector<Familiar*> m_FamiliarList;

	int				m_CrIndex;
	Familiar*		m_CrFamiliar;
	CStats*			m_Stats;
	b2Body*			m_Crbody;
};

#endif // CFamiliar_h__
