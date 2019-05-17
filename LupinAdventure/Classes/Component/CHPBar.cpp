#include "Component/CHPBar.h"
#include "Utils.h"
#include "Scene\GamePlayScene.h"

CHPBar::~CHPBar()
{
	GamePlayScene::get()->m_gameworldLayer->removeChild(m_HPBackground);
	GamePlayScene::get()->m_gameworldLayer->removeChild(m_HPBar);
}

void CHPBar::addComponentTo(EObject* object)
{
	auto component = CHPBar::create();

	component->m_Object = object;
	component->m_HPBackground = Sprite::create("GUI//HPBossBackground.png");
	component->m_HPBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	component->m_HPBar = Sprite::create("GUI//HPBoss.png");
	component->m_HPBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	component->m_HPBackground->setPosition(400, 600 - 30);
	component->m_HPBar->setPosition(400, 600 - 30);

	component->m_Stats = (CStats*)object->getChildByTag(TAG::COMPONENT::STATS);
	component->m_OffSet = Vec2(0, 0);

	object->addChild(component);
	GamePlayScene::get()->m_UILayer->addChild(component->m_HPBackground,Z_ORDER::UI);
	GamePlayScene::get()->m_UILayer->addChild(component->m_HPBar, Z_ORDER::UI);
}

void CHPBar::addComponentTo(EObject* object, Vec2 offset, Size bodySize)
{
	auto component = CHPBar::create();

	component->m_Object = object;
	component->m_HPBackground = Sprite::create("GUI//HPBossBackground.png");
	component->m_HPBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	component->m_HPBar = Sprite::create("GUI//HPBoss.png");
	component->m_HPBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	component->m_HPBackground->setScaleX(bodySize.width / 120);
	component->m_HPBackground->setPosition(400, 600 - 30);
	component->m_HPBar->setPosition(400, 600 - 30);
	component->m_HPBar->setScaleX(bodySize.width / 120);

	component->m_Stats = (CStats*)object->getChildByTag(TAG::COMPONENT::STATS);
	component->m_OffSet = offset;

	object->addChild(component);
	GamePlayScene::get()->m_gameworldLayer->addChild(component->m_HPBackground, Z_ORDER::UI);
	GamePlayScene::get()->m_gameworldLayer->addChild(component->m_HPBar, Z_ORDER::UI);
}

void CHPBar::commandProcess(int command, float deltaTime)
{
	/*switch (command)
	{
	case COMMAND::COMPONENT::STATS::RESET:
		m_HP = m_MaxHP;
		m_MP = m_MaxMP;
		m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::STAND);
		break;
	default:
		break;
	}*/
}

void CHPBar::update(float delta)
{
	//m_BossBackground = Sprite::create("GUI//HPBossBackground.png");
	m_RectWidth = m_Stats->getHP() * 120 / m_Stats->getMaxHP();
	if (m_RectWidth <= 0)
		m_RectWidth = 0;
	//m_HPBar = Sprite::create("GUI//HPBoss.png", Rect(0, 0, m_RectWidth, 20));
	m_HPBar->setTextureRect(Rect(0, 0, m_RectWidth, 15));
	m_HPBackground->setPosition(m_Object->getPosition() + m_OffSet);
	m_HPBar->setPosition(m_Object->getPosition() + m_OffSet);
	/*this->addChild(m_HPBackground);
	this->addChild(m_HPBar);*/
}

void CHPBar::setProperty(int parameterTypeFlag, UParam parameter)
{
	//switch (parameterTypeFlag)
	//{
	//case UPARAM_TYPE_CSTATS:
	//{
	//						   /*if (isStringEqual(m_Object->getName().c_str(), NAME::PLAYER))
	//						   {
	//						   CCLOG("Stats affected");
	//						   }*/
	//						   CStats* source = parameter.uStats;
	//						   addStats(
	//							   source->m_HP,
	//							   source->m_MP,
	//							   source->m_MaxHP,
	//							   source->m_MaxMP,
	//							   source->m_life);

	//						   if (m_HP <= 0)
	//						   {
	//							   m_Object->pushCommand(COMMAND::COMPONENT::STATE::CHANGE_TO::DIE);
	//							   return;
	//						   }

	//						   if (source->m_HP < 0)
	//						   {
	//							   /*if (isStringEqual(m_Object->getName().c_str(), ID::PLAYER))
	//							   {
	//							   CCLOG("HIT = %d", source->m_HP);
	//							   }*/
	//							   //source->m_HP = 0;
	//							   m_Object->commandProcess(COMMAND::COMPONENT::STATE::CHANGE_TO::HIT);
	//							   m_Object->pushCommand(COMMAND::COMPONENT::MOVE::KNOCK_BACK);
	//						   }
	//}
	//	break;
	//default:
	//	break;
	//}
}

bool CHPBar::init()
{
	if (!CBase::init())
	{
		return false;
	}
	setTag(TAG::COMPONENT::HPBAR);
	m_HPBackground = nullptr;
	m_HPBar = nullptr;
	scheduleUpdate();
	return true;
}

CBase* CHPBar::clone(EObject* object)
{
	auto cloneOne = CHPBar::create();

	return cloneOne;
}