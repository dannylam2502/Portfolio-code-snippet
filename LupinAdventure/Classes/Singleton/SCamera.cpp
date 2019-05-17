#include "SCamera.h"

#define WIN_SIZE					Director::getInstance()->getWinSize()
// Middle of window
#define CAMERA_VIEWPORT_HEIGHT		WIN_SIZE.operator*(0.4f).height
#define CAMERA_VIEWPORT_WIDTH		WIN_SIZE.operator*(0.2f).width
#define CAMERA_VIEWPORT				Size(CAMERA_VIEWPORT_WIDTH, CAMERA_VIEWPORT_HEIGHT)

// Both side
#define CAMERA_UNVIEWPORT_HEIGHT	WIN_SIZE.operator*(0.3f).height
#define CAMERA_UNVIEWPORT_WIDTH		WIN_SIZE.operator*(0.4f).width
#define CAMERA_UNVIEWPORT			Size(CAMERA_UNVIEWPORT_WIDTH, CAMERA_UNVIEWPORT_HEIGHT)

#define CAMERA_LIMITED_SIZE			Size(500.0f, 500.0f)

SCamera*	SCamera::m_instance = nullptr;

SCamera*	SCamera::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = SCamera::create();
	}

	return m_instance;
}

bool	SCamera::init()
{
	if (!Node::init())
	{
		return false;
	}

	Size l_winSize		= WIN_SIZE;

	m_camera			= Camera::createOrthographic(l_winSize.width, l_winSize.height, 0, 10000);

	m_camraViewportSize = CAMERA_VIEWPORT;
	m_cameraStaticRange	= new Rect(CAMERA_UNVIEWPORT_WIDTH,CAMERA_UNVIEWPORT_HEIGHT, m_camraViewportSize.width, m_camraViewportSize.height);
	m_cameraLimitedSize = CAMERA_LIMITED_SIZE;
	m_cameraViewPort = Rect(0, 0, l_winSize.width, l_winSize.height);

	//m_testViewport		= Sprite::create();
	//m_testViewport->setPhysicsBody(PhysicsBody::createBox(m_camraViewportSize));
	//m_testViewport->setPosition(WIN_SIZE.operator/(2));

	//this->addChild(m_testViewport);
	this->addChild(m_camera);

	scheduleUpdate();

	return true;
}

void	SCamera::update(float detla)
{
	m_cameraUpdated = false;
	if (m_cameraTarget == nullptr)
		return;
	if (!m_cameraStaticRange->containsPoint(m_cameraTarget->getPosition()))
	{
		// Camera move right
		if (m_cameraTarget->getPositionX() > m_cameraStaticRange->getMaxX())
		{
			float	deltaPositionX = m_cameraTarget->getPositionX() - m_cameraStaticRange->getMaxX();
			if (m_camera->getPositionX() + deltaPositionX < m_cameraLimitedSize.width)
				m_camera->setPositionX(m_camera->getPositionX() + deltaPositionX);
			else
				m_camera->setPositionX(m_cameraLimitedSize.width);
			m_cameraUpdated = true;
		}
		// Camera move left
		if (m_cameraTarget->getPositionX() < m_cameraStaticRange->getMinX())
		{
			float	deltaPositionX = m_cameraTarget->getPositionX() - m_cameraStaticRange->getMinX();
			if (m_camera->getPositionX() + deltaPositionX > 0)
				m_camera->setPositionX(m_camera->getPositionX() + deltaPositionX);
			else
				m_camera->setPositionX(0.0f);
			m_cameraUpdated = true;
		}
		// Camera move top
		if (m_cameraTarget->getPositionY() > m_cameraStaticRange->getMaxY())
		{
			float deltaPositionY = m_cameraTarget->getPositionY() - m_cameraStaticRange->getMaxY();
			if (m_camera->getPositionY() + deltaPositionY < m_cameraLimitedSize.height)
				m_camera->setPositionY(m_camera->getPositionY() + deltaPositionY);
			else
				m_camera->setPositionY(m_cameraLimitedSize.height);
			m_cameraUpdated = true;
		}
		// Camera move bottom
		if (m_cameraTarget->getPositionY() < m_cameraStaticRange->getMinY())
		{
			float deltaPositionY = m_cameraTarget->getPositionY() - m_cameraStaticRange->getMinY();
			if (m_camera->getPositionY() + deltaPositionY > 0)
				m_camera->setPositionY(m_camera->getPositionY() + deltaPositionY);
			else
				m_camera->setPositionY(0);
			m_cameraUpdated = true;
		}
		// Update camera viewport
		if (m_cameraUpdated)
		{
			m_cameraStaticRange->setRect(m_camera->getPositionX() + CAMERA_UNVIEWPORT_WIDTH, m_camera->getPositionY() + CAMERA_UNVIEWPORT_HEIGHT, m_camraViewportSize.width, m_camraViewportSize.height);
			m_cameraViewPort.origin.x = m_camera->getPositionX();
			m_cameraViewPort.origin.y = m_camera->getPositionY();
			//m_testViewport->setPosition(m_camera->getPosition() + WIN_SIZE.operator/(2));
		}
	}
}

void	SCamera::setCameraTarget(Node* cameraTarget)
{
	m_cameraTarget = cameraTarget;
}

void	SCamera::setCameraLimitedSize(const Size& size)
{
	m_cameraLimitedSize = size;
	m_cameraLimitedSize.width -= WIN_SIZE.width;
	m_cameraLimitedSize.height -= WIN_SIZE.height;
	if (m_cameraLimitedSize.height < 0)
		m_cameraLimitedSize.height = 0;
}

Vec2	SCamera::getCameraPosition()
{
	return m_camera->getPosition();
}

Rect SCamera::getCameraViewPort()
{
	return m_cameraViewPort;
}