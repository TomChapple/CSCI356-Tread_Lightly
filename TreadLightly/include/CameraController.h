/*
* TREAD LIGHTLY
* Authors: Brett Latham (26/10/16)
* File: CameraController.h
* Last Edited on: 26/10/16
* Purpose: This file describes the movement of the camera in the scene.
*/


#include "stdafx.h"
#include "Ogre.h"

namespace TreadLightly {
	class CameraController {
	public:
		CameraController(Ogre::Camera* cam);
		CameraController(Ogre::Camera* cam, Ogre::SceneManager* mSceneMgr, Ogre::RenderWindow* window, OIS::Mouse* mouse);
		virtual ~CameraController(void);
		void setTarget(Ogre::SceneNode* target);
		void setWindow(Ogre::RenderWindow* window);
		void setMouse(OIS::Mouse* mouse);
		bool frameRenderingQueued(const Ogre::FrameEvent& evt, int xCoord, int yCoord);
		bool mouseMoved(const OIS::MouseEvent& evt);
		void mousePressed(const OIS::MouseEvent& evt);
		Ogre::Ray getSelectRay(){
			//return mCamera->getCameraToViewPortRay(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
		}
	private:	
		Ogre::RenderWindow* mWindow;
		OIS::Mouse* mMouse;
		OIS::Keyboard* mKeyboard;

		Ogre::Camera * mCamera;
		Ogre::SceneNode * mCameraNode;
		Ogre::SceneNode* camTarget;

		Ogre::Real mMove;
		Ogre::Vector3 mDirection;
	};
}
