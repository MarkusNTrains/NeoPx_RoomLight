/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 25. April 2023 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightScene_LightOn.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_LightOn::LightScene_LightOn(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Datastore::ParameterId::SceneLightOn_Brightness, Datastore::ParameterId::SceneLightOn_Color)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_LightOn::~LightScene_LightOn()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_LightOn::Enter()
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id), false);
    this->m_light_hdl_p->SetColor(this->m_datastore_p->GetParameter(this->m_color_param_id));
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_LightOn::Exit()
{
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_LightOn::TaskHdl()
{
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, (LedRow::LED_ROW_NOF - 1));  
    this->m_light_hdl_p->Show();
}


