/*
* TREAD LIGHTLY
* Authors: Tom Chapple (30/10)
* File: Unit.h
* Last Edited: 30/10/6
* Purpose: Defines a unit that is to move around on the
* battlefield.
*/

#pragma once

#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <vector>

#include "Map.h"

namespace TreadLightly {

	class TreadLightlyApp;

	class Unit {

	public:

		/* ~~~ Constructors / Destructors ~~~ */

		Unit(Ogre::SceneManager *mgr, Map *map,
			const Ogre::Vector3& pos = Ogre::Vector3::ZERO);
		virtual ~Unit();

		/* ~~~ Interface Methods ~~~ */
		virtual bool update(const Ogre::Real& t);

		void MoveTo(const Ogre::Vector3& pos);

	protected:

		/* ~~~ Members ~~~ */
		Map *_Map;
		Ogre::SceneManager *_SceneManager;

		Ogre::Vector3 _CurrDest;
		std::vector<Ogre::Vector3> _FollowPath;

		Ogre::Vector3 _Velocity;
		bool _ReachedDestination;

		Ogre::SceneNode *_Node;
		Ogre::Entity *_Entity;
	};
}