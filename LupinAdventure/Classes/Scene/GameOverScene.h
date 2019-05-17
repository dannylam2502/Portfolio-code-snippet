#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__


#include "cocos2d.h"
USING_NS_CC;

class GameOverScene :
	public Scene
{
public:

	static void initialize();
	static void release();

	static	GameOverScene*	get();
	//virtual void update(float delta);
	void	GameOverScene::CloseWindows(Ref*);
	void	GameOverScene::PlayAgain(Ref*);
	void	GameOverScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

private:

	virtual bool init();

	CREATE_FUNC(GameOverScene);
	static	GameOverScene*		m_Instance;
	static	int					m_IndexSelected;
	static	Sprite*				m_controller;
};

#endif