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

#ifndef _LIGHT_SCENE_DISCO_H
#define _LIGHT_SCENE_DISCO_H


//----------------------------------------------------------------------------
// include
#include "LightScene.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_Disco : public LightScene
{
    public:
        LightScene_Disco(LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_Disco();
        
        void Enter();
        void Exit();

    private:
        const uint32_t TASK_TmoMs = 100;

        void TaskHdl();
};

#endif // _LIGHT_SCENE_DISCO_H
