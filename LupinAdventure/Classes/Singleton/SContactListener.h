#ifndef SContactListener_h__
#define SContactListener_h__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class EObject;

class SContactListener : public b2ContactListener
{
public:
	virtual void BeginContact(b2Contact* contact);

	virtual void EndContact(b2Contact* contact);

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	static void init();
	static void release();
	static SContactListener* get();

private:
	SContactListener();
	~SContactListener();

	int computeCollideFlag(int objectP1Category, int objectP2Category);

	void collideProcessObjectTouchTile(b2Contact* contact, EObject* object, EObject* theTile);

private:
	static SContactListener*	m_Instance;
};

#endif // SContactListener_h__
