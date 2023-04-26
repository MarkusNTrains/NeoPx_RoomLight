/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 21. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_USER_SETTING_H
#define _LIGHT_SCENE_USER_SETTING_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "Datastore.h"
#include "LedArea.h"
#include "LightHdl.h"
#include "LightScene.h"



//----------------------------------------------------------------------------
// extern


//----------------------------------------------------------------------------
// class
class LightScene_UserSetting : public LightScene
{
    public:
        LightScene_UserSetting(LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_UserSetting();

        void Enter(void);
        void Exit(void);
        void Task(void);

        void GetLedArea(LedArea* area);
        void SetLedArea(LedArea* area);

    private:
        const static uint32_t TASK_TmoMs = 1000;

        Datastore* m_datastore_p;
        LightHdl* m_light_hdl_p;

        LedArea* m_led_area_p;

        void TaskHdl();
};

#endif // _LIGHT_SCENE_USER_SETTING_H
