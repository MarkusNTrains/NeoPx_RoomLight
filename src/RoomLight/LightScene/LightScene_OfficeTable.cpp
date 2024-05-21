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
#include "LightScene_OfficeTable.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene_OfficeTable::LightScene_OfficeTable(LightHdl* light_hdl, Datastore* datastore_p) : 
    LightScene(light_hdl, datastore_p, TASK_TmoMs, Parameter::Id::SceneOfficeTable_Brightness, Parameter::Id::SceneOfficeTable_Color)
{
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene_OfficeTable::~LightScene_OfficeTable()
{
}


//*****************************************************************************
// description:
//   Enter
//*****************************************************************************
void LightScene_OfficeTable::Enter()
{
    this->m_light_hdl_p->Clear();
    this->m_light_hdl_p->SetBrightness_Fade((uint8_t)this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    this->m_light_hdl_p->SetColor(this->m_datastore_p->GetParameter(this->m_color_param_id));
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   Exit
//*****************************************************************************
void LightScene_OfficeTable::Exit()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
bool LightScene_OfficeTable::TaskHdl()
{
  #if (ROOM_LIGHT == ROOM_LIGHT_MarkusNTrains)
    this->m_light_hdl_p->SetLedArea(0, 160, 1, (LedRow::LED_ROW_NOF - 3));  
    this->m_light_hdl_p->SetLedArea(0, 50, (LedRow::LED_ROW_NOF - 2), (LedRow::LED_ROW_NOF - 2));  
    this->m_light_hdl_p->SetLedArea(100, 160, (LedRow::LED_ROW_NOF - 2), (LedRow::LED_ROW_NOF - 2));  
    this->m_light_hdl_p->SetLedArea(0, 25, (LedRow::LED_ROW_NOF - 1), (LedRow::LED_ROW_NOF - 1));  
    this->m_light_hdl_p->SetLedArea(125, 160, (LedRow::LED_ROW_NOF - 1), (LedRow::LED_ROW_NOF - 1));  
  #elif (ROOM_LIGHT == ROOM_LIGHT_Altenglienicke)
    this->m_light_hdl_p->SetLedArea(150, 220, 1, 1);  
  #else
    this->m_light_hdl_p->SetLedArea(0, 3, 0, 0);  
    this->m_light_hdl_p->SetLedArea(10, LedRow::LED_ROW_LENGTH - 1, 0, 0);  
  #endif

    return true;
}


