#include "stdafx.h"
#include "CameraController.h"

namespace TreadLightly {
	
	CameraController::CameraController(Ogre::Camera* cam) {
		mCamera = cam;
	}

	CameraController::CameraController(Ogre::Camera* cam, Ogre::SceneManager* mSceneMgr, Ogre::RenderWindow* window, OIS::Mouse* mouse) {
		mCamera =  cam;
		camTarget = mSceneMgr->getRootSceneNode()->createChildSceneNode("camTargNode", Ogre::Vector3(0, 0, 0));
		mCameraNode = camTarget->createChildSceneNode("cameraNode", Ogre::Vector3(0, 200, 0));
		mCameraNode->setAutoTracking(true, camTarget);
		mCameraNode->setFixedYawAxis(true);
		mCameraNode->attachObject(mCamera);

		setWindow(window);
		setMouse(mouse);

		mMove = 200;
	}

	CameraController::~CameraController() {

	}

	void CameraController::setTarget(Ogre::SceneNode* target) {
		if (target != camTarget)
		{
			camTarget = target;
			if (target)
			{
				mCamera->setPosition(camTarget->_getDerivedPosition());
				mCamera->moveRelative(Ogre::Vector3(0, 200, 0));
				mCamera->setAutoTracking(true, camTarget);
			}
			else
			{
				mCamera->setAutoTracking(false);
			}
		}
	}

	void CameraController::setWindow(Ogre::RenderWindow* window) {
		mWindow = window;
	}

	void CameraController::setMouse(OIS::Mouse* mouse) {
		mMouse = mouse;
	}

	bool CameraController::frameRenderingQueued(const Ogre::FrameEvent& evt, int xCoord, int yCoord)
	{

		if (xCoord > (mWindow->getWidth()-20)) {
			mDirection.x -= mMove;
		}
		else if (xCoord < 20) {
			mDirection.x = mMove;
		}
		if (yCoord > (mWindow->getHeight()-20)) {
			mDirection.z -= mMove;
		}
		else if (yCoord < 20) {
			mDirection.z = mMove;
		}
		camTarget->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		mDirection = Ogre::Vector3::ZERO;


		return true;
	}
}