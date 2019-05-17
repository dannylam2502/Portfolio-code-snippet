#ifndef FamiliarDark_h__
#define FamiliarDark_h__

#include "cocos2d.h"
#include "Entity\EObject.h"
#include "Familiar\Familiar.h"

class FamiliarDark : public Familiar
{
public:
	static FamiliarDark* create(EObject* owner);

	virtual bool init();

	virtual void update(float delta);

	virtual void abillity(EObject* owner);

private:
	CREATE_FUNC(FamiliarDark);
};

#endif // FamiliarDark_h__
