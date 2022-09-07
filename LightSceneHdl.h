/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/

#ifndef _LIGHT_SCENE_HDL_H
#define _LIGHT_SCENE_HDL_H


//----------------------------------------------------------------------------
// include
#include "common.h"
#include "LightHdl.h"
#include "LightScene_Lightning.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum
enum LightScene
{
    LIGHTSCENE_OfficeTableWW = 0,
    LIGHTSCENE_LightOnWW,
    LIGHTSCENE_Rainbow,
    LIGHTSCENE_Sunset,
    LIGHTSCENE_Sunrise,
    LIGHTSCENE_PowerOff,
    LIGHTSCENE_MovingDot,
    LIGHTSCENE_Sbh,
    LIGHTSCENE_UserSetting,
    LIGHTSCENE_Idle,
    LIGHTSCENE_Lightning,
    LIGHTSCENE_MoBa,
};


//----------------------------------------------------------------------------
// extern
class LightScene_Lightning;


//----------------------------------------------------------------------------
// class
class LightSceneHdl
{
    public:
        LightSceneHdl();
        ~LightSceneHdl();
        void Tasks(void);
        void ChangeLightScene(LightScene scene);
        void ChangeLightScene(LightScene scene, uint8_t brightness);
        LightScene GetLightScene(void);
        LightScene GetLastScene(void);
        LightHdl* GetLightHdl(void);
        void SetBrightness(uint8_t brightness);

    protected:

    private:
        const uint32_t TMO_TILL_NEXT_UPDATE_MS = 20;

        LightScene m_scene;
        LightScene m_last_scene;
        LightHdl* m_light_hdl_p;
        LightScene_Lightning* m_scene_lightning_p;
        uint32_t m_update_time_ms;

        // daylight tasks
        uint32_t m_sunrise_sun_height;
        uint32_t m_sunrise_sun_pos;
        
        // moving dot task
        uint16_t m_moving_dot_current_px;


        // light scene
        void LightScene_OfficeTableWW_Enter(uint16_t brightness);
        void LightScene_OfficeTableWW_Task(void);
        void LightScene_MoBa_Enter(uint16_t brightness);
        void LightScene_MoBa_Task(void);
        void LightScene_LightOnWW_Enter(uint16_t brightness);
        void LightScene_LightOnWW_Task(void);
        void LightScene_Sunrise_Task(void);
        void LightScene_PowerOff_Task(void);
        void LightScene_MovingDot_Task(void);
        void LightScene_UserSetting_Task(void);
        void LightScene_WhiteOverRainbow_Task(int whiteSpeed, int whiteLength);
};

#endif // _LIGHT_SCENE_HDL_H
