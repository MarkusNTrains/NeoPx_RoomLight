/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 07. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_LIGHTNING_H
#define _LIGHT_SCENE_LIGHTNING_H


//----------------------------------------------------------------------------
// include
#include "LightSceneHdl.h"


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_Lightning
{
    public:
        LightScene_Lightning(LightSceneHdl* parent, LightHdl* light_hdl, Datastore* datastore_p);
        ~LightScene_Lightning();
        void Enter();
        void Exit();
        bool Task();

    private:
        enum State
        {
            Darken = 0,
            Lightning,
            BrightenUp
        };

        // lightning task
        const static uint32_t TASK_TmoMs = 50;

        const static uint8_t FADING_Factor = 20;

        const static uint32_t FLASH_MaxWaitTimeMs = 5000;
        const static uint32_t FLASH_MaxDurationMs = 100;
        const static uint16_t FLASH_MaxLengthPx = 40;
        const static uint8_t FLASH_MinNof = 5;
        const static uint8_t FLASH_MaxNof = 20;
        const static uint8_t FLASH_MaxNofAfterEachOther = 5;

        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;
        Datastore* m_datastore_p;

        State m_state;
        uint32_t m_task_tmo_ms;
        uint32_t m_task_timestamp_ms;
        uint8_t m_nof_flashes;
        uint8_t m_flash_counter;
        wrgb_color_t m_background_color;

        uint8_t m_last_scene_brightness;
        wrgb_color_t m_last_scene_color;

        bool FadeColor(uint8_t target_color, uint8_t* current_color);
        void Leave();
};

#endif // _LIGHT_SCENE_LIGHTNING_H
