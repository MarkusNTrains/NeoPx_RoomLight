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
#include "LightSourceHdl.h"



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
LightSourceHdl::LightSourceHdl()
{
    this->m_datastore_p = new Datastore();
    this->m_room_light_hdl_p = new RoomLightHdl(this->m_datastore_p);
    this->m_shadow_station_ligth_hdl_p = new SimpleLedStripHdl(this->m_datastore_p, 
            Parameter::Id::Sbf_SceneID, Parameter::Id::Sbf_Brightness, Parameter::Id::Sbf_Color, Parameter::Id::Sbf_White,
            SBF_LEDRed_Pin, SBF_LEDGreen_Pin, SBF_LEDBlue_Pin, SBF_LEDWarmWhite_Pin, SBF_LEDColdWhite_Pin);
    this->m_active_light_source_p = this->m_room_light_hdl_p;

    for (uint8_t id = 0; id < Source::Nof; id++)
    {
        if (LIGHT_SOURCE_EnableMap & (0x1 << id))
        {
            this->m_active_source = id;
            break;
        }
    }
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightSourceHdl::~LightSourceHdl()
{
    delete this->m_datastore_p;
    delete this->m_room_light_hdl_p;
    delete this->m_shadow_station_ligth_hdl_p;
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LightSourceHdl::Tasks()
{
    //--- run datsatore task ----------------------
    this->m_datastore_p->Task();

    //--- handle room light -----------------------
    this->m_room_light_hdl_p->Task();

    //--- handl shadow station light --------------
    this->m_shadow_station_ligth_hdl_p->Task();
}


//*****************************************************************************
// description:
//   GetActivateSource
//*****************************************************************************
LightSourceHdl::Source LightSourceHdl::GetActiveSource()
{
    return this->m_active_source;
}


//*****************************************************************************
// description:
//   ActivateSource
//*****************************************************************************
void LightSourceHdl::ActivateSource(Source source)
{
    switch (source)
    {
        case RoomLight:
            this->m_active_source = source;
            this->m_active_light_source_p = this->m_room_light_hdl_p;
            break;

        case ShadowStation:
            this->m_active_source = source;
            this->m_active_light_source_p = this->m_shadow_station_ligth_hdl_p;
            break;

        default:
            break;
    }
}


//*****************************************************************************
// description:
//   ChangeScene
//*****************************************************************************
void LightSourceHdl::ChangeScene(uint8_t scene_id)
{
    this->m_active_light_source_p->ChangeScene(scene_id);
}


//*****************************************************************************
// description:
//   GetActiveScene
//*****************************************************************************
uint8_t LightSourceHdl::GetActiveScene()
{
    return this->m_active_light_source_p->GetActiveScene();
}


//*****************************************************************************
// description:
//   GetActiveAnimation
//*****************************************************************************
uint8_t LightSourceHdl::GetActiveAnimation()
{
    return this->m_room_light_hdl_p->GetActiveAnimation();
}


//*****************************************************************************
// description:
//   Get Brightness
// return:
//   brightness as 0 - 255
//*****************************************************************************
uint8_t LightSourceHdl::GetBrightness()
{
    return this->m_active_light_source_p->GetBrightness();
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightSourceHdl::SetBrightness(uint8_t brightness)
{
    this->m_active_light_source_p->SetBrightness(brightness);
}


//*****************************************************************************
// description:
//   Get Color
//*****************************************************************************
uint32_t LightSourceHdl::GetColor()
{
    return this->m_active_light_source_p->GetColor();
}


//*****************************************************************************
// description:
//   Set Color
//*****************************************************************************
void LightSourceHdl::SetColor(uint32_t color)
{
    this->m_active_light_source_p->SetColor(color);
}


//*****************************************************************************
// description:
//   Get White
//*****************************************************************************
uint16_t LightSourceHdl::GetWhite()
{
    return this->m_shadow_station_ligth_hdl_p->GetWhite();
}


//*****************************************************************************
// description:
//   Set White
//*****************************************************************************
void LightSourceHdl::SetWhite(uint16_t white)
{
    this->m_shadow_station_ligth_hdl_p->SetWhite(white);
}


//*****************************************************************************
// description:
//   GetUserSettingArea
//*****************************************************************************
void LightSourceHdl::GetUserSettingArea(LedArea* area_p)
{
    return this->m_room_light_hdl_p->GetUserSettingArea(area_p);
}


//*****************************************************************************
// description:
//   SetUserSettingArea
//*****************************************************************************
void LightSourceHdl::SetUserSettingArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    this->m_room_light_hdl_p->SetUserSettingArea(xs, xe, ys, ye);
}

