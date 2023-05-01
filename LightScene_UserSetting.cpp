/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 21. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_UserSetting.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_UserSetting::LightScene_UserSetting(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Parameter::Id::SceneUserSetting_Brightness, Parameter::Id::SceneUserSetting_Color)
{
    this->m_datastore_p = datastore_p;
    this->m_light_hdl_p = light_hdl;

    uint16_t xs = (uint16_t)this->m_datastore_p->GetParameter(Parameter::Id::SceneUserSetting_Xs);
    uint16_t xe = (uint16_t)this->m_datastore_p->GetParameter(Parameter::Id::SceneUserSetting_Xe);
    uint16_t ys = (uint16_t)this->m_datastore_p->GetParameter(Parameter::Id::SceneUserSetting_Ys);
    uint16_t ye = (uint16_t)this->m_datastore_p->GetParameter(Parameter::Id::SceneUserSetting_Ye);

    if (xs >= ROOM_LIGHT_RowNofPx) {
        xs = ROOM_LIGHT_RowNofPx - 1;
    }
    if (xe >= ROOM_LIGHT_RowNofPx) {
        xe = ROOM_LIGHT_RowNofPx - 1;
    }

    if (ys >= ROOM_LIGHT_NofRows) {
        ys = ROOM_LIGHT_NofRows - 1;
    }
    if (ye >= ROOM_LIGHT_NofRows) {
        ye = ROOM_LIGHT_NofRows - 1;
    }

    this->m_led_area_p = new LedArea();
    this->m_led_area_p->Set(xs, xe, ys, ye);
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_UserSetting::~LightScene_UserSetting()
{
    delete this->m_led_area_p;
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_UserSetting::Enter()
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    this->m_light_hdl_p->SetColor(this->m_datastore_p->GetParameter(this->m_color_param_id));
    this->m_light_hdl_p->SetLedArea(this->m_led_area_p);
    this->m_light_hdl_p->Show();

    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_UserSetting::Exit()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_UserSetting::TaskHdl()
{
    this->m_light_hdl_p->UpdateLedArea();
    
    return true;
}


//*****************************************************************************
// description:
//   GetLedArea
//*****************************************************************************
void LightScene_UserSetting::GetLedArea(LedArea* area_p)
{
    this->m_led_area_p->Get(area_p);
}


//*****************************************************************************
// description:
//   SetLedArea
//*****************************************************************************
void LightScene_UserSetting::SetLedArea(LedArea* area)
{
    this->m_led_area_p->Set(area);

    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetLedArea(area);
    this->m_light_hdl_p->Show();

    // save parameter
    this->m_datastore_p->SetParameter(Parameter::Id::SceneUserSetting_Xs, area->GetColumnStart());
    this->m_datastore_p->SetParameter(Parameter::Id::SceneUserSetting_Xe, area->GetColumnEnd());
    this->m_datastore_p->SetParameter(Parameter::Id::SceneUserSetting_Ys, area->GetRowStart());
    this->m_datastore_p->SetParameter(Parameter::Id::SceneUserSetting_Ye, area->GetRowEnd());
}

