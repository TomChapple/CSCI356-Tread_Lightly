/*
* TREAD LIGHTLY
* Authors: Tom Chapple (29/10)
* File: MapAssets.h
* Last modified on: 29/10/16
* Purpose: Describes the assets used to display the map
* which can be generated from a Data class or loaded
* in separately.
*/

#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreSceneNode.h>

#include <string>
#include <map>
#include "MapData.h"

namespace TreadLightly {

	namespace MapUtilities {

		class Assets {

		public:

			/* ~~~ Types / Typedefs ~~~ */
			typedef std::map<Ogre::String, Ogre::SceneNode*>::iterator AssetIterator;
			typedef std::pair<Ogre::Real, Ogre::Real> ValueRange;

			/* ~~~ Constants ~~~ */
			static const Ogre::Real DEFAULT_GRID_SIZE;
			static const Ogre::String GRID_ASSET_NAME;

			/* ~~~ Constructors / Destructors ~~~ */
			Assets(Ogre::SceneManager *mgr, Ogre::SceneNode *parent);
			Assets(Ogre::SceneManager *mgr, Ogre::SceneNode *parent, const Data& mapData,
				const Ogre::Real& gridSize = DEFAULT_GRID_SIZE);
			~Assets();

			/* ~~~ Interface Methods ~~~ */

			void AddAssetSet(const Ogre::String& name, Ogre::SceneNode *node);
			void CreateAssetSetFromMapData(const Data& mapData,
				const Ogre::Real& gridSize = DEFAULT_GRID_SIZE,
				bool overwriteRanges = true);

			bool HasAsset(const Ogre::String& name) const;
			Ogre::SceneNode *GetAssetSceneNode(const Ogre::String& name);
			Ogre::SceneNode *GetCurrentAssetSceneNode();

			void ChangeCurrentAssetSceneNode(const Ogre::String& name);
			void ChangeCurrentAssetSceneNode(AssetIterator it);

			void RemoveAssetSceneNode(const Ogre::String& name);
			void RemoveAssetSceneNode(AssetIterator it);

			/* ~~~ Accessors / Mutators ~~~ */

			AssetIterator AssetBegin();
			AssetIterator AssetEnd();

			ValueRange GetXRange() const;
			void SetXRange(const ValueRange& x);
			void SetXRange(const Ogre::Real& min, const Ogre::Real& max);

			ValueRange GetYRange() const;
			void SetYRange(const ValueRange& y);
			void SetYRange(const Ogre::Real& min, const Ogre::Real& max);

		protected:

			/* ~~~ Members ~~~ */

			Ogre::SceneManager *_SceneManager;
			Ogre::SceneNode *_Parent;
			std::map<Ogre::String, Ogre::SceneNode*> _AssetSets;
			Ogre::String _CurrentAssetName;
			ValueRange _XRange, _YRange;
			

		};
	}
}