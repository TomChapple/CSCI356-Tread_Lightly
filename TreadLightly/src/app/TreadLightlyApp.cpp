/*
-----------------------------------------------------------------------------
Filename:    TreadLightlyApp.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "TreadLightlyApp.h"

#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreManualObject.h>

#include "Map.h"

namespace TreadLightly {

	//-------------------------------------------------------------------------------------
	TreadLightlyApp::TreadLightlyApp(void)
	{
		_Map = NULL;
		_CamControl = NULL;
	}
	//-------------------------------------------------------------------------------------
	TreadLightlyApp::~TreadLightlyApp(void)
	{
		if (!_Map)
			delete _Map;
		if (!_CamControl)
			delete _CamControl;

		for (Unit* unit : _RedTeam)
			delete unit;
		for(Unit* unit : _BlueTeam)
			delete unit;
	}
	//-------------------------------------------------------------------------------------
	bool TreadLightlyApp::setup() {
		BaseApplication::setup();

		mTrayMgr->showCursor();

		//Position mouse in middle of screen so it doesn't start moving camera
		OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(mMouse->getMouseState());
		mutableMouseState.X.abs = mWindow->getWidth()/2;
		mutableMouseState.Y.abs = mWindow->getHeight()/2;

		return true;
	}

	//-------------------------------------------------------------------------------------
	void TreadLightlyApp::createScene(void)
	{
		// create your scene here :)

		// Set up camera
		_CamControl = new CameraController(mCamera, mSceneMgr, mWindow, mMouse);

		// Set the scene's ambient light
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 
		// Create an Entity
		Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
 
		// Create a SceneNode and attach the Entity to it
		Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
		headNode->attachObject(ogreHead);
 
		// Create a Light and set its position
		Ogre::Light* light = mSceneMgr->createLight("MainLight");
		light->setPosition(20.0f, 80.0f, 50.0f);

		/* Create Map */
		_Map = new Map(mSceneMgr, mSceneMgr->getRootSceneNode(), "heightmap2.png");

		/* Create some Units */
		_RedTeam.push_back(new Unit(mSceneMgr, _Map, Ogre::Vector3(-55, 0, 45)));
	}

	
	bool TreadLightlyApp::frameRenderingQueued(const Ogre::FrameEvent& evt) {
		
		if(mWindow->isClosed())
			return false;
 
		if(mShutDown)
			return false;

		mKeyboard->capture();
		mMouse->capture();

		mTrayMgr->frameRenderingQueued(evt);
		_CamControl->frameRenderingQueued(evt, mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);

		for (Unit *unit : _RedTeam)
			unit->update(evt.timeSinceLastFrame);
		for (Unit *unit : _BlueTeam)
			unit->update(evt.timeSinceLastFrame);

		return true;
	}

	bool TreadLightlyApp::mouseMoved( const OIS::MouseEvent &arg )
	{
		
		_CamControl->mouseMoved(arg);

		if (mTrayMgr->injectMouseMove(arg)) return true;

		return true;
	}

}
