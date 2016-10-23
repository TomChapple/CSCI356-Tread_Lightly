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

namespace TreadLightly {

	//-------------------------------------------------------------------------------------
	TreadLightlyApp::TreadLightlyApp(void)
	{
	}
	//-------------------------------------------------------------------------------------
	TreadLightlyApp::~TreadLightlyApp(void)
	{
	}

	//-------------------------------------------------------------------------------------
	void TreadLightlyApp::createScene(void)
	{
		// create your scene here :)

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

	}

}
