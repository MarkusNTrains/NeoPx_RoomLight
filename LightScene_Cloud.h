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
    uint16_t width;
};


//----------------------------------------------------------------------------
// const
const uint32_t LIGHTSCENE_CLOUD_Task_TmoMs = 50;

const uint32_t LIGHTSCENE_CLOUD_Cloud_AddNextMinTmoMs = 200;
const uint32_t LIGHTSCENE_CLOUD_Cloud_AddNewMaxLengthMs = 60000;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MinSpeed = 1;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MaxSpeed = 6;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MinLengthPx = 20;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MaxLengthPx = 100;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MinWidth = 1;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MaxWidth = LedRow::LED_ROW_NOF;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MinDarkness = 180;
const uint32_t LIGHTSCENE_CLOUD_Cloud_MaxDarkness = 255;
const uint8_t LIGHTSCENE_CLOUD_Cloud_MinNof = 5;
const uint8_t LIGHTSCENE_CLOUD_Cloud_MaxNof = 30;


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
        LightSceneHdl* m_scene_hdl_p;
        LightHdl* m_light_hdl_p;
        SkyCloud* m_cloud_p[LIGHTSCENE_CLOUD_Cloud_MaxNof];
        uint8_t m_nof_clouds;
        uint32_t m_scene_color;
        uint32_t m_task_hdl_timestamp_ms;
        uint32_t m_task_cycle_cnt;
        uint32_t m_start_cloud_timestamp_ms;
        uint32_t m_start_next_cloud_tmo_ms;
        uint8_t m_start_next_cloud_idx;
};

#endif // _LIGHT_SCENE_CLOUD_H
