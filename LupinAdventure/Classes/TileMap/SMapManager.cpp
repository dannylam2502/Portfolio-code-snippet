#include "TileMap/SMapManager.h"
#include "tinyxml2/tinyxml2.h"
#include "Utils.h"
#include "Singleton\SCamera.h"
#include "Scene\GamePlayScene.h"

SMapManager* SMapManager::m_Instance = nullptr;

SMapManager::SMapManager()
{
}

SMapManager::~SMapManager()
{
	m_MapList.clear();
}

void SMapManager::initialize(b2World* physicsWorld, Node* parent)
{
	CC_SAFE_DELETE(m_Instance);
	m_Instance = new SMapManager();
	
	m_Instance->m_CrIndex = 0;
	m_Instance->m_MapList.clear();
	m_Instance->m_CurrentMap = nullptr;
	m_Instance->readMapList();
	m_Instance->m_Parent = parent;

	TileMap::initialize(physicsWorld);
}

void SMapManager::setCurrentMapAt(int index)
{
	if (index < 0 || index >= m_MapList.size())
		return;
	
	m_CrIndex = index;

	if (m_CurrentMap != nullptr)
		m_CurrentMap->removeFromParentAndCleanup(true);

	m_CurrentMap = TileMap::create();
	m_CurrentMap->readMap(m_MapList.at(index)->tmxPath->c_str());
	m_Parent->addChild(m_CurrentMap);
}

void SMapManager::readMapList()
{
	FILE* l_Data = fopen(XML_MAP_LIST, "rb");
	tinyxml2::XMLDocument l_Document;

	if (l_Data != nullptr)
	{
		if (l_Document.LoadFile(XML_MAP_LIST) != tinyxml2::XMLError::XML_NO_ERROR)
			return;
	}
	else
		return;

	tinyxml2::XMLElement* l_Elem = l_Document.FirstChildElement(XML_MAP_KEY);

	while (l_Elem != nullptr)
	{
		m_MapList.push_back(
			new MapInfo(
			l_Elem->Attribute(XML_MAP_ELEM_NAME),
			l_Elem->Attribute(XML_MAP_ELEM_TMX),
			l_Elem->Attribute(XML_MAP_ELEM_BG)));

		l_Elem = l_Elem->NextSiblingElement(XML_MAP_KEY);
	}

	fclose(l_Data);
}

SMapManager* SMapManager::getInstance()
{
	if (m_Instance == nullptr)
	{
		MessageBox("Map manager's Instance is null, please call Init at once before use Get", "Null instance");
		
	}
	return m_Instance;
}

TileMap*	SMapManager::getCrMap()
{
	return m_CurrentMap;
}

void SMapManager::release()
{
	CC_SAFE_DELETE(m_Instance);
}

void SMapManager::switchMap(int step)
{
	// Dung 30/6
	if (step != 0)
	{
		m_LastCharSpawnPos = m_CurrentMap->getCharSpawnPoint();
	}
	//

	setCurrentMapAt(m_CrIndex + step);
	SCamera::getInstance()->setCameraLimitedSize(SMapManager::getInstance()->getCrMap()->getMapSize());
	// Dung 30/6
	if (step == 0)
	{
		auto l_Player = (EObject*)GamePlayScene::get()->m_gameworldLayer->getChildByName(NAME::PLAYER);
		l_Player->setBodyPosition(m_LastCharSpawnPos.x, m_LastCharSpawnPos.y);
	}
	//
}

int	 SMapManager::getCrIndex()
{
	return m_CrIndex;
}