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
#include "Datastore.h"
#include "LightHdl.h"
#include "LightScene_Cloud.h"
#include "LightScene_Lightning.h"
#include "LightScene_Sun.h"
#include "LightScene_UserSetting.h"


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum
enum class LightScene
{
    OfficeTable = 0,
    LightOn,
    Rainbow,
    Sunset,
    Sunrise,
    LightOff,
    Disco,
    Sbh,
    UserSetting,
    Idle,
    Lightning,
    MoBa,
    Day,
    Night,
    Cloud,
    Nof
};


//----------------------------------------------------------------------------
// extern
class LightScene_Cloud;
class LightScene_Lightning;
class LightScene_Sun;
class LightScene_UserSetting;


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
        void GetUserSettingArea(LedArea* area);
        void SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        //this->m_light_scene->ChangeLightScene(LightScene::UserSetting);
        //this->m_light_scene->GetLightHdl()->SetLedArea(param, param2, param3, param4);
        void SetBrightness(uint8_t brightness);

    protected:

    private:
        const uint32_t BRIGHTNESS_UPDATE_TMO_MS = 20;
        const uint32_t TASK_SceneLightOn_TmoMs = 1000;
        const uint32_t TASK_SceneLightOff_TmoMs = 20;
        const uint32_t TASK_SceneMoBa_TmoMs = 1000;
        const uint32_t TASK_SceneDisco_TmoMs = 20;
        const uint32_t TASK_SceneRainbow_TmoMs = 20;
        const uint32_t TASK_SceneOfficeTable_TmoMs = 1000;

        Datastore* m_datastore_p;
        uint32_t m_brightnessUpdate_timestamp_ms;
        uint32_t m_task_timestamp_ms;
        LightScene m_scene;
        LightScene m_last_scene;
        LightHdl* m_light_hdl_p;
        LightScene_Cloud* m_scene_cloud_p;
        LightScene_Lightning* m_scene_lightning_p;
        LightScene_Sun* m_scene_sun_p;
        LightScene_UserSetting* m_scene_userSetting_p;

        // moving dot task
        uint16_t m_moving_dot_current_px;

        // rainbow
        uint16_t m_rainbow_firstPixelHue;


        // light scene
        void LightScene_OfficeTable_Enter(uint16_t brightness);
        void LightScene_OfficeTable_Task(void);
        void LightScene_MoBa_Enter(uint16_t brightness);
        void LightScene_MoBa_Task(void);
        void LightScene_LightOn_Enter(uint16_t brightness);
        void LightScene_LightOn_Task(void);
        void LightScene_LightOff_Task(void);
        void LightScene_Disco_Task(void);
        void LightScene_WhiteOverRainbow_Task(void);
};

#endif // _LIGHT_SCENE_HDL_H
