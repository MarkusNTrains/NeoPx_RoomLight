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
#include "../common.h"
#include "../Datastore/Datastore.h"
#include "../LightHdl/LightHdl.h"


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
class LightScene_Night;
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
        void Tasks();
        void ChangeLightScene(LightSceneID scene);
        LightSceneID GetActiveLightScene();
        LightSceneID GetActiveLightAnimation();
        LightSceneID GetLastScene();
        void GetUserSettingArea(LedArea* area);
        void SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye);
        uint8_t GetBrightness();
        void SetBrightness(uint8_t brightness);
        uint32_t GetColor();
        void SetColor(uint32_t color);

    protected:

    private:
        const static uint32_t BRIGHTNESS_UPDATE_TMO_MS = 40;
        const static uint32_t TASK_SceneLightOff_TmoMs = 40;

        Datastore* m_datastore_p;
        uint32_t m_brightnessUpdate_timestamp_ms;
        uint32_t m_task_timestamp_ms;
        LightHdl* m_light_hdl_p;
        bool m_led_strip_updated_needed;

        LightSceneID m_scene_id;
        LightSceneID m_active_scene_id;
        LightSceneID m_last_scene_id;
        LightScene* m_active_light_scene_p;
        LightScene_Cloud* m_scene_cloud_p;
        LightScene_Day* m_scene_day_p;
        LightScene_Disco* m_scene_disco_p;
        LightScene_Lightning* m_scene_lightning_p;
        LightScene_LightOn* m_scene_light_on_p;
        LightScene_MoBa* m_scene_moba_p;
        LightScene_Night* m_scene_night_p;
        LightScene_OfficeTable* m_scene_office_table_p;
        LightScene_Rainbow* m_scene_rainbow_p;
        LightScene_Sun* m_scene_sun_p;
        LightScene_UserSetting* m_scene_userSetting_p;


        // light scene
        bool LightScene_LightOff_Task();
};

#endif // _LIGHT_SCENE_HDL_H
