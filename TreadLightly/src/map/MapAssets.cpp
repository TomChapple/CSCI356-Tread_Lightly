#include "stdafx.h"
#include "map/MapAssets.h"

#include <OgreVector3.h>
#include <OgreEntity.h>

namespace TreadLightly {

	namespace MapUtilities {

		const Ogre::Real Assets::DEFAULT_GRID_SIZE = 10.0f;
		const Ogre::String Assets::GRID_ASSET_NAME = "GridAsset";

		Assets::Assets(Ogre::SceneManager *mgr, Ogre::SceneNode *parent):
			_SceneManager(mgr), _Parent(parent), _CurrentAssetName("") {

		}

		Assets::Assets(Ogre::SceneManager *mgr, Ogre::SceneNode *parent, const Data& mapData,
			const Ogre::Real& gridSize):
			_SceneManager(mgr), _Parent(parent), _CurrentAssetName("") {

			/* Create the Map right off the bat */
			CreateAssetSetFromMapData(mapData, gridSize);
		}

		Assets::~Assets() {

			/* Detach current from parent */
			auto FindIt = _AssetSets.find(_CurrentAssetName);
			if (FindIt != _AssetSets.end()) {
				_Parent->removeChild(FindIt->second);
			}

			/* Destroy generated Asset from MapData */
			FindIt = _AssetSets.find(GRID_ASSET_NAME);
			if (FindIt != _AssetSets.end()) {
				FindIt->second->removeAndDestroyAllChildren();
				_SceneManager->destroySceneNode(FindIt->second);
			}
		}

		void Assets::AddAssetSet(const Ogre::String& name, Ogre::SceneNode *node) {

			/* Disallow the use of the default grid name */
			if (name == GRID_ASSET_NAME) {
				throw std::runtime_error("Cannot add an asset with the same name as the grid");
			}

			/* Check if the entry exists and overwrite it if so */
			auto FindIt = _AssetSets.find(name);
			if (FindIt != _AssetSets.end()) {
				RemoveAssetSceneNode(FindIt);
			}

			/* Add the new Scene Node now */
			_AssetSets.insert(std::make_pair(name, node));
			if (_AssetSets.size() == 1) {
				_CurrentAssetName = name;
				_Parent->addChild(node);
			}
		}

		void Assets::CreateAssetSetFromMapData(const Data& mapData,
			const Ogre::Real& gridSize, bool overwriteRanges) {

			/* Prepare entities and scaling */
			const Ogre::String BLOCKED_ENT = "cube.mesh";
			const Ogre::Vector3 BASE_SCALE(0.01, 0.01, 0.01);
			Ogre::Vector3 EntScale = BASE_SCALE * gridSize;
			Ogre::Vector3 EntOffset = Ogre::Vector3::ZERO;

			Ogre::Entity *TestEnt = _SceneManager->createEntity(BLOCKED_ENT);
			EntOffset.y = TestEnt->getBoundingBox().getHalfSize().y * EntScale.y;
			_SceneManager->destroyEntity(TestEnt);

			// Set start point so that the map is centred
			EntOffset.x = -mapData.GetWidth() * gridSize / 2;
			EntOffset.z = -mapData.GetHeight() * gridSize / 2;

			/* Create central SceneNode for all objects to be parented to */
			Ogre::SceneNode *CentralNode = _SceneManager->createSceneNode(GRID_ASSET_NAME + "Node");

			/* Create nodes for each blocked piece of the ground */
#ifdef _DEBUG
			unsigned int BlockCount = 0;
#endif
			for (auto CurrCell = mapData.begin(); CurrCell != mapData.end(); CurrCell++) {

				/* If it isn't traversable, put a block there */
				if (!CurrCell->IsTraversable()) {
					Ogre::Vector3 BlockPosition = EntOffset;
					BlockPosition.x += CurrCell->GetX() * gridSize;
					BlockPosition.z += CurrCell->GetY() * gridSize;

					Ogre::Entity *BlockEnt = _SceneManager->createEntity(BLOCKED_ENT);
					Ogre::SceneNode *BlockNode = CentralNode->createChildSceneNode(BlockPosition);
					BlockNode->setScale(EntScale);
					BlockNode->attachObject(BlockEnt);
				}
#ifdef _DEBUG
				BlockCount++;
#endif
			}

			/* Add to Asset List */
			auto FindIt = _AssetSets.find(GRID_ASSET_NAME);
			if (FindIt != _AssetSets.end()) {
				RemoveAssetSceneNode(FindIt);
			}

			/* Add the new Scene Node now */
			_AssetSets.insert(std::make_pair(GRID_ASSET_NAME, CentralNode));
			if (_AssetSets.size() == 1) {
				_CurrentAssetName = GRID_ASSET_NAME;
				_Parent->addChild(CentralNode);
			}

			if (overwriteRanges) {
				_XRange = std::make_pair(EntOffset.x, EntOffset.x + mapData.GetWidth() * gridSize);
				_YRange = std::make_pair(EntOffset.z, EntOffset.z + mapData.GetHeight() * gridSize);
			}
		}

