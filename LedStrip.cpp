/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 04. September 2019 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LedStrip.h"


//-----------------------------------------------------------------------------
// static module variable
//static Adafruit_NeoPixel m_pixel(16, 6, NEO_GRB + NEO_KHZ800);



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedStrip::LedStrip(uint8_t px_pin, uint16_t nof_px, uint8_t nof_row)
{
  m_nof_px = nof_px;
  m_nof_row = nof_row;
  m_state = POWER_ON;
  m_current_brightness = 0;
  m_desired_brightness = 20;
  m_update_time_ms = millis();
  
  //Adafruit_NeoPixel pixels(num_of_neo_px, neo_px_pin, NEO_GRB + NEO_KHZ800);
  m_pixel = new Adafruit_NeoPixel(nof_px, px_pin, NEO_GRB + NEO_KHZ800);
  m_pixel->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  m_pixel->clear();
  m_pixel->show();            // Turn OFF all pixels ASAP
  m_pixel->setBrightness(255); // Set brigthness for all neo pixels
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedStrip::~LedStrip()
{
}


//*****************************************************************************
// description:
//   Statemachine
//*****************************************************************************
void LedStrip::Tasks()
{
  if (millis() > this->m_update_time_ms + TMO_TILL_NEXT_UPDATE_MS)
  {
    this->m_update_time_ms = millis();
    
    switch (this->m_state)
    {
      case POWER_ON:
        this->m_current_brightness++;
        ShowOfficeTableWarmWhite(this->m_current_brightness);
        
        if (this->m_current_brightness >= this->m_desired_brightness)
        {
          this->m_state = IDLE;
        }
        break;
        
      case IDLE:
        break;
        
      default:
        break;
    }
  }
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::ShowOfficeTableWarmWhite(uint16_t brightness)
{
  uint32_t color = m_pixel->Color(0, 0, brightness);
  this->SetPixel(2, 2, 0, 1, color);
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedStrip::SetPixel(uint16_t start_pos, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color)
{
  uint16_t px = 0;
  uint16_t idx = 0;
  uint16_t row = 0;
  uint16_t cnt = 0;
  uint16_t offset = 0;
  uint16_t row_length = m_nof_px / m_nof_row;
  
  m_pixel->clear();

  if (start_pos >= row_length)
  { return; }
  
  if ((start_pos + width) > row_length)
  {
    width = row_length - start_pos;
  }

  for (row = 0; row < m_nof_row; row++)
  {
    offset = start_pos;
    for (cnt = 0; cnt < nof_repeat; cnt++)
    {
      for (px = 0; px < width; px++)
      {
        // check if position is further than the rowlength
        if ((offset + px) >= row_length) { break; } 
        
        if (row % 2)
        {
          idx = ((row + 1) * row_length) - 1 - offset - px;  // this is required if the pixel signal is zigzagged through the LED strips        
        }
        else
        {
          idx = (row * row_length) + offset + px;
        }
        m_pixel->setPixelColor(idx, color);    
      }
      
      offset += space + width;      
      // check if offset is further than the rowlength
      if (offset >= row_length) { break; }
    }
  }
  
  m_pixel->show();   // Send the updated pixel colors to the hardware.
}
