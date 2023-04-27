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
LightHdl::LightHdl(Datastore* datastore_p)
{
    this->m_datastore_p = datastore_p;

    this->m_current_brightness = 0;
    this->m_desired_brightness = 0;
    this->m_color = 0;
    this->m_update_time_ms = 0;
    this->m_led_matrix = new LedMatrix();
    this->m_led_area = new LedArea();

    this->SetBrightness_Instantly(0);
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
//   Get LED color
//*****************************************************************************
uint32_t LightHdl::GetLedColor(uint16_t row, uint16_t column)
{
    return this->m_led_matrix->GetPixelColor(row, column);
}


//*****************************************************************************
// description:
//   Set one LED
//*****************************************************************************
void LightHdl::SetLed(uint16_t row, uint16_t column, uint32_t color)
{
    this->m_led_matrix->SetPixel(row, column, color);
    //this->m_led_matrix->SetPixelArray(column, column, row, row, color);
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
//   Get Led Area
//*****************************************************************************
void LightHdl::SetLedArea(LedArea* area)
{
    this->SetLedArea(area->GetColumnStart(), area->GetColumnEnd(), area->GetRowStart(), area->GetRowEnd());
}


//*****************************************************************************
// description:
//   Set Led Area
//*****************************************************************************
void LightHdl::SetLedArea(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye)
{
    this->m_led_area->Set(xs, xe, ys, ye);
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
    this->SetBrightness_Fade(brightness);
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

        this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
        this->Show();
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

        this->m_led_matrix->SetBrightness(this->m_current_brightness); // Set brigthness for all neo pixels
        this->Show();
    }
    else
    {
        // IDLE;
    }    
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
