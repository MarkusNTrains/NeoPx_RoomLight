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
#include "LightScene_Day.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_Day::LightScene_Day(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Parameter::Id::SceneDay_Brightness, Parameter::Id::Unknown)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_Day::~LightScene_Day()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_Day::Enter()
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    this->m_light_hdl_p->SetColor(COLOR);
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_Day::Exit()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_Day::TaskHdl()
{
    this->m_light_hdl_p->SetLedArea(0, (LedRow::LED_ROW_LENGTH - 1), 0, (LedRow::LED_ROW_NOF - 1));  
    
    return true;
}


