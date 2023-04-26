/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_MOBA_H
#define _LIGHT_SCENE_MOBA_H


//----------------------------------------------------------------------------
// include
#include "LightScene.h"


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_MoBa : public LightScene
{
    public:
        LightScene_MoBa(LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_MoBa();
        
        void Enter();
        void Exit();

    private:
        const static uint32_t TASK_TmoMs = 1000;

        void TaskHdl();
};

#endif // _LIGHT_SCENE_MOBA_H
