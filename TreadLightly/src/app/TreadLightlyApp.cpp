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

#include "map/MapData.h"
#include "map/PathFinder.h"
#include "map/MapAssets.h"

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

		/* Create the MapData and test iterator */
		Ogre::String TestName = "testmap.png";
		size_t Temp = sizeof(MapUtilities::Cell);
		MapUtilities::Data TestData(TestName);
		for (MapUtilities::Data::iterator it = TestData.begin(); it != TestData.end(); it++) {
			Ogre::String Message;
			Message += Ogre::StringConverter::toString(it->GetX()) +
				"," + Ogre::StringConverter::toString(it->GetY()) + ":";
			Message += " Zone - " + Ogre::StringConverter::toString(it->GetZone()) +
				", Team - " + Ogre::StringConverter::toString(it->GetTeam()) +
				", Traverse - " + Ogre::StringConverter::toString(it->GetTraverseType());
			Ogre::LogManager::getSingletonPtr()->logMessage(Message);
		}

		/* Test out the PathFinder */
		MapUtilities::PathFinder PF(TestData);
		std::vector<MapUtilities::Cell> PathResults;
		if (PF.FindPath(0, 14, 22, 7, PathResults)) {
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Found Path!"));
		}
		else {
			Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::String("Could not find path..."));
		}

		/* Test out of assets */
		MapUtilities::Assets *TestAssets = new MapUtilities::Assets(mSceneMgr, mSceneMgr->getRootSceneNode(), TestData);
	}

}
