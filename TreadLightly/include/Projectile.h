/*
* TREAD LIGHTLY
* Authors: Brett Latham (26/10/16)
* File: Projectile.h
* Last Edited on: 26/10/16
* Purpose: This file describes the movement, collisions and destruction of a projectile.
*/

//Incomplete
#include "stdafx.h"


namespace TreadLightly {
	class Projectile {
	public:
		Projectile(Ogre::Vector3 location, Ogre::Vector3 direction,  Ogre::Real velocity = 10);
		virtual ~Projectile();
		virtual void updatePosition(); //internal collision function
		virtual void onDestroy();
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	private:
		Ogre::SceneNode* projectileNode;
		Ogre::Entity* projectile;
		Ogre::Real mass;
		Ogre::Real gravity;
		Ogre::Real startVelocity;
		Ogre::Vector3 position;
	}
}
