#ifndef __GAMEMENU_SCENE_H_
#define __GAMEMENU_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class GameMenuScene : public Scene
{
public:

	static void initialize();
	static void release();
	
	static	GameMenuScene*	get();

	void	GameMenuScene::Exit(Ref*);
	void	GameMenuScene::Play(Ref*);
	void	GameMenuScene::About(Ref*);
	void	GameMenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

private:

	virtual bool init();

	CREATE_FUNC(GameMenuScene);
	static	GameMenuScene*		m_Instance;
	static	int					m_IndexSelected;
	static	Sprite*				m_controller;
};

#endif