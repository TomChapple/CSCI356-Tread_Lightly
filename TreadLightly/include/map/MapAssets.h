/*
* TREAD LIGHTLY
* Authors: Tom Chapple (26/10)
* File: MapAssets.h
* Last modified on: 26/10/16
* Purpose: Describes the assets used to display the map
* which can be generated from a Data class or loaded
* in separately.
*/

#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>

#include <string>
#include <map>
#include "MapData.h"

namespace TreadLightly {

	namespace MapUtilities {

		class Assets {

		public:

			/* ~~~ Types / Typedefs ~~~ */
			typedef std::map<Ogre::String, Ogre::SceneNode*>::iterator AccessIterator;

			/* ~~~ Constructors / Destructors ~~~ */
			Assets(Ogre::SceneManager *mgr);
			Assets(Ogre::SceneManager *mgr, const Data& mapData);
			~Assets();

			/* ~~~ Interface Methods ~~~ */

			void AddAssetSet(const Ogre::String& name, Ogre::SceneNode *node);
			void CreateAssetSetFromMapData(const Ogre::String& name, const Data& mapData);

			Ogre::SceneNode *GetAssetSceneNode(const Ogre::String& name);
			Ogre::SceneNode *GetCurrentAssetSceneNode();


			void ChangeCurrentAssetSceneNode(const Ogre::String& name);
			void ChangeCurrentAssetSceneNode(AccessIterator it);

			AccessIterator AssetBegin();
			AccessIterator AssetEnd();

		protected:

			/* ~~~ Members ~~~ */

			Ogre::SceneManager *_SceneManager;
			std::map<Ogre::String, Ogre::SceneNode*> _AssetSets;
			Ogre::String _CurrentAssetName;

		};
	}
}