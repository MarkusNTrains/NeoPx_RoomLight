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
#include "RoomLightHdl.h"
#include "LightScene/LightScene_Cloud.h"
#include "LightScene/LightScene_Day.h"
#include "LightScene/LightScene_Disco.h"
#include "LightScene/LightScene_Lightning.h"
#include "LightScene/LightScene_LightOn.h"
#include "LightScene/LightScene_MoBa.h"
#include "LightScene/LightScene_Night.h"
#include "LightScene/LightScene_OfficeTable.h"
#include "LightScene/LightScene_Rainbow.h"
#include "LightScene/LightScene_Sun.h"
#include "LightScene/LightScene_UserSetting.h"



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
RoomLightHdl::RoomLightHdl(Datastore* datastore_p) : LightSource(datastore_p)
{
    this->m_light_hdl_p = new LightHdl();

    this->m_active_light_scene_p = nullptr;
    this->m_scene_cloud_p = new LightScene_Cloud(this, this->m_light_hdl_p);
    this->m_scene_day_p = new LightScene_Day(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_disco_p = new LightScene_Disco(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_light_on_p = new LightScene_LightOn(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_lightning_p = new LightScene_Lightning(this, this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_moba_p = new LightScene_MoBa(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_night_p = new LightScene_Night(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_office_table_p = new LightScene_OfficeTable(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_rainbow_p = new LightScene_Rainbow(this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_sun_p = new LightScene_Sun(this, this->m_light_hdl_p, this->m_datastore_p);
    this->m_scene_userSetting_p = new LightScene_UserSetting(this->m_light_hdl_p, this->m_datastore_p);

    this->m_brightnessUpdate_timestamp_ms = 0;
    this->m_scene_id = SceneID::LightOff;
    this->m_last_scene_id = SceneID::LightOff;
    this->m_active_scene_id = this->m_scene_id;
    this->m_task_timestamp_ms = 0;
    this->m_led_strip_updated_needed = false;


    // show last lightscene
#if ((DATASTORE_SaveLightScene == ON) && (DATASTORE_SaveDataOnEEPROM == ON))
    this->ChangeScene((uint8_t)(this->m_datastore_p->GetParameter(Parameter::Id::SceneID)));
#else
  #if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    this->ChangeScene((uint8_t)SceneID::OfficeTable);
  #else
    this->ChangeScene((uint8_t)SceneID::LightOn);
  #endif
#endif

    this->m_last_scene_id = this->m_scene_id;

    this->m_light_hdl_p->SetBrightness_Instantly(this->m_light_hdl_p->GetBrightness());
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->TaskHdl();
    }
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
RoomLightHdl::~RoomLightHdl()
{
    delete this->m_datastore_p;
    delete this->m_light_hdl_p;
    delete this->m_scene_cloud_p;
    delete this->m_scene_day_p;
    delete this->m_scene_disco_p;
    delete this->m_scene_light_on_p;
    delete this->m_scene_lightning_p;
    delete this->m_scene_moba_p;
    delete this->m_scene_night_p;
    delete this->m_scene_office_table_p;
    delete this->m_scene_rainbow_p;    
    delete this->m_scene_sun_p;
    delete this->m_scene_userSetting_p;
}


//*****************************************************************************
// description:
//   ChangeScene
//*****************************************************************************
void RoomLightHdl::ChangeScene(uint8_t scene_id)
{
    if (this->m_scene_id != (SceneID)scene_id)
    {
        //--- call Exit function of light scene ---
        if (this->m_active_light_scene_p != nullptr)
        {
            this->m_active_light_scene_p->Exit();
        }
        else
        {
            switch (this->m_scene_id)
            {
                case SceneID::Cloud:
                    this->m_scene_cloud_p->Exit();
                    break;
                    
                case SceneID::Idle:
                    break;

                case SceneID::Lightning:
                    this->m_scene_lightning_p->Exit();
                    break;
                    
                case SceneID::LightOff:
                    break;

                case SceneID::Sunrise:
                    break;

                case SceneID::Sunset:
                    break;
                
                default:
                    break;
            }
        }

        //--- change to requested scene ---
        switch (this->m_scene_id)
        {
            case SceneID::Cloud:
            case SceneID::Lightning:
            case SceneID::Sunrise:
            case SceneID::Sunset:
                // do not remember these scene as last scene
                break;

            default:
                this->m_last_scene_id = this->m_scene_id;
                break;
        }
        this->m_active_light_scene_p = nullptr;
        this->m_scene_id = (SceneID)scene_id;

#if (IS_DEBUG_MODE == ON)
        Serial.print(F("Change to Scene: "));
        Serial.println((int)scene_id);
#endif
    }
    
    //--- enter light scene -----
    bool save_light_scene = false;
    switch ((SceneID)scene_id)
    {
        case SceneID::Cloud:
            this->m_scene_cloud_p->Enter();
            break;

        case SceneID::Day:
            this->m_active_light_scene_p = this->m_scene_day_p;
            break;

        case SceneID::Lightning:
            this->m_scene_lightning_p->Enter();
            break;

        case SceneID::LightOn:
            this->m_active_light_scene_p = this->m_scene_light_on_p;
            this->m_active_scene_id = (SceneID)scene_id;
            save_light_scene = true;
            break;
        
        case SceneID::LightOff:
            this->m_light_hdl_p->SetBrightness_Fade(0);
            this->m_active_scene_id = (SceneID)scene_id;
            break;
        
        case SceneID::Disco:
            this->m_active_light_scene_p = this->m_scene_disco_p;
            this->m_active_scene_id = (SceneID)scene_id;
            break;

        case SceneID::MoBa:
            this->m_active_light_scene_p = this->m_scene_moba_p;
            this->m_active_scene_id = (SceneID)scene_id;
            save_light_scene = true;
            break;

        case SceneID::Night:
            this->m_active_light_scene_p = this->m_scene_night_p;
            break;
          
        case SceneID::OfficeTable:
            this->m_active_light_scene_p = this->m_scene_office_table_p;
            this->m_active_scene_id = (SceneID)scene_id;
            save_light_scene = true;
            break;
        
        case SceneID::Rainbow:
            this->m_active_light_scene_p = this->m_scene_rainbow_p;
            this->m_active_scene_id = (SceneID)scene_id;
            break;
            
        case SceneID::Sbh:
            //
            break;

        case SceneID::Sunrise:
            this->m_scene_sun_p->Sunrise_Enter();
            break;
            
        case SceneID::Sunset:
            this->m_scene_sun_p->Sunset_Enter();
            break;
          
        case SceneID::UserSetting:
            this->m_active_light_scene_p = this->m_scene_userSetting_p;
            this->m_active_scene_id = (SceneID)scene_id;
            save_light_scene = true;
            break;

        default:
            break;
    }

    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->Enter();
    }

    this->m_led_strip_updated_needed = true;

    // save light scene if needed
#if ((DATASTORE_SaveLightScene == ON) && (DATASTORE_SaveDataOnEEPROM == ON))
    if (save_light_scene == true)
    {
        this->m_datastore_p->SetParameter(Parameter::Id::SceneID, (uint8_t)scene_id);
    }
#endif
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void RoomLightHdl::Task()
{
    //--- update brightness -----------------------
    if (millis() - this->m_brightnessUpdate_timestamp_ms > BRIGHTNESS_UpdateTmoMs)
    {
        this->m_brightnessUpdate_timestamp_ms += BRIGHTNESS_UpdateTmoMs;
        if (this->m_light_hdl_p->UpdateBrightness() == true)
        {
            this->m_led_strip_updated_needed |= true;
            if (this->m_active_light_scene_p != nullptr)
            {
                // update strips while brightness did change, otherwise led color won't be correct
                this->m_active_light_scene_p->TaskHdl();
            }
        }
    }


    //--- run scene tasks -------------------------
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_led_strip_updated_needed |= this->m_active_light_scene_p->Task();
    }
    else
    {
        switch (this->m_scene_id)
        {
            case SceneID::Cloud:
                this->m_led_strip_updated_needed |= this->m_scene_cloud_p->Task();
                break;
                
            case SceneID::Idle:
                break;

            case SceneID::Lightning:
                this->m_led_strip_updated_needed |= this->m_scene_lightning_p->Task();
                break;
                
            case SceneID::LightOff:
                this->m_led_strip_updated_needed |= LightScene_LightOff_Task();
                break;

            case SceneID::Sunrise:
                this->m_led_strip_updated_needed |= this->m_scene_sun_p->Sunrise_Task();
                break;

            case SceneID::Sunset:
                this->m_led_strip_updated_needed |= this->m_scene_sun_p->Sunset_Task();
                break;
            
            default:
                break;
        }
    }


    // update led strip only at the end of task --> save run time
    if (this->m_led_strip_updated_needed == true)
    {
        this->m_led_strip_updated_needed = false;
        this->m_light_hdl_p->Show();
    }
}


//*****************************************************************************
// description:
//   Get Light Scene
//*****************************************************************************
uint8_t RoomLightHdl::GetActiveScene()
{
    return (uint8_t)this->m_active_scene_id;
}


//*****************************************************************************
// description:
//   Get Light Scene
//*****************************************************************************
uint8_t RoomLightHdl::GetActiveAnimation()
{
    return (uint8_t)this->m_scene_id;
}


//*****************************************************************************
// description:
//   Get Last Light Scene
//*****************************************************************************
RoomLightHdl::SceneID RoomLightHdl::GetLastScene()
{
    return this->m_last_scene_id;
}


//*****************************************************************************
// description:
//   Power Off Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool RoomLightHdl::LightScene_LightOff_Task()
{
    if (millis() - this->m_task_timestamp_ms > TASK_SceneLightOff_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        return true;
    }

    return false;
}


//*****************************************************************************
// description:
//   GetUserSettingArea
//*****************************************************************************
void RoomLightHdl::GetUserSettingArea(LedArea* area_p)
{
    this->m_scene_userSetting_p->GetLedArea(area_p);
}


//*****************************************************************************
// description:
//   SetUserSettingArea
//*****************************************************************************
void RoomLightHdl::SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    if (this->m_scene_id != SceneID::UserSetting)
    {
        this->ChangeScene((uint8_t)SceneID::UserSetting);
    }

    LedArea area;
    area.Set(xs, xe, ys, ye);
    this->m_scene_userSetting_p->SetLedArea(&area);
}


//*****************************************************************************
// description:
//   Get Brightness
// return:
//   brightness as 0 - 255
//*****************************************************************************
uint8_t RoomLightHdl::GetBrightness()
{
    return this->m_light_hdl_p->GetBrightness();
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void RoomLightHdl::SetBrightness(uint8_t brightness)
{
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_active_light_scene_p->SetBrightness(brightness);
    }
    else
    {
        if (this->m_scene_id == SceneID::LightOff)
        {
            this->m_scene_id = this->m_last_scene_id;
            this->ChangeScene((uint8_t)this->m_scene_id);
        }

        this->m_light_hdl_p->SetBrightness_Fade(brightness);
    }
}


//*****************************************************************************
// description:
//   Get Color
// return:
//   Color as format WRGB
//*****************************************************************************
uint32_t RoomLightHdl::GetColor()
{
    return this->m_light_hdl_p->GetColor();
}


//*****************************************************************************
// description:
//   Set Color
//*****************************************************************************
void RoomLightHdl::SetColor(uint32_t color)
{
    if (this->m_active_light_scene_p != nullptr)
    {
        this->m_led_strip_updated_needed |= this->m_active_light_scene_p->SetColor(color);
    }
    else
    {
        this->m_light_hdl_p->SetColor(color);
    }
}



