#ifndef FamiliarFire_h__
#define FamiliarFire_h__

#include "cocos2d.h"
#include "Entity\EObject.h"
#include "Familiar\Familiar.h"

class FamiliarFire : public Familiar
{
public:
	static FamiliarFire* create(EObject* owner);

	virtual bool init();

	virtual void update(float delta);

	virtual void abillity(EObject* owner);

private:
	CREATE_FUNC(FamiliarFire);
};

#endif // FamiliarFire_h__
