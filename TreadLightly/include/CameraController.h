/*
* TREAD LIGHTLY
* Authors: Brett Latham (26/10/16)
* File: CameraController.h
* Last Edited on: 26/10/16
* Purpose: This file describes the movement of the camera in the scene.
*/


#include "stdafx.h"

namespace TreadLightly {
	class CameraController {
	public:
		CameraController(Ogre::Camera* cam);
		virtual ~CameraController();
		virtual void Ogre::Camera* getCamera();
		virtual void Ogre::SceneNode* getCameraNode();
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		virtual void mouseMoved(const OIS::MouseEvent& evt);
		virtual void mousePressed(const (OIS::MouseEvent& evt);
		virtual Ogre::Ray getSelectRay(){
			return mCamera->getCameraToViewPortRay(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
		}
	private:	
		Ogre::Camera * mCamera;
		Ogre::SceneNode * mCameraNode;

		OIS::Mouse * mMouse;
	}
}
