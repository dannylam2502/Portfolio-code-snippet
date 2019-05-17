#include "GameMenuScene.h"
#include "GamePlayScene.h"
#include "GameOverScene.h"
GameMenuScene*	GameMenuScene::m_Instance		= nullptr;
int				GameMenuScene::m_IndexSelected	= 1;
Sprite*			GameMenuScene::m_controller		= nullptr;


void GameMenuScene::initialize()
{
	m_Instance = GameMenuScene::create();
	m_Instance->retain();
}

void GameMenuScene::release()
{
	m_Instance->Scene::release();
}

GameMenuScene* GameMenuScene::get()
{
	return m_Instance;
}

bool GameMenuScene::init()
{
	if (!Scene::init())
		return false;


	SCREEN_SIZE = Director::getInstance()->getWinSize();

	auto background = Sprite::create("Button//game_play__background.png");
	background->setScaleX(SCREEN_SIZE.width / background->getBoundingBox().size.width);
	background->setPosition(480, 400);
	this->addChild(background, 1);

	// playAgain
	auto img_backMenu = MenuItemImage::create("Button//btt_PlayAgain.png", "Button//btt_PlayAgain.png", CC_CALLBACK_1(GameMenuScene::Play, this));
	img_backMenu->setPosition(-200, -75);

	// close windows
	auto img_aboutGame = MenuItemImage::create("Button//btt_AboutGame.png", "Button//btt_AboutGame.png", CC_CALLBACK_1(GameMenuScene::About, this));
	img_aboutGame->setPosition(-200, -175);

	// close windows
	auto img_exitGame = MenuItemImage::create("Button//btt_ExitGame.png", "Button//btt_ExitGame.png", CC_CALLBACK_1(GameMenuScene::Exit, this));
	img_exitGame->setPosition(-200, -275);

	m_controller = Sprite::create("Button//btt_Controller.png");
	m_controller->setPosition(100, 250);
	
	this->addChild(m_controller, 2);

	// back menu

	Vector<MenuItem*> listMenu;
	listMenu.pushBack(img_backMenu);
	listMenu.pushBack(img_exitGame);
	listMenu.pushBack(img_aboutGame);

	auto menuGameOver = Menu::createWithArray(listMenu);
	this->addChild(menuGameOver, 3);

	// event keyboard
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameMenuScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();

	return true;
}


void GameMenuScene::Play(Ref* pSender)
{
	if (GamePlayScene::get() == nullptr)
			GamePlayScene::initialize();
	auto scene = GamePlayScene::get();

	// run
	//Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
	Director::getInstance()->replaceScene(scene);
}


void GameMenuScene::About(Ref* pSeder )
{

}

void GameMenuScene::Exit(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameMenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		m_IndexSelected++;
		if (m_IndexSelected > 3)
			m_IndexSelected = 1;
	}
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		m_IndexSelected--;
		if (m_IndexSelected < 1)
			m_IndexSelected = 3;
	}
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
	case EventKeyboard::KeyCode::KEY_KP_ENTER:
	{
		if (m_IndexSelected == 1)
		{
			GameMenuScene::Play(nullptr);
		}
		if (m_IndexSelected == 2)
		{
			GameMenuScene::About(nullptr);
		}
		if (m_IndexSelected == 3)
		{
			GameMenuScene::Exit(nullptr);
		}
	}
		break;
	}

	if (m_IndexSelected == 1)
		m_controller->setPosition(100, 250);
	if (m_IndexSelected == 2)
		m_controller->setPosition(100, 150);
	if (m_IndexSelected == 3)
		m_controller->setPosition(100, 50);
}