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
#include "SimpleLedStrip.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
SimpleLedStrip::SimpleLedStrip(Datastore* datastore_p, uint32_t task_tmo_ms, 
    Parameter::Id lightscene_param_id, Parameter::Id brightness_param_id, Parameter::Id color_param_id, Parameter::Id white_param_id, 
    uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t warmwhite_pin, uint8_t coldwhite_pin)
{
    this->m_datastore_p = datastore_p;
    this->m_task_tmo_ms = task_tmo_ms;
    this->m_scene_param_id = lightscene_param_id;
    this->m_brightness_param_id = brightness_param_id;
    this->m_color_param_id = color_param_id;
    this->m_white_param_id = white_param_id;

    this->m_task_timestamp_ms = millis();

    pinMode(this->m_red_pin, OUTPUT);
    pinMode(this->m_green_pin, OUTPUT);
    pinMode(this->m_blue_pin, OUTPUT);
    pinMode(this->m_warmwhite_pin, OUTPUT);
    pinMode(this->m_coldwhite_pin, OUTPUT);

    this->Update();
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
SimpleLedStrip::~SimpleLedStrip()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
void SimpleLedStrip::Task()
{
    if (millis() - this->m_task_timestamp_ms > this->m_task_tmo_ms)
    {
        this->m_task_timestamp_ms = millis();

        this->Update();
    }
}


//*****************************************************************************
// description:
//   SetScene
//*****************************************************************************
void SimpleLedStrip::SetScene(Scene scene_id)
{
    this->m_datastore_p->SetParameter(this->m_scene_param_id, (uint8_t)scene_id);

    rgb_color_t rgb;
    white_t white;

    rgb.color = 0;
    white.white = 0;

    switch (scene_id)
    {
        case Scene::Off:
            rgb.color = 0;
            white.white = 0;
            break; 
        
        case Red:
            rgb.red = 255;

        default:
            break;
    }

    this->SetRGBColor(rgb);
    this->SetWhite(white);

    this->Update();
}


//*****************************************************************************
// description:
//   SetBrightness
//*****************************************************************************
void SimpleLedStrip::SetBrightness(uint8_t brightness)
{
    this->m_datastore_p->SetParameter(this->m_brightness_param_id, brightness);
}


//*****************************************************************************
// description:
//   SetColor
//*****************************************************************************
void SimpleLedStrip::SetRGBColor(rgb_color_t color)
{
    this->m_datastore_p->SetParameter(this->m_color_param_id, color.color);
}


//*****************************************************************************
// description:
//   SetWhite
//*****************************************************************************
void SimpleLedStrip::SetWhite(white_t white)
{
    this->m_datastore_p->SetParameter(this->m_color_param_id, white.white);
}


//*****************************************************************************
// description:
//   Update LED Strip
//*****************************************************************************
void SimpleLedStrip::Update()
{
    uint8_t brightness = this->m_datastore_p->GetParameter(this->m_brightness_param_id);
}