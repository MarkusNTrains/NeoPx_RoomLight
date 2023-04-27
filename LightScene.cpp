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
#include "LightScene.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightScene::LightScene(LightHdl* light_hdl, Datastore* datastore_p, uint32_t task_tmo_m, Parameter::Id brightness_param_id, Parameter::Id color_param_id)
{
    this->m_datastore_p = datastore_p;
    this->m_light_hdl_p = light_hdl;
    this->m_task_tmo_ms = task_tmo_m;
    this->m_brightness_param_id = brightness_param_id;
    this->m_color_param_id = color_param_id;

    this->m_task_timestamp_ms = millis();
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightScene::~LightScene()
{
}


//*****************************************************************************
// description:
//   Task
//*****************************************************************************
void LightScene::Task()
{
    if (millis() - this->m_task_timestamp_ms > this->m_task_tmo_ms)
    {
        this->m_task_timestamp_ms = millis();

        this->TaskHdl();
    }
}


//*****************************************************************************
// description:
//   SetBrightness
//*****************************************************************************
void LightScene::SetBrightness(uint8_t brightness)
{
    this->m_datastore_p->SetParameter(this->m_brightness_param_id, brightness);
    this->m_light_hdl_p->SetBrightness_Fade(this->m_datastore_p->GetParameter(this->m_brightness_param_id));
    this->TaskHdl();
}


//*****************************************************************************
// description:
//   SetColor
//*****************************************************************************
void LightScene::SetColor(uint32_t color)
{
    this->m_datastore_p->SetParameter(this->m_color_param_id, color);
    this->m_light_hdl_p->SetColor(this->m_datastore_p->GetParameter(this->m_color_param_id));
    this->TaskHdl();
}
