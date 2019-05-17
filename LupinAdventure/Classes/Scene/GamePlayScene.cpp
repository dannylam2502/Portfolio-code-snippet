#include "Scene/GamePlayScene.h"
#include "TileMap/SMapManager.h"
#include "GLES-Render.h"

#include "Component/CStats.h"
#include "Component/CAttack.h"
#include "Component/CAnimation.h"

#include "Singleton/SCamera.h"
#include "Singleton/SEffectLayer.h"
#include "Singleton/SKeyboard.h"
#include "Singleton/SFactory.h"
#include "Singleton/SContactListener.h"
#include "Singleton/SFactoryEffect.h"

#include "Entity\EObject.h"
#include "Familiar\Familiar.h"
#include "Component\CFamiliar.h"
#include <vector>
#include "Singleton\CCShake.h"

#pragma region Scene
GamePlayScene* GamePlayScene::m_Instance = nullptr;

bool GamePlayScene::init()
{
	if (!Scene::init())
		return false;

	SCREEN_SIZE = Director::getInstance()->getWinSizeInPixels();

	m_PhysicsWorld = new b2World(PHYSICS_GRAVITY);
	m_PhysicsWorld->SetAllowSleeping(true);
	m_PhysicsWorld->SetContinuousPhysics(true);
	m_PhysicsWorld->SetContactListener(SContactListener::get());

	// Temp call for init physicsWorld, it's need for GameWorldLayer constructor
	SFactory::initialize(m_PhysicsWorld, nullptr);

	m_gameworldLayer = GameWorldLayer::create(m_PhysicsWorld);
	m_UILayer = UILayer::create();

	m_UILayer->initPlayer(m_gameworldLayer->l_Player);

	addChild(m_gameworldLayer, Z_ORDER::LAYERS::GameWorld);
	addChild(SEffectLayer::get(), Z_ORDER::LAYERS::Effect);
	addChild(m_UILayer, Z_ORDER::LAYERS::UI);

	// Re-call for confirm the "game world"
	SFactory::initialize(m_PhysicsWorld, m_gameworldLayer);

	this->addChild(BackgroundLayer::create(), Z_ORDER::LAYERS::BackGround);
	//m_Instance->addChild(UILayer::create(), zUI);

	/*b2Draw *m_debugDraw = new  GLESDebugDraw(BOX2D_RATIO);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);
	m_PhysicsWorld->SetDebugDraw(m_debugDraw);*/

	scheduleUpdate();
	
	return true;
}

void GamePlayScene::addChild(Node * child, int localZOrder)
{
	Node::addChild(child, localZOrder);

	m_Layers.insert(std::pair<int, Layer*>(localZOrder, (Layer*)child));
}

Layer* GamePlayScene::getLayer(int zOrder)
{
	return m_Layers.at(zOrder);
}

void GamePlayScene::initialize()
{
	m_Instance = GamePlayScene::create();
	m_Instance->retain();
}

void GamePlayScene::release()
{
	m_Instance->Scene::release();
}

GamePlayScene* GamePlayScene::get()
{
	return m_Instance;
}

b2World* GamePlayScene::getPhysicsWorld()
{
	return get()->m_PhysicsWorld;
}

void GamePlayScene::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);

	/*kmGLPushMatrix();
	m_PhysicsWorld->DrawDebugData();
	kmGLPopMatrix();*/
}

GamePlayScene::~GamePlayScene()
{
	CAnimation::releaseCache();
}

#pragma endregion

#pragma region GameWorld layer
bool GameWorldLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	CAttack::setWorld(this);
	SFactoryEffect::init();
	Familiar::initialize(this);

	// Create Player
	l_Player = SFactory::create(ID::PLAYER);
	this->addChild(l_Player, Z_ORDER::GAMEWORLD::Player);
	l_Player->setBodyPosition(CHAR_SPAW_POS_IN_PIXEL.x, CHAR_SPAW_POS_IN_PIXEL.y);
	l_Player->pushCommand(COMMAND::COMPONENT::STATS::RESET);
	//l_Player->setBodyPosition(400, 400);

	// Map
	SMapManager::initialize(m_PhysicsWorld, this);
	SMapManager::getInstance()->setCurrentMapAt(0);

	// Camera
	this->addChild(SCamera::getInstance());
	SCamera::getInstance()->setCameraTarget(l_Player);
	SCamera::getInstance()->setCameraLimitedSize(SMapManager::getInstance()->getCrMap()->getMapSize());

	// IMPORTANT: The scheduleUpdate must be called right before this->addChild(SKeyboard::get()),
	// or the SKeyboard::getEvents always return a vector of nothing because of SKeyboard::update
	scheduleUpdate();
	this->addChild(SKeyboard::get());

	return true;
}

