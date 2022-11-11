/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 10. November 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_CLOUD_H
#define _LIGHT_SCENE_CLOUD_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LightSceneHdl.h"
#include "LightHdl.h"



//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum


//----------------------------------------------------------------------------
// struct
struct SkyCloud
{
    bool is_enable;
    uint8_t row;
    uint8_t speed;
    uint8_t darkness;
    uint16_t position_px;
    uint16_t length_px;
};


//----------------------------------------------------------------------------
// extern
class LightSceneHdl;


//----------------------------------------------------------------------------
// class
class LightScene_Cloud
{
    public:
        LightScene_Cloud(LightSceneHdl* parent, LightHdl* light_hdl);
        ~LightScene_Cloud();
        void Enter(void);
        void Exit(void);
        void Task(void);

    private:
        // lightning task
        const uint32_t TASK_HDL_TMO_MS = 50;
        const uint32_t MIN_CLOUD_SPEED = 1;
        const uint32_t MAX_CLOUD_SPEED = 10;
        const uint32_t MIN_CLOUD_WIDTH_PX = 10;
        const uint32_t MAX_CLOUD_WIDTH_PX = 150;
        const uint32_t MIN_CLOUD_DARKNESS = 255;
        const uint32_t MAX_CLOUD_DARKNESS = 255;
        const uint8_t MIN_NOF_CLOUDS = 5;
        static const uint8_t MAX_NOF_CLOUDS = 20;

        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;
        SkyCloud* m_cloud_p[MAX_NOF_CLOUDS];
        uint8_t m_nof_clouds;
        uint8_t m_scene_brightness;
        uint32_t m_scene_color;
        uint32_t m_task_hdl_timestamp_ms;
        uint32_t m_task_cycle_cnt;
        uint32_t m_start_cloud_timestamp_ms;
        uint32_t m_start_next_cloud_tmo_ms;
        uint8_t m_start_next_cloud_idx;
};

#endif // _LIGHT_SCENE_CLOUD_H
