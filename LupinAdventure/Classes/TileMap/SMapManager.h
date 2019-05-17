#ifndef MapManager_h__
#define MapManager_h__

#include "TileMap/TileMap.h"
#include "Box2D/Box2D.h"
#include <vector>
#include "Config.h"
using namespace std;

class SMapManager
{
public:
	TileMap*	getCrMap();
	void		setCurrentMapAt(int index);

	// step = 1 meaning the next one, -1 is the previous one and so on...
	void		switchMap(int step);

	/*Getting instance method
	return a Map Manager Instance
	Need to call MapManager::init() at once before use this method*/
	static SMapManager*	getInstance();
	static void			initialize(b2World* physicsWorld, Node* parent);
	static void			release();

	int		getCrIndex();

private:
	SMapManager();
	~SMapManager();

	void readMapList();

private:
	vector<MapInfo*>	m_MapList;
	int					m_CrIndex;
	TileMap*			m_CurrentMap;
	Node*				m_Parent;
	Vec2				m_LastCharSpawnPos;

	static SMapManager*	m_Instance;
};

#endif // MapManager_h__
