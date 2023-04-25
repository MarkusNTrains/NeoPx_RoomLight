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
#include "LightScene_MoBa.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_MoBa::LightScene_MoBa(LightSceneHdl* parent, LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(parent, light_hdl, datastore_p, TASK_TmoMs, Datastore::ParameterId::SceneMoBa_Brightness, Datastore::ParameterId::SceneMoBa_Color)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_MoBa::~LightScene_MoBa()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_MoBa::Enter()
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
void LightScene_MoBa::Exit()
{
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene_MoBa::TaskHdl()
{
  #if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 0, 0);  
    this->m_light_hdl_p->SetLedArea(0, 30, 1, 2);  
    this->m_light_hdl_p->SetLedArea(LedRow::LED_ROW_LENGTH - 30, LedRow::LED_ROW_LENGTH, 1, 2);  
    this->m_light_hdl_p->SetLedArea(0, LedRow::LED_ROW_LENGTH, 3, 3);  
    this->m_light_hdl_p->Show();
  #elif (ROOM_LIGHT == ROOM_LIGHT_Altenglienicke)
    this->m_light_hdl_p->Show();
  #else
    this->m_light_hdl_p->SetLedArea(3, 5, 0, 0);  
    this->m_light_hdl_p->SetLedArea(9, LedRow::LED_ROW_LENGTH - 3, 0, 0);  
    this->m_light_hdl_p->Show();
  #endif
}


