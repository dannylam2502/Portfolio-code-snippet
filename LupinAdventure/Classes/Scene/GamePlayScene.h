// IMPORTANT: GamePlayScene is not allowed to add any child but the Layers
// The other node(s) must be added into the suited Layer.

#ifndef GamePlayScene_h__
#define GamePlayScene_h__

#include "cocos2d.h"
#include <map>
#include "Box2D/Box2D.h"
USING_NS_CC;
using namespace std;

#include "TileMap/TileMap.h"

#pragma region Layers
// GameWorldLayer: contain the EObject(s) from the game.
class GameWorldLayer : public Layer
{
public:
	CREATE_FUNC(GameWorldLayer);
	static GameWorldLayer* create(b2World* physicsWorld);

	virtual bool init();

	virtual void update(float delta);

	virtual void onExit();

	EObject* l_Player;

private:
	Camera*		m_camera;
	b2World*	m_PhysicsWorld;
};

// BackgroundLayer: contain the Sprite(s) or Animation(s) of the game
// Accessible by the Map
class BackgroundLayer : public Layer
{
public:
	CREATE_FUNC(BackgroundLayer);

	virtual bool init();

	virtual void update(float delta);

	LayerColor *m_color;
};

// UILayer: contain the User Interface object(s)
class UILayer : public Layer
{
public:
	CREATE_FUNC(UILayer);

	virtual bool init();

	virtual void update(float delta);

	void initPlayer(EObject* player);

private:
	// Related Stat component

	int i = 0;
	EObject* m_Player;
	CStats*	m_PlayerStats;
	Sprite*	m_hp;
	Sprite* m_mp;
	Sprite* m_HPbackground;
	Sprite* m_MPbackground;
	Size winsize;

	LabelTTF* m_HPLabel;
	LabelTTF* m_MPLabel;

	float rectWidth;

};
#pragma endregion

#pragma region Scene
class GamePlayScene : public Scene
{
public:
	UILayer*			m_UILayer;
	BackgroundLayer*	m_backgroundLayer;
	GameWorldLayer*		m_gameworldLayer;
public:
	~GamePlayScene();
	static void initialize();
	static void release();
	Layer*	getLayer(int zOrder);

	static GamePlayScene*	get();
	static b2World*	getPhysicsWorld();

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);


private:
	virtual bool init();

	CREATE_FUNC(GamePlayScene);

	using Node::addChild;

	virtual void addChild(Node * child, int localZOrder);

		

private:
	map<int, Layer*>	m_Layers;
	b2World*			m_PhysicsWorld;

	static GamePlayScene*		m_Instance;
};
#pragma endregion

#endif // GamePlayScene_h__