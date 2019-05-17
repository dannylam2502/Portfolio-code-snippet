#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include "cocos2d.h"
#include "Entity\EObject.h"

USING_NS_CC;

class TileMap : public Node
{
public:
	CREATE_FUNC(TileMap);
	~TileMap();

	void			update(float deltaTime);
	void			readMap(const char* tmxPath);
	Size			getMapSize();
	static void		initialize(b2World* physicsWorld);
	//Update 25/06
	Vec2			getCharSpawnPoint();

private:
	CCTMXTiledMap		*m_TileMap;
	CCTMXLayer			*m_Background;
	static b2World		*m_PhysicsWorld;

	// Manage spawn point of character
	Vec2				m_CharSpawnPoint;
};
#endif // _TILEMAP_H_