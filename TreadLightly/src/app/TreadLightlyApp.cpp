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
#include "Unit.h"

namespace TreadLightly {

	//-------------------------------------------------------------------------------------
	TreadLightlyApp::TreadLightlyApp(void)
	{
		_Map = NULL;
	}
	//-------------------------------------------------------------------------------------
	TreadLightlyApp::~TreadLightlyApp(void)
	{
		if (!_Map)
			delete _Map;

		for (Unit *unit : _RedTeam)
			delete unit;
		for (Unit *unit : _BlueTeam)
			delete unit;
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

		/* Create Map */
		_Map = new Map(mSceneMgr, mSceneMgr->getRootSceneNode(), "testmap.bmp");

		/* Create some units */
		_RedTeam.push_back(new Unit(mSceneMgr, _Map, Ogre::Vector3(-55, 0, 45)));
		_RedTeam.push_back(new Unit(mSceneMgr, _Map, Ogre::Vector3(105, 0, -75)));

		/* Test Pathfinding */
		/*std::vector<Ogre::Vector3> FoundPath;
		_Map->FindPath(Ogre::Vector3(-55, 0, 45), Ogre::Vector3(105, 0, -75), FoundPath);
		if (!FoundPath.empty()) {
			Ogre::ManualObject *Path = mSceneMgr->createManualObject();
			Path->begin("", Ogre::RenderOperation::OT_LINE_STRIP);
			for (Ogre::Vector3& node : FoundPath) {
				Path->position(node);
			}
			Path->end();

			mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(Path);
		}*/
	}

	Map *TreadLightlyApp::GetMap() {
		return _Map;
	}

}
