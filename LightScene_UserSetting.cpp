/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 7. September 2022 by MarkusNTrains
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

    this->m_led_area_p = new LedArea();
    this->m_led_area_p->Set(
        (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Xs),
        (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Xe),
        (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Ys),
        (uint16_t)this->m_datastore_p->GetParameter(Datastore::ParameterId::UserSetting_Ye),
        this->m_datastore_p->GetParameter(Datastore::ParameterId::Color)
    );
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
    //this->m_light_hdl_p->SetLedArea(area);
    this->m_light_hdl_p->SetLedArea(area->GetColumnStart(), area->GetColumnEnd(), area->GetRowStart(), area->GetRowEnd(), area->GetColor());
    this->m_light_hdl_p->Show();

    // save parameter
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Xs, area->GetColumnStart());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Xe, area->GetColumnEnd());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Ys, area->GetRowStart());
    this->m_datastore_p->SetParameter(Datastore::ParameterId::UserSetting_Ye, area->GetRowEnd());
}

