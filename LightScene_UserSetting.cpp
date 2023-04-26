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
LightScene_UserSetting::LightScene_UserSetting(LightSceneHdl* parent, LightHdl* light_hdl, Datastore* datastore_p)
{
    this->m_datastore_p = datastore_p;
    this->m_scene_hdl_p = parent;
    this->m_light_hdl_p = light_hdl;

    uint16_t xs = (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Xs);
    uint16_t xe = (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Xe);
    uint16_t ys = (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Ys);
    uint16_t ye = (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Ye);

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
    this->m_led_area_p->Set(xs, xe, ys, ye, this->m_datastore_p->GetParameter(Datastore::ParameterId::Color));
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_UserSetting::~LightScene_UserSetting()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_UserSetting::Enter(void)
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetLedArea(this->m_led_area_p);
    this->m_light_hdl_p->Show();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_UserSetting::Exit(void)
{
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_UserSetting::Task(void)
{
    if (millis() - this->m_task_timestamp_ms > TASK_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->m_light_hdl_p->UpdateLedArea();
        this->m_light_hdl_p->Show();
    }
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
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Xs, area->GetColumnStart());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Xe, area->GetColumnEnd());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Ys, area->GetRowStart());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Ye, area->GetRowEnd());
}