		bool Assets::HasAsset(const Ogre::String& name) const {
			auto FindIt = _AssetSets.find(name);
			if (FindIt == _AssetSets.end())
				return false;
			return true;
		}

		Ogre::SceneNode *Assets::GetAssetSceneNode(const Ogre::String& name) {

			auto FindIt = _AssetSets.find(name);
			if (FindIt == _AssetSets.end())
				throw std::runtime_error("Cannot find Asset \'" + name + '\'');

			return FindIt->second;
		}

		Ogre::SceneNode *Assets::GetCurrentAssetSceneNode() {
			auto FindIt = _AssetSets.find(_CurrentAssetName);
			if (FindIt == _AssetSets.end())
				throw std::runtime_error("Could not find current Asset (empty?)");

			return FindIt->second;
		}

		void Assets::ChangeCurrentAssetSceneNode(const Ogre::String& name) {
			auto FindIt = _AssetSets.find(name), CurrIt = _AssetSets.find(_CurrentAssetName);
			if (FindIt == _AssetSets.end())
				throw std::runtime_error("Cannot find Asset \'" + name + '\'');

			ChangeCurrentAssetSceneNode(FindIt);
		}

		void Assets::ChangeCurrentAssetSceneNode(AssetIterator it) {
			auto CurrIt = _AssetSets.find(_CurrentAssetName);
			if (CurrIt != _AssetSets.end())
				_Parent->removeChild(CurrIt->first);

			_Parent->addChild(it->second);
			_CurrentAssetName = it->first;
		}

		void Assets::RemoveAssetSceneNode(const Ogre::String& name) {
			auto FindIt = _AssetSets.find(name);
			if (FindIt == _AssetSets.end())
				throw std::runtime_error("Cannot find Asset \'" + name + '\'');

			RemoveAssetSceneNode(FindIt);
		}

		void Assets::RemoveAssetSceneNode(AssetIterator it) {
			_Parent->removeChild(it->first);
			_AssetSets.erase(it);
		}

		Assets::ValueRange Assets::GetXRange() const {
			return _XRange;
		}

		void Assets::SetXRange(const ValueRange& x) {
			_XRange = x;
		}

		void Assets::SetXRange(const Ogre::Real& min, const Ogre::Real& max) {
			_XRange.first = min;
			_XRange.second = max;
		}

		Assets::ValueRange Assets::GetYRange() const {
			return _YRange;
		}

		void Assets::SetYRange(const ValueRange& y) {
			_YRange = y;
		}

		void Assets::SetYRange(const Ogre::Real& min, const Ogre::Real& max) {
			_YRange.first = min;
			_YRange.second = max;
		}
	}
}