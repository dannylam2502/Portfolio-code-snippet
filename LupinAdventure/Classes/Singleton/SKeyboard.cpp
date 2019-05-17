#include "SKeyboard.h"

SKeyboard			* SKeyboard::m_Instance =  nullptr;

// To directly get the current state of the passed key by it's key code
KeyState SKeyboard::getKeyState(EventKeyboard::KeyCode keyCode)
{
	return get()->m_KeyMap[(unsigned int)keyCode];
}

// To get the keyboard events since prev update
vector<KeyEvent*> SKeyboard::getEvents()
{
	return get()->m_EventStack;
}


bool SKeyboard::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Reset keyboard to all key up
	memset(m_KeyMap, 0, sizeof(KeyState) * KEYMAP_SIZE);

	// Create listener to catch the keyboard events
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(SKeyboard::onKeyPress, this);
	listener->onKeyReleased = CC_CALLBACK_2(SKeyboard::onKeyRelease, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// To call the update method every round of game loop
	scheduleUpdate();

	return true;
}


void SKeyboard::update(float delta)
{
	// Sette all the key in pressed or released state from prev loop to up and down state
	for (size_t i = 0; i < m_EventStack.size(); ++i)
	{
		if (m_EventStack.at(i)->stateEvent == KS_PRESS)
		{
			m_KeyMap[(unsigned int)m_EventStack.at(i)->keyCode] = KS_DOWN;
		}

		if (m_EventStack.at(i)->stateEvent == KS_RELEASE)
		{
			m_KeyMap[(unsigned int)m_EventStack.at(i)->keyCode] = KS_UP;
		}
	}

	// Clear events from prev game loop to prepare for current loop
	m_EventStack.clear();
}


SKeyboard::SKeyboard()
{
	m_KeyMap = new KeyState[KEYMAP_SIZE];
	m_EventStack.clear();
	init();
}

SKeyboard::~SKeyboard()
{
	CC_SAFE_DELETE(m_KeyMap);
	m_EventStack.clear();
}


// get Instance method
SKeyboard* SKeyboard::get()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SKeyboard();
	}

	return m_Instance;
}

// To release Keyboard manager after use
void SKeyboard::release()
{
	if (m_Instance != nullptr)
	{
		m_Instance->removeFromParentAndCleanup(true);
	}
}

// Event called methods
void SKeyboard::onKeyPress(EventKeyboard::KeyCode code, Event* kEvent)
{
	m_KeyMap[(unsigned int)code] = KS_PRESS;
	m_EventStack.push_back(new KeyEvent(code, KS_PRESS));
}

void SKeyboard::onKeyRelease(EventKeyboard::KeyCode code, Event* kEvent)
{
	m_KeyMap[(unsigned int)code] = KS_RELEASE;
	m_EventStack.push_back(new KeyEvent(code, KS_RELEASE));
}
// =======================================================