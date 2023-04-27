#include "Arduino.h"
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


//-----------------------------------------------------------------------------
// includes
#include "LightSceneHdl.h"
#include "LightScene_Cloud.h"
#include "LightScene_Day.h"
#include "LightScene_Disco.h"
#include "LightScene_Lightning.h"
#include "LightScene_LightOn.h"
#include "LightScene_MoBa.h"
#include "LightScene_Night.h"
#include "LightScene_OfficeTable.h"
#include "LightScene_Rainbow.h"
#include "LightScene_Sun.h"
#include "LightScene_UserSetting.h"



//-----------------------------------------------------------------------------
// typedef


//-----------------------------------------------------------------------------
// define


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightSceneHdl::LightSceneHdl()
{
    this->m_datastore_p = new Datastore();
    this->m_light_hdl_p = new LightHdl(this->m_datastore_p);

    this->m_active_light_scene_p = nullptr;
    this->m_scene_cloud_p = new LightScene_Cloud(this, this->m_light_hdl_p);
    this->m_scene_day_p = new LightScene_Day(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_disco_p = new LightScene_Disco(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_lightning_p = new LightScene_Lightning(this, this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_light_on_p = new LightScene_LightOn(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_moba_p = new LightScene_MoBa(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_night_p = new LightScene_Night(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_office_table_p = new LightScene_OfficeTable(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_rainbow_p = new LightScene_Rainbow(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_sun_p = new LightScene_Sun(this, this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_userSetting_p = new LightScene_UserSetting(this->m_light_hdl_p, this->m_datastore_p);

    this->m_brightnessUpdate_timestamp_ms = 0;
    this->m_scene = LightSceneID::LightOff;
    this->m_last_scene = LightSceneID::LightOff;
    this->m_task_timestamp_ms = 0;

    // get last lightscene from datastore
    this->ChangeLightScene((LightSceneID)(this->m_datastore_p->GetParameter(Parameter::Id::LightSceneID)));
    //this->ChangeLightScene(LightSceneID::OfficeTable);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightSceneHdl::~LightSceneHdl()
{
    delete this->m_light_hdl_p;
    delete this->m_scene_cloud_p;
    delete this->m_scene_lightning_p;
    delete this->m_scene_sun_p;
}


//*****************************************************************************
// description:
//   ChangeLightScene
//*****************************************************************************
void LightSceneHdl::ChangeLightScene(LightSceneID scene)
{
    if (this->m_scene != scene)
    {
        this->m_active_light_scene_p = nullptr;
        this->m_last_scene = this->m_scene;
        this->m_scene = scene;
#if (IS_DEBUG_MODE == ON)
        Serial.print(F("Change to Scene: "));
        Serial.println((int)scene);
#endif
    }
    
    //--- enter light scene -----
    bool save_light_scene = false;
    switch (scene)
    {
        case LightSceneID::Cloud:
            this->m_scene_cloud_p->Enter();
            break;

        case LightSceneID::Day:
            this->m_active_light_scene_p = this->m_scene_day_p;
            break;

        case LightSceneID::Lightning:
            this->m_scene_lightning_p->Enter();
            break;

        case LightSceneID::LightOn:
            this->m_active_light_scene_p = this->m_scene_light_on_p;
            save_light_scene = true;
            break;
        
        case LightSceneID::LightOff:
            this->m_light_hdl_p->SetBrightness_Fade(0);
            break;
        
        case LightSceneID::Disco:
            this->m_active_light_scene_p = this->m_scene_disco_p;
            save_light_scene = true;
            break;

        case LightSceneID::MoBa:
            this->m_active_light_scene_p = this->m_scene_moba_p;
            save_light_scene = true;
            break;

        case LightSceneID::Night:
            this->m_active_light_scene_p = this->m_scene_night_p;
            break;
          
        case LightSceneID::OfficeTable:
            this->m_active_light_scene_p = this->m_scene_office_table_p;
            save_light_scene = true;
            break;
        
        case LightSceneID::Rainbow:
            this->m_active_light_scene_p = this->m_scene_rainbow_p;
            save_light_scene = true;
            break;
            
        case LightSceneID::Sbh:
            //
            break;

        case LightSceneID::Sunrise:
            this->m_scene_sun_p->Sunrise_Enter();
            break;
            
        case LightSceneID::Sunset:
            this->m_scene_sun_p->Sunset_Enter();
            break;
          
        case LightSceneID::UserSetting:
            this->m_active_light_scene_p = this->m_scene_userSetting_p;
            save_light_scene = true;
            break;

        default:
            break;
    }

    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->Enter();
    }

    // save light scene if needed
    if (save_light_scene == true)
    {
        this->m_datastore_p->SetParameter(Parameter::Id::LightSceneID, (uint8_t)scene);
    }
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LightSceneHdl::Tasks()
{
    //--- run datsatore task ----------------------
    this->m_datastore_p->Task();

    //--- update brightness -----------------------
    if (millis() - this->m_brightnessUpdate_timestamp_ms > BRIGHTNESS_UPDATE_TMO_MS)
    {
        this->m_brightnessUpdate_timestamp_ms += BRIGHTNESS_UPDATE_TMO_MS;
        this->m_light_hdl_p->UpdateBrightness();
    }

    //--- run scene tasks -------------------------
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->Task();
    }
    else
    {
        switch (this->m_scene)
        {
            case LightSceneID::Cloud:
                this->m_scene_cloud_p->Task();
                break;
                
            case LightSceneID::Idle:
                break;

            case LightSceneID::Lightning:
                this->m_scene_lightning_p->Task();
                break;
                
            case LightSceneID::LightOff:
                LightScene_LightOff_Task();
                break;

            case LightSceneID::Sunrise:
                this->m_scene_sun_p->Sunrise_Task();
                break;

            case LightSceneID::Sunset:
                this->m_scene_sun_p->Sunset_Task();
                break;
            
            default:
                break;
        }
    }
}


//*****************************************************************************
// description:
//   Get Light Scene
//*****************************************************************************
LightSceneID LightSceneHdl::GetLightScene(void)
{
    return this->m_scene;
}


//*****************************************************************************
// description:
//   Get Last Light Scene
//*****************************************************************************
LightSceneID LightSceneHdl::GetLastScene(void)
{
    return this->m_last_scene;
}


//*****************************************************************************
// description:
//   GetLightHdl
//*****************************************************************************
LightHdl* LightSceneHdl::GetLightHdl(void)
{
    return this->m_light_hdl_p;
}


//*****************************************************************************
// description:
//   Power Off Task
//*****************************************************************************
void LightSceneHdl::LightScene_LightOff_Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneLightOff_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   GetUserSettingArea
//*****************************************************************************
void LightSceneHdl::GetUserSettingArea(LedArea* area_p)
{
    this->m_scene_userSetting_p->GetLedArea(area_p);
}


//*****************************************************************************
// description:
//   SetUserSettingArea
//*****************************************************************************
void LightSceneHdl::SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    if (this->m_scene != LightSceneID::UserSetting)
    {
        this->ChangeLightScene(LightSceneID::UserSetting);
    }

    LedArea area;
    area.Set(xs, xe, ys, ye);
    this->m_scene_userSetting_p->SetLedArea(&area);
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightSceneHdl::SetBrightness(uint8_t brightness)
{
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->SetBrightness(brightness);
    }
    else
    {
        if (this->m_scene == LightSceneID::LightOff)
        {
            this->m_scene = this->m_last_scene;
            this->ChangeLightScene(m_scene);
        }

        this->m_light_hdl_p->SetBrightness_Fade(brightness);
    }
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightSceneHdl::SetColor(uint32_t color)
{
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->SetColor(color);
    }
    else
    {
        this->m_light_hdl_p->SetColor(color);
    }
}



