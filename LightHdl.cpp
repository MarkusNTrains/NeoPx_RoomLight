/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 9. September 2022 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LightHdl.h"


//-----------------------------------------------------------------------------
// typedef


//-----------------------------------------------------------------------------
// define


//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LightHdl::LightHdl()
{
    this->m_current_brightness = 0;
    this->m_desired_brightness = 0;
    this->m_color = COLOR_WHITE;
    this->m_update_time_ms = 0;
    this->m_led_matrix = new LedMatrix();
    this->m_led_area = new LedArea();
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LightHdl::~LightHdl()
{
    delete this->m_led_matrix;
    delete this->m_led_area;
}


//*****************************************************************************
// description:
//   Get Led Area
//*****************************************************************************
LedArea* LightHdl::GetLedArea(void)
{
    return this->m_led_area;  
}


//*****************************************************************************
// description:
//   Set Led Area
//*****************************************************************************
void LightHdl::SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    this->m_led_area->Set(xs, xe, ys, ye, this->m_color);
    this->m_led_matrix->SetPixelArray(xs, xe, ys, ye, this->m_color);    
}


//*****************************************************************************
// description:
//   Set Led Area
//*****************************************************************************
void LightHdl::SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye, uint32_t color)
{
    this->m_color = color;
    this->SetLedArea(xs, xe, ys, ye);    
}


//*****************************************************************************
// description:
//   Update LED area --> new color and new brightness are automatically used 
//   for all LEDs in this area
//*****************************************************************************
void LightHdl::UpdateLedArea(void)
{
    this->SetLedArea(this->m_led_area->GetColumnStart(), this->m_led_area->GetColumnEnd(), this->m_led_area->GetRowStart(), this->m_led_area->GetRowEnd());
}


//*****************************************************************************
// description:
//   Get Brightness
//*****************************************************************************
uint8_t LightHdl::GetBrightness(void)
{
    return this->m_desired_brightness;
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightHdl::SetBrightness_Fade(uint8_t brightness)
{
    this->m_desired_brightness = brightness;
}


//*****************************************************************************
// description:
//   Set Brightness
//*****************************************************************************
void LightHdl::SetBrightness_Instantly(uint8_t brightness)
{
    this->m_current_brightness = brightness;
    this->m_desired_brightness = brightness;
    this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
}


//*****************************************************************************
// description:
//   Update Brightness
//*****************************************************************************
void LightHdl::UpdateBrightness(void)
{
    uint8_t factor = 10;
    
    if (this->m_current_brightness < this->m_desired_brightness)
    {
        if (((this->m_current_brightness + ((this->m_current_brightness / factor) + 1))) < this->m_desired_brightness)
        {
            this->m_current_brightness += (this->m_current_brightness / factor) + 1;
        }
        else
        {
            this->m_current_brightness = this->m_desired_brightness;
        }
    }
    else if (this->m_current_brightness > this->m_desired_brightness)
    {
        if (((this->m_current_brightness - ((this->m_current_brightness / factor) + 1))) > this->m_desired_brightness)
        {
            this->m_current_brightness -= (this->m_current_brightness / factor) + 1;
        }
        else
        {
            this->m_current_brightness = this->m_desired_brightness;      
        }
    }
    else
    {
        // IDLE;
    }    
    
    this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
}


//*****************************************************************************
// description:
//   Get Color
//*****************************************************************************
uint32_t LightHdl::GetColor(void)
{
    return this->m_color;
}


//*****************************************************************************
// description:
//   Set Color
// parameter:
//   color: RGBW --> 8888
//*****************************************************************************
void LightHdl::SetColor(uint32_t color)
{
    this->m_color = color;
    this->m_led_matrix->SetColor(color);
}


//*****************************************************************************
// description:
//   Clear all LED
//*****************************************************************************
void LightHdl::Clear(void)
{
    this->m_led_matrix->Clear();
}


//*****************************************************************************
// description:
//   Show
//*****************************************************************************
void LightHdl::Show(void)
{
    this->m_led_matrix->Show();
}


//*****************************************************************************
// description:
//   Get Color
// parameter:
//   current_value: 
//   desired_value: value to change
//   change_val: max change of value
//*****************************************************************************
uint8_t LightHdl::UpdateValueTo(uint8_t current_value, uint8_t desired_value, uint8_t factor)
{
    if (current_value < desired_value)
    {
        if (((current_value + ((current_value / factor) + 1))) < desired_value)
        {
            current_value += (current_value / factor) + 1;
        }
        else
        {
            current_value = desired_value;
        }
    }
    else if (current_value > desired_value)
    {
        if (((current_value - ((current_value / factor) + 1))) > desired_value)
        {
            current_value -= (current_value / factor) + 1;
        }
        else
        {
            current_value = desired_value;      
        }
    }
    else
    {
        // IDLE;
    }  

    return current_value;  
}
