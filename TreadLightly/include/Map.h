#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "map/MapAssets.h"
#include "map/MapData.h"
#include "map/PathFinder.h"

namespace TreadLightly {

	class Map {

	public:

		/* ~~~ Types / Typedefs ~~~ */
		typedef std::pair<MapUtilities::pos_type, MapUtilities::pos_type> GridPosition;

		/* ~~~ Constructors / Destructors ~~~ */

		Map(Ogre::SceneManager *mgr, const Ogre::String& mapFile);
		~Map();

		/* ~~~ Interface Methods ~~~ */

		void SwitchAssets(const Ogre::String& name);
		bool HasAsset(const Ogre::String& name) const;
		void AddAsset(const Ogre::String& name, Ogre::SceneNode *node);

		void FindPath(const Ogre::Vector3& from,
			const Ogre::Vector3& to,
			std::vector<Ogre::Vector3>& insert) const;


	protected:

		/* ~~~ Members ~~~ */
		MapUtilities::Assets *_Assets;
		MapUtilities::Data *_Data;
		MapUtilities::PathFinder *_PathFinder;

		/* ~~~ Internal Functions ~~~ */
		Ogre::Vector3 _GridToOgrePosition(const GridPosition& pos) const;
		GridPosition _OgreToGridPosition(const Ogre::Vector3& pos) const;
	};
}