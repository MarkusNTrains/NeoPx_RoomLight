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


//----------------------------------------------------------------------------
// define


//----------------------------------------------------------------------------
// enum
enum class LightSceneID
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
class LightScene;
class LightScene_Cloud;
class LightScene_Day;
class LightScene_Disco;
class LightScene_Lightning;
class LightScene_LightOn;
class LightScene_MoBa;
class LightScene_OfficeTable;
class LightScene_Rainbow;
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
        void ChangeLightScene(LightSceneID scene);
        LightSceneID GetLightScene(void);
        LightSceneID GetLastScene(void);
        LightHdl* GetLightHdl(void);
        void GetUserSettingArea(LedArea* area);
        void SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        void SetBrightness(uint8_t brightness);
        void SetColor(uint32_t color);

    protected:

    private:
        const static uint32_t BRIGHTNESS_UPDATE_TMO_MS = 20;
        const static uint32_t TASK_SceneLightOff_TmoMs = 20;

        Datastore* m_datastore_p;
        uint32_t m_brightnessUpdate_timestamp_ms;
        uint32_t m_task_timestamp_ms;
        LightHdl* m_light_hdl_p;

        LightSceneID m_scene;
        LightSceneID m_last_scene;
        LightScene* m_active_light_scene_p;
        LightScene_Cloud* m_scene_cloud_p;
        LightScene_Day* m_scene_day_p;
        LightScene_Disco* m_scene_disco_p;
        LightScene_Lightning* m_scene_lightning_p;
        LightScene_LightOn* m_scene_light_on_p;
        LightScene_MoBa* m_scene_moba_p;
        LightScene_OfficeTable* m_scene_office_table_p;
        LightScene_Rainbow* m_scene_rainbow_p;
        LightScene_Sun* m_scene_sun_p;
        LightScene_UserSetting* m_scene_userSetting_p;


        // light scene
        void LightScene_LightOff_Task(void);
};

#endif // _LIGHT_SCENE_HDL_H
