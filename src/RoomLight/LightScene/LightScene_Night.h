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

#ifndef _LIGHT_SCENE_NIGHT_H
#define _LIGHT_SCENE_NIGHT_H


//----------------------------------------------------------------------------
// include
#include "LightScene.h"


//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class LightScene_Night : public LightScene
{
    public:
        const static uint8_t BRIGHTNESS_Min = 1;
        const static uint8_t BRIGHTNESS_Default = 4;
        const static uint8_t BRIGHTNESS_Max = 15;
        const static uint32_t COLOR = LightHdl::COLOR_BLUE;

        LightScene_Night(LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_Night();
        
        void Enter();
        void Exit();

    private:
        const static uint32_t TASK_TmoMs = 1000ul;

        bool TaskHdl();
};

#endif // _LIGHT_SCENE_NIGHT_H
