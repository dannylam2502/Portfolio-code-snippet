#include "GameOverScene.h"
#include "GamePlayScene.h"
#include "Config.h"
#include "Singleton\SFactory.h"
#include "Component\CStats.h"

GameOverScene*  GameOverScene::m_Instance		= nullptr;
int				GameOverScene::m_IndexSelected	= 1;
Sprite*			GameOverScene::m_controller		= nullptr;

void GameOverScene::initialize()
{
	m_IndexSelected = 1;
	m_Instance = GameOverScene::create();
	m_Instance->retain();
}

void GameOverScene::release()
{
	m_Instance->Scene::release();
}

GameOverScene* GameOverScene::get()
{
	return m_Instance;
}

bool GameOverScene::init()
{
  	if (!Scene::init())
		return false;


	SCREEN_SIZE = Director::getInstance()->getWinSize();

	auto background = Sprite::create("Button//game_over__background.png");
	background->setScaleX(SCREEN_SIZE.width / background->getBoundingBox().size.width);
	background->setPosition(480, 400);
	this->addChild(background, 1);

	// playAgain
	auto img_backMenu = MenuItemImage::create("Button//btt_PlayAgain.png", "Button//btt_PlayAgain.png", CC_CALLBACK_1(GameOverScene::PlayAgain, this));
	img_backMenu->setPosition(-250, 120);

	// close windows
	auto img_exitGame = MenuItemImage::create("Button//btt_ExitGame.png", "Button//btt_ExitGame.png", CC_CALLBACK_1(GameOverScene::CloseWindows, this));
	img_exitGame->setPosition(-250, 20);

	m_controller = Sprite::create("Button//btt_Controller.png");
	m_controller->setPosition(50, 450);
	this->addChild(m_controller, 2);

	// back menu

	Vector<MenuItem*> listMenu;
	listMenu.pushBack(img_backMenu);
	listMenu.pushBack(img_exitGame);

	auto menuGameOver = Menu::createWithArray(listMenu);
	this->addChild(menuGameOver, 1);

	// event keyboard
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameOverScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();

	return true;
}


void GameOverScene::PlayAgain(Ref* pSender)
{
	if (GamePlayScene::get() == nullptr)
		GamePlayScene::initialize();
	auto scene = GamePlayScene::get();
	Director::getInstance()->popScene();
	/*EObject* l_Player = (EObject*)GamePlayScene::get()->m_gameworldLayer->getChildByName(NAME::PLAYER);
	l_Player->pushCommand(COMMAND::COMPONENT::STATS::RESET);*/
}



void GameOverScene::CloseWindows(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}	


void GameOverScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		m_IndexSelected++;
		if (m_IndexSelected > 2)
			m_IndexSelected = 1;
	}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		m_IndexSelected--;
		if (m_IndexSelected < 1)
			m_IndexSelected = 2;
	}
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
	case EventKeyboard::KeyCode::KEY_KP_ENTER:
	{
		if (m_IndexSelected == 1)
		{
			GameOverScene::PlayAgain(nullptr);
		}
		if (m_IndexSelected == 2)
		{
			GameOverScene::CloseWindows(nullptr);
		}
	}
		break;
	}

	if (m_IndexSelected == 1)
		m_controller->setPosition(50, 450);
	if (m_IndexSelected == 2)
		m_controller->setPosition(50, 350);
}
