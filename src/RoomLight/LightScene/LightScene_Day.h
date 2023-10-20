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

#ifndef _LIGHT_SCENE_DAY_H
#define _LIGHT_SCENE_DAY_H


//----------------------------------------------------------------------------
// include
#include "LightScene.h"
#include "LightScene_Night.h"


//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class LightScene_Day : public LightScene
{
    public:
        const static uint8_t BRIGHTNESS_Default = 150;
        const static uint8_t BRIGHTNESS_Min = LightScene_Night::BRIGHTNESS_Max + 20;
        const static uint32_t COLOR = LightHdl::COLOR_WHITE;

        LightScene_Day(LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_Day();
        
        void Enter();
        void Exit();

    private:
        const static uint32_t TASK_TmoMs = 1000ul;

        bool TaskHdl();
};

#endif // _LIGHT_SCENE_DAY_H
