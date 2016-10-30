#include "stdafx.h"
#include "Map.h"

using namespace TreadLightly::MapUtilities;

namespace TreadLightly {

	Map::Map(Ogre::SceneManager *mgr, Ogre::SceneNode *parent, const Ogre::String& mapFile) {

		_Data = new Data(mapFile);
		_Assets = new Assets(mgr, parent, *_Data);
		_PathFinder = new PathFinder(*_Data);
	}

	Map::~Map() {
		if (_PathFinder)
			delete _PathFinder;
		if (_Assets)
			delete _Assets;
		if (_Data)
			delete _Data;
	}

	void Map::SwitchAssets(const Ogre::String& name) {
		_Assets->ChangeCurrentAssetSceneNode(name);
	}

	bool Map::HasAsset(const Ogre::String& name) const {
		return _Assets->HasAsset(name);
	}
}