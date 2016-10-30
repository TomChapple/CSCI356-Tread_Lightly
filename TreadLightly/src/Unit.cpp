#include "stdafx.h"

#include "Unit.h"

const Ogre::Real TANK_SPEED = 20.0f;
const Ogre::Real MOVE_TO_THRESHOLD = 5.0f;
const Ogre::Vector3 TANK_SCALE(0.2, 0.2, 0.2);

namespace TreadLightly {

	Unit::Unit(Ogre::SceneManager *mgr, Map *map,
		const Ogre::Vector3& pos):
		_SceneManager(mgr), _Map(map), _CurrDest(Ogre::Vector3::ZERO), _Velocity(Ogre::Vector3::ZERO),
		_Node(NULL), _Entity(NULL), _ReachedDestination(true) {

		/* Create the SceneNode and Entities for this */
		_Node = _SceneManager->getRootSceneNode()->createChildSceneNode(pos);
		_Entity = _SceneManager->createEntity("lpbody.mesh");
		_Entity->setMaterialName("lp_body_material");
		_Node->attachObject(_Entity);
		_Node->setScale(TANK_SCALE);
	}

	Unit::~Unit() {

	}

	bool Unit::update(const Ogre::Real& t) {

		/* Check if we are moving to our next destination */
		if (!_ReachedDestination) {

			/* Check if we are in range */
			if (_CurrDest.distance(_Node->getPosition()) < MOVE_TO_THRESHOLD) {
				_ReachedDestination = true;
			}
			else {
				_Velocity = _CurrDest - _Node->getPosition();
				_Velocity.normalise();
				_Velocity *= TANK_SPEED * t;
			}
		}

		if (_ReachedDestination) {

			/* Check if there is somewhere to go next */
			if (!_FollowPath.empty()) {
				_CurrDest = _FollowPath.front();
				_FollowPath.erase(_FollowPath.begin());
				_ReachedDestination = false;
			}
		}

		/* Perform Movement */
		_Node->translate(_Velocity);

		return true;
	}

	void Unit::MoveTo(const Ogre::Vector3& pos) {

		/* Erase current path and stop movement */
		_FollowPath.clear();
		_ReachedDestination = true;

		/* Find a path to it */
		std::vector<Ogre::Vector3> FoundPath;
		bool FindResult = _Map->FindPath(_Node->getPosition(), pos, FoundPath);
		if (FindResult) {
			_FollowPath.insert(_FollowPath.begin(), FoundPath.begin(), FoundPath.end());
			_ReachedDestination = false;
		}
	}
}