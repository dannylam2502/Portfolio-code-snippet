// Author: Dung
// Created: 11/5/2015
// Attack Component for Character and Mob.

#ifndef CAttack_H_
#define CAttack_H_

#include "cocos2d.h"
#include "CBase.h"
#include <vector>
#include "Config.h"
#include "Utils.h"

USING_NS_CC;
using namespace std;

class EObject;

class CAttack : public CBase
{
public:
	static void addComponentTo(EObject* object, const char* idBullet, b2Vec2 shootSpot);

	static CAttack* createComponent(EObject* object, const char* idBullet, b2Vec2 shootSpot);

	// Frederick - 16/5/2015
	static void setWorld(Layer* gameWorldLayer);

	virtual void commandProcess(int command, float deltaTime);

	// parameterTypeFlag:
	virtual void setProperty(int parameterTypeFlag, UParam parameter);

	virtual void update(float delta);

	virtual bool init();

	virtual CBase* clone(EObject* object);

private:
	CREATE_FUNC(CAttack);

	void	setBulletInfoFromFile(const char* filePath);

	void	makeBulletLive(EObject* bullet);


private:
	vector<BulletInfo>			m_vectorBulletInfo;
	float						m_TimeBoost;
	bool						m_IsBoost;
	
	const char*					m_idBullet;
	b2Filter					m_BulletFilter;
	b2Vec2						m_ShootSpot;
	b2Body*						m_Body;

	// Frederick - 16/5/2015
	static Layer*				m_PhysicWorld;
};

#endif // CDamage_H_