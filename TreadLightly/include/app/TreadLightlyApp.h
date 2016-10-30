/*
-----------------------------------------------------------------------------
Filename:    TreadLightlyApp.h
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
#ifndef __TreadLightlyApp_h_
#define __TreadLightlyApp_h_

#include "BaseApplication.h"
#include "Map.h"

namespace TreadLightly {

	class TreadLightlyApp : public BaseApplication
	{
	public:
		TreadLightlyApp(void);
		virtual ~TreadLightlyApp(void);

	protected:
		virtual void createScene(void);

		/* ~~~ Members ~~~ */
		Map *_Map;
	};
}

#endif // #ifndef __TreadLightlyApp_h_
