/*
-----------------------------------------------------------------------------
Filename:    OgreApplication.h
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
#ifndef __OgreApplication_h_
#define __OgreApplication_h_

#include "BaseApplication.h"

class OgreApplication : public BaseApplication
{
public:
    OgreApplication(void);
    virtual ~OgreApplication(void);

protected:
    virtual void createScene(void);
};

#endif // #ifndef __OgreApplication_h_
