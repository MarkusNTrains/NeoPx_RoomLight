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
#include "SimpleLedStripHdl.h"



//-----------------------------------------------------------------------------
// typedef



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
SimpleLedStripHdl::SimpleLedStripHdl(Datastore* datastore_p, 
    Parameter::Id lightscene_param_id, Parameter::Id brightness_param_id, Parameter::Id color_param_id, Parameter::Id white_param_id, 
    uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t warmwhite_pin, uint8_t coldwhite_pin) : LightSource(datastore_p)
{
    this->m_scene_param_id = lightscene_param_id;
    this->m_brightness_param_id = brightness_param_id;
    this->m_color_param_id = color_param_id;
    this->m_white_param_id = white_param_id;
    this->m_red_pin = red_pin;
    this->m_green_pin = green_pin;
    this->m_blue_pin = blue_pin;
    this->m_warmwhite_pin = warmwhite_pin;
    this->m_coldwhite_pin = coldwhite_pin;

    this->m_task_timestamp_ms = millis();

    pinMode(this->m_red_pin, OUTPUT);
    pinMode(this->m_green_pin, OUTPUT);
    pinMode(this->m_blue_pin, OUTPUT);
    pinMode(this->m_warmwhite_pin, OUTPUT);
    pinMode(this->m_coldwhite_pin, OUTPUT);

    this->ChangeScene(SceneID::Off);
    this->Update();
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
SimpleLedStripHdl::~SimpleLedStripHdl()
{
}


//*****************************************************************************
// description:
//   Task
// return:
//   true if LightHdl::Show() needs to be called, else false
//*****************************************************************************
void SimpleLedStripHdl::Task()
{
    if (millis() - this->m_task_timestamp_ms > TASK_TmoMs)
    {
        this->m_task_timestamp_ms = millis();

        this->Update();
    }
}


//*****************************************************************************
// description:
//   GetActiveScene
//*****************************************************************************
uint8_t SimpleLedStripHdl::GetActiveScene()
{
    return (uint8_t)this->m_scene_id;
}


//*****************************************************************************
// description:
//   ChangeScene
//*****************************************************************************
void SimpleLedStripHdl::ChangeScene(uint8_t scene_id)
{
    this->m_scene_id = scene_id;
    this->m_datastore_p->SetParameter(this->m_scene_param_id, (uint8_t)scene_id);

    rgb_color_t rgb;
    white_t white;

    rgb.color = 0;
    white.white = 0;

    switch (scene_id)
    {
        case SceneID::Off:
            rgb.color = 0;
            white.white = 0;
            break; 
        
        case SceneID::Red:
            rgb.red = 255;
            break;

        case SceneID::Green:
            rgb.green = 255;
            break;

        case SceneID::Blue:
            rgb.blue = 255;
            break;

        case SceneID::WarmWhite:
            white.warm = 255;
            break;

        case SceneID::White:
            white.warm = 255;
            white.cold = 255;
            break;

        case SceneID::ColdWhite:
            white.cold = 255;
            break;

        default:
            break;
    }

    this->SetColor(rgb.color);
    this->SetWhite(white.white);

    this->Update();
}


//*****************************************************************************
// description:
//   GetColor
//*****************************************************************************
uint8_t SimpleLedStripHdl::GetBrightness()
{
    return (uint8_t)(this->m_datastore_p->GetParameter(this->m_brightness_param_id));
}


//*****************************************************************************
// description:
//   SetBrightness
//*****************************************************************************
void SimpleLedStripHdl::SetBrightness(uint8_t brightness)
{
    this->m_datastore_p->SetParameter(this->m_brightness_param_id, brightness);
    this->Update();
}


//*****************************************************************************
// description:
//   GetColor
//*****************************************************************************
uint32_t SimpleLedStripHdl::GetColor()
{
    return this->m_datastore_p->GetParameter(this->m_color_param_id);
}


//*****************************************************************************
// description:
//   SetColor
//*****************************************************************************
void SimpleLedStripHdl::SetColor(uint32_t color)
{
    color = color & 0xFFFFFF;
    this->m_datastore_p->SetParameter(this->m_color_param_id, color);
    this->Update();
}


//*****************************************************************************
// description:
//   GetWhite
//*****************************************************************************
uint16_t SimpleLedStripHdl::GetWhite()
{
    return (uint16_t)(this->m_datastore_p->GetParameter(this->m_white_param_id));
}


//*****************************************************************************
// description:
//   SetWhite
//*****************************************************************************
void SimpleLedStripHdl::SetWhite(uint16_t white)
{
    this->m_datastore_p->SetParameter(this->m_white_param_id, white);
    this->Update();
}


//*****************************************************************************
// description:
//   Update LED Strip
//*****************************************************************************
void SimpleLedStripHdl::Update()
{
    uint8_t brightness = this->m_datastore_p->GetParameter(this->m_brightness_param_id);
    rgb_color_t rgb;
    white_t white;
    rgb.color = this->m_datastore_p->GetParameter(this->m_color_param_id);
    white.white = (uint16_t)this->m_datastore_p->GetParameter(this->m_white_param_id);

    // update brightness
    rgb.red = (uint8_t)(((uint16_t)rgb.red * (uint16_t)brightness) / 255);
    rgb.green = (uint8_t)(((uint16_t)rgb.green * (uint16_t)brightness) / 255);
    rgb.blue = (uint8_t)(((uint16_t)rgb.blue * (uint16_t)brightness) / 255);
    white.warm = (uint8_t)(((uint16_t)white.warm * (uint16_t)brightness) / 255);
    white.cold = (uint8_t)(((uint16_t)white.cold * (uint16_t)brightness) / 255);

    // set pwm
    this->SetPWM(this->m_red_pin, rgb.red);
    this->SetPWM(this->m_green_pin, rgb.green);
    this->SetPWM(this->m_blue_pin, rgb.blue);
    this->SetPWM(this->m_warmwhite_pin, white.warm);
    this->SetPWM(this->m_coldwhite_pin, white.cold);

#if (IS_DEBUG_MODE == ON)
    Serial.print(F("PWM White: "));
    Serial.println(white.white);
#endif
}


//*****************************************************************************
// description:
//   Set PWM value for a LED color
//*****************************************************************************
void SimpleLedStripHdl::SetPWM(uint8_t pin, uint8_t pwm)
{
    analogWrite(pin, pwm);

#if (IS_DEBUG_MODE == ON)
    Serial.print(F("Pin: "));
    Serial.print(pin);
    Serial.print(F(" / PWM: "));
    Serial.println(pwm);
#endif
}