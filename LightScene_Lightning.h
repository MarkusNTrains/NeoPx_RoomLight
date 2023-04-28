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
#include "common.h"
#include "LightSceneHdl.h"
#include "LightHdl.h"


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
        void Enter(void);
        void Exit(void);
        bool Task(void);

    private:
        // lightning task
        const static uint32_t MAX_FLASH_LENGHT_MS = 100;
        const static uint32_t MAX_FLASH_WIDTH_PX = 40;
        const static uint8_t MIN_NOF_FLASHES = 10;

        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;
        Datastore* m_datastore_p;

        uint32_t m_flash_timestamp_ms;
        uint32_t m_flash_pause_ms;
        uint8_t m_nof_flashes;
        uint8_t m_flash_counter;
        uint8_t m_last_brightness;
};

#endif // _LIGHT_SCENE_LIGHTNING_H
