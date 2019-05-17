#include "TileMap/TileMap.h"
#include "Config.h"
#include "Singleton\SFactory.h"
#include "Utils.h"
#include "TileMap\SMapManager.h"

using namespace std;

b2World		* TileMap::m_PhysicsWorld = nullptr;

TileMap::~TileMap()
{
	b2Body* holder;
	for (b2Body* body = m_PhysicsWorld->GetBodyList(); body != nullptr; body = holder)
	{
		holder = body->GetNext();
		EObject* object = (EObject*)body->GetUserData();
		if (object != nullptr)
			if (isStringEqual(object->getName().c_str(), ID::PLAYER))
			{
				body->SetLinearVelocity(B2VEC2_ZERO);
				object->setBodyPosition(this->getCharSpawnPoint().x,this->getCharSpawnPoint().y);
			}
			else
			{
				m_PhysicsWorld->DestroyBody(body);
				object->removeFromParentAndCleanup(true);
				body = nullptr;
			}
	}
}

void TileMap::update(float deltatime)
{
}

void TileMap::readMap(const char* tmxPath)
{
	this->removeAllChildrenWithCleanup(true);

	m_TileMap = CCTMXTiledMap::create(tmxPath);

	// Get Map Properties
	// CharSpawnPoint is the point of next map
	// Map0 will have spawnpoint of Map1
	ValueMap l_mapProp;
	l_mapProp = m_TileMap->getProperties();
	m_CharSpawnPoint.x = stof(l_mapProp.at("SpawnPointX").asString());
	m_CharSpawnPoint.y = stof(l_mapProp.at("SpawnPointY").asString());
	//m_Time = stof(l_mapProp.at("Time").asString());
	// Load Tile, Platform, Mob and Decorate
	ValueMap l_valueTemp;
	ValueVector l_vectorValue, l_vectorPoints;
	CCTMXObjectGroup* l_Group;
	float x, y, w, h, Vx, Vy;
	Vec2 range;
	string type;
	string name;
	// Load tile
	/*m_Background = m_TileMap->layerNamed("Tiled");
	m_Background = m_TileMap->layerNamed("Reactor");
	m_Background = m_TileMap->layerNamed("Object");*/

	// Load Static platform
	l_Group = m_TileMap->objectGroupNamed("StaticPlatform");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		//auto physicsBody = PhysicsBody::create();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			//auto sprite = Platform::create(Vec2(x + w / 2, y + h / 2));
			w = l_valueTemp.at("width").asFloat();
			h = l_valueTemp.at("height").asFloat();
			x = l_valueTemp.at("x").asFloat() + w / 2;
			y = l_valueTemp.at("y").asFloat() + h / 2;
			type = l_valueTemp.at("type").asString();

			if (isStringEqual(type.c_str(), ID::PLATFORM_POLYGON))
			{
				l_vectorPoints = l_valueTemp.at("points").asValueVector();
			}
			else
			{
				l_vectorPoints.clear();
			}

			EObject* object = SFactory::create(type.c_str());
			object->changeBodyShape(x, y, h, w);
			object->setBodyPosition(x, y);
			object->setContentSize(Size(w, h));

			this->addChild(object, Z_ORDER::MAPOBJECT::Tile);
		}
	}
	// Load Dynamic Platform object
	l_Group = m_TileMap->objectGroupNamed("DynamicPlatform");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			// Create Dynamic platform object here
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			w = l_valueTemp.at("width").asFloat();
			h = l_valueTemp.at("height").asFloat();
			x = l_valueTemp.at("x").asFloat();
			y = l_valueTemp.at("y").asFloat();
			/*range.x = l_valueTemp.at("rangeX").asFloat();
			range.y = l_valueTemp.at("rangeY").asFloat();*/
			type = l_valueTemp.at("type").asString().c_str();
			Vx = l_valueTemp.at("Vx").asFloat();
			Vy = l_valueTemp.at("Vy").asFloat();

			/*if (isStringEqual(type.c_str(), ID::PLATFORM_DYNAMIC_TYPE1_X))
			{*/
				EObject* object = SFactory::create(type.c_str());
				object->setBodyPosition(x, y);
				object->setContentSize(Size(w, h));

				object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_VEC2_CO_MOVE_VELO, UParam(b2Vec2(Vx, Vy)));
				object->setProperty(TAG::COMPONENT::AI_MOVE, UPARAM_TYPE_RECT, UParam(Rect(x, y, w, h)));

				this->addChild(object, Z_ORDER::MAPOBJECT::Tile);
			//}
		}
	}

	// Load Mob object
	l_Group = m_TileMap->objectGroupNamed("Mob");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			// Create Mob Object here
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			w = l_valueTemp.at("width").asFloat();
			h = l_valueTemp.at("height").asFloat();
			x = l_valueTemp.at("x").asFloat() + w/2;
			y = l_valueTemp.at("y").asFloat() + h/2;
			type = l_valueTemp.at("type").asString().c_str();
			/*Vx = std::atof(l_valueTemp.at("Vx").asString().c_str());
			Vy = std::atof(l_valueTemp.at("Vy").asString().c_str());*/
			/*range.x = std::atof(l_valueTemp.at("rangeX").asString().c_str());
			range.y = std::atof(l_valueTemp.at("rangeY").asString().c_str());*/

			EObject* object = SFactory::create(type.c_str());
			object->setProperty(TAG::COMPONENT::AI_MOVE, UPARAM_TYPE_RECT, UParam(Rect(x, y, w, h)));
			object->setBodyPosition(x, y);

			//object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_VEC2_CO_MOVE_VELO, UParam(b2Vec2(Vx, Vy)));

			this->addChild(object, Z_ORDER::MAPOBJECT::Mob);
		}
	}

	// Load Decorating Object
	l_Group = m_TileMap->objectGroupNamed("Reactor");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			// Create Object here
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			w = l_valueTemp.at("width").asFloat();
			h = l_valueTemp.at("height").asFloat();
			x = l_valueTemp.at("x").asFloat();
			y = l_valueTemp.at("y").asFloat();
			type = l_valueTemp.at("type").asString().c_str();
			name = l_valueTemp.at("name").asString();
			
			EObject* object = SFactory::create(type.c_str());
			//object->setProperty(TAG::COMPONENT::AI_MOVE, UPARAM_TYPE_RECT, UParam(Rect(x, y, w, h)));
			object->setBodyPosition(x, y);

			this->addChild(object, Z_ORDER::MAPOBJECT::Mob);
		}
	}

	// Load Trap
	l_Group = m_TileMap->objectGroupNamed("Trap");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			// Create Object here
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			w = l_valueTemp.at("width").asFloat();
			h = l_valueTemp.at("height").asFloat();
			x = l_valueTemp.at("x").asFloat() + w/2;
			y = l_valueTemp.at("y").asFloat() + h/2;
			Vx = l_valueTemp.at("Vx").asFloat();
			Vy = l_valueTemp.at("Vy").asFloat();
			type = l_valueTemp.at("type").asString().c_str();
			name = l_valueTemp.at("name").asString();

			EObject* object = SFactory::create(type.c_str());
			object->setProperty(TAG::COMPONENT::MOVE, UPARAM_TYPE_VEC2_CO_MOVE_VELO, UParam(b2Vec2(Vx, Vy)));
			object->setProperty(TAG::COMPONENT::AI_MOVE, UPARAM_TYPE_RECT, UParam(Rect(x, y, w, h)));
			object->setBodyPosition(x, y);

			this->addChild(object, Z_ORDER::MAPOBJECT::Tile);
		}
	}

	// Load Item
	l_Group = m_TileMap->objectGroupNamed("Item");
	if (l_Group != NULL)
	{
		l_vectorValue = l_Group->getObjects();
		for (int i = 0; i < l_vectorValue.size(); i++)
		{
			// Create Object here
			l_valueTemp = l_vectorValue.at(i).asValueMap();
			x = l_valueTemp.at("x").asFloat();
			y = l_valueTemp.at("y").asFloat();
			type = l_valueTemp.at("type").asString().c_str();
			name = l_valueTemp.at("name").asString();

			EObject* object = SFactory::create(type.c_str());
			object->setBodyPosition(x, y);

			this->addChild(object, Z_ORDER::MAPOBJECT::Mob);
		}
	}

	// End load
	this->addChild(m_TileMap, Z_ORDER::GAMEWORLD::Map);
}

Size	TileMap::getMapSize()
{
	Size result;
	result.width = m_TileMap->getMapSize().width * m_TileMap->getTileSize().width;
	result.height = m_TileMap->getMapSize().height * m_TileMap->getTileSize().height;
	return result;
}

void TileMap::initialize(b2World* physicsWorld)
{
	m_PhysicsWorld = physicsWorld;
}

Vec2 TileMap::getCharSpawnPoint()
{
	return m_CharSpawnPoint;
}