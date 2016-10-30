#include "stdafx.h"
#include "Map.h"

using namespace TreadLightly::MapUtilities;

namespace TreadLightly {

	const Ogre::Real Map::DEFAULT_GRID_SIZE = 10.0f;

	Map::Map(Ogre::SceneManager *mgr, Ogre::SceneNode *parent, const Ogre::String& mapFile,
		const Ogre::Real& gridSize) {

		_GridSize = gridSize;

		_Data = new Data(mapFile);
		_Assets = new Assets(mgr, parent, *_Data, gridSize);
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

	void Map::AddAsset(const Ogre::String& name, Ogre::SceneNode *node) {
		_Assets->AddAssetSet(name, node);
	}

	bool Map::FindPath(const Ogre::Vector3& from,
		const Ogre::Vector3& to,
		std::vector<Ogre::Vector3>& store) const {
	
		/* Call the pathfinding class */
		std::vector<Cell> CellPath;
		if (!_PathFinder->FindPath(_OgreToGridPosition(from),
			_OgreToGridPosition(to), CellPath))
			return false;

		/* Transfer into Ogre::Vector3s */
		for (Cell& cell : CellPath) {
			store.insert(store.begin(), _GridToOgrePosition(cell.GetPoint()));
		}

		return true;
	}

	Ogre::Real Map::GetGridSize() const {
		return _GridSize;
	}

	Ogre::Vector3 Map::_GridToOgrePosition(const GridPosition& pos) const {

		Ogre::Vector3 ReturnVec = Ogre::Vector3::ZERO;
		ReturnVec.x = _Assets->GetXRange().first + pos.first * _GridSize;
		ReturnVec.z = _Assets->GetYRange().first + pos.second * _GridSize;

		return ReturnVec;
	}

	Map::GridPosition Map::_OgreToGridPosition(const Ogre::Vector3& pos) const {

		GridPosition ReturnPos;
		ReturnPos.first = (pos.x - _Assets->GetXRange().first) / _GridSize;
		ReturnPos.second = (pos.z - _Assets->GetYRange().first) / _GridSize;

		return ReturnPos;
	}
}