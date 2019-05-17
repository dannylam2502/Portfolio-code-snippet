#ifndef __S_CAMERA_H__
#define __S_CAMERA_H__

#include "cocos2d.h"

USING_NS_CC;

class SCamera : public Node
{
public:
	static	SCamera*	getInstance();
			void		setCameraTarget(Node* cameraTarget);
			void		setCameraLimitedSize(const Size& size);
			Rect		getCameraViewPort();

	virtual bool	init();
	virtual	void	update(float delta);
	Vec2	getCameraPosition();

	CREATE_FUNC(SCamera);
private:
			Camera*	m_camera;
			Node*	m_cameraTarget;
			Size	m_cameraLimitedSize;
			Rect*	m_cameraStaticRange;
			Rect	m_cameraViewPort;
			Size	m_camraViewportSize;
			//Sprite*	m_testViewport;
			bool	m_cameraUpdated;
	static	SCamera*	m_instance;
};

#endif // !__S_CAMERA_H__