void GameWorldLayer::update(float delta)
{
	m_PhysicsWorld->Step(delta, 10, 10);
	/*m_Time -= delta;
	CCLOG("%f", m_Time);
	*/
	SFactory::updateDestroyer();

	for (b2Body* body = m_PhysicsWorld->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetType() == b2_staticBody)
			continue;

		EObject* object = (EObject*)body->GetUserData();
		if (object != nullptr)
		{
			Vec2 newPos = Vec2(body->GetPosition().x * BOX2D_RATIO, body->GetPosition().y * BOX2D_RATIO);
			object->Node::setPosition(newPos);
		}
	}

	if (SKeyboard::get()->getKeyState(EventKeyboard::KeyCode::KEY_N) == KS_PRESS)
	{
		auto l_Player = (EObject*)getChildByName(ID::PLAYER);
		l_Player->removeAllLinkedObject();
		// Important: SpawnPoint is the Spawnpoint in next map
		// Map0 will have Spawnpoint of Map1
		l_Player->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));

		SMapManager::getInstance()->switchMap(1);
	}
	if (SKeyboard::get()->getKeyState(EventKeyboard::KeyCode::KEY_M) == KS_PRESS)
	{
		auto l_Player = (EObject*)getChildByName(ID::PLAYER);
		l_Player->removeAllLinkedObject();
		// Important: SpawnPoint is the Spawnpoint in next map
		// Map0 will have Spawnpoint of Map1
		l_Player->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));

		SMapManager::getInstance()->switchMap(-1);
	}

	/*if (SKeyboard::get()->getKeyState(EventKeyboard::KeyCode::KEY_UP_ARROW) == KS_PRESS)
	{*/
	auto l_Portal = SMapManager::getInstance()->getCrMap()->getChildByName(ID::REACTOR_PORTAL_1);
	auto l_Player = (EObject*)getChildByName(ID::PLAYER);

	if (l_Portal == nullptr || l_Player == nullptr)
		return;

	if (l_Portal->getBoundingBox().containsPoint(l_Player->getPosition()))
	{
		l_Player->removeAllLinkedObject();
		// Important: SpawnPoint is the Spawnpoint in next map
		// Map0 will have Spawnpoint of Map1
		l_Player->pushCommand((COMMAND::COMPONENT::STATE::CHANGE_TO::STAND));

		SMapManager::getInstance()->switchMap(1);
	}
	//
}

void GameWorldLayer::onExit()
{
	Layer::onExit();
	//SKeyboard::release();
	SEffectLayer::release();
}

GameWorldLayer* GameWorldLayer::create(b2World* physicsWorld)
{
	GameWorldLayer* result = new GameWorldLayer();
	result->autorelease();

	result->m_PhysicsWorld = physicsWorld;
	result->init();

	return result;
}

#pragma endregion

#pragma region Background Layer
bool BackgroundLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_color = LayerColor::create(Color4B(94, 150, 234, 255));
	this->addChild(m_color, Z_ORDER::LAYERS::BackGround);
	scheduleUpdate();
	return true;
}

void BackgroundLayer::update(float delta)
{
	/*if (SMapManager::getInstance()->getCrIndex() >= 5)
	{
		m_color->setColor(Color3B(48, 65, 50));
	}*/
	this->setPosition(SCamera::getInstance()->getCameraPosition());
}
#pragma endregion

#pragma region UI Layer

#define CHAR_HP_WIDTH		250
#define CHAR_HP_HEIGHT		20
#define CHAR_MP_WIDTH		200
#define CHAR_MP_HEIGHT		20
bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	winsize = Director::getInstance()->getWinSize();
	rectWidth = 0;

	this->m_HPbackground = Sprite::create("GUI//HPBackground.png");
	this->m_HPbackground->setPosition(55.0f, winsize.height - 30);
	this->m_HPbackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_HPbackground);

	this->m_MPbackground = Sprite::create("GUI//MPBackground.png");
	this->m_MPbackground->setPosition(55.0f, winsize.height - 55);
	this->m_MPbackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_MPbackground);

	this->m_hp = Sprite::create("GUI//HP.png");
	this->m_hp->setPosition(55.0f, winsize.height - 30);
	this->m_hp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_hp);

	this->m_mp = Sprite::create("GUI//MP.png");
	this->m_mp->setPosition(55.0f, winsize.height - 55);
	this->m_mp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_mp);

	this->m_HPLabel = LabelTTF::create("alignment left", "Arial", 24);
	this->m_HPLabel->setPosition(15.0f, winsize.height - 28);
	this->m_HPLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_HPLabel);

	this->m_MPLabel = LabelTTF::create("alignment left", "Arial", 24);
	this->m_MPLabel->setPosition(15.0f, winsize.height - 55);
	this->m_MPLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(m_MPLabel);

	scheduleUpdate();

	return true;
}

void UILayer::update(float delta)
{
	this->setPosition(SCamera::getInstance()->getCameraPosition());

	rectWidth = (m_PlayerStats->m_HP * CHAR_HP_WIDTH / m_PlayerStats->m_MaxHP);
	if (rectWidth <= 0)
		rectWidth = 0;
	m_hp->setTextureRect(Rect(0, 0, rectWidth, CHAR_HP_HEIGHT));

	m_HPLabel->setString(std::to_string(m_PlayerStats->m_HP));

	auto cFamiliar = (CFamiliar*)m_Player->getChildByTag(TAG::COMPONENT::FAMILIAR);
	if (cFamiliar == nullptr)
	{
		return;
	}
	rectWidth = cFamiliar->getMPPercent() * CHAR_MP_WIDTH;
	if (rectWidth <= 0)
		rectWidth = 0;
	m_mp->setTextureRect(Rect(0, 0, rectWidth, CHAR_MP_HEIGHT));

	m_MPLabel->setString(std::to_string(cFamiliar->getMP()));
}

void UILayer::initPlayer(EObject* player)
{
	m_Player = player;
	vector<CBase*> component;
	component.operator=(m_Player->getComponent());
	for (size_t i = 0; i < component.size(); i++)
	{
		if (component.at(i)->getTag() == TAG::COMPONENT::STATS)
		{
			m_PlayerStats = dynamic_cast<CStats*>(component.at(i));
			break;
		}
	}
}
#pragma endregion