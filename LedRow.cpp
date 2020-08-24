/*******************************************************************************
Project   RoomLight

  This is an OpenSource Project.
  You can use, share or improve this project. If you improve this source code
  please share with the comunity or at least with the author of the original 
  source code
  
  Created 24. August 2020 by MarkusNTrains
================================================================================
$HeadURL:  $
$Id:  $
*******************************************************************************/


//-----------------------------------------------------------------------------
// includes
#include "LedRow.h"


//-----------------------------------------------------------------------------
// typedef



//-----------------------------------------------------------------------------
// static module variable



//*****************************************************************************
// description:
//   constructor
//*****************************************************************************
LedRow::LedRow(Adafruit_NeoPixel* led_strip_p, uint16_t* lookup_table_p, uint16_t start_px, uint16_t end_px)
{
  m_led_strip_p = led_strip_p;
  m_start_px = start_px;
  m_end_px = end_px;
  m_lookup_table_p = lookup_table_p;
}


//*****************************************************************************
// description:
//   destructor
//*****************************************************************************
LedRow::~LedRow()
{
}


//*****************************************************************************
// description:
//   Set Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t idx, uint32_t color)
{
  m_led_strip_p->setPixelColor(m_lookup_table_p[idx], color);
  m_led_strip_p->show();
}


//*****************************************************************************
// description:
//   Show White Pixel
//*****************************************************************************
void LedRow::SetPixel(uint16_t start_idx, uint16_t width, uint16_t space, uint16_t nof_repeat, uint32_t color)
{
/*  uint16_t px = 0;
  uint16_t idx = 0;
  uint16_t row = 0;
  uint16_t cnt = 0;
  uint16_t offset = 0;
  uint16_t row_length = m_nof_px / m_nof_row;
  
  this->m_pixel->clear();
  this->m_pixel_sbh->clear();

  if (start_idx >= row_length)
  { return; }
  
  if ((start_idx + width) > row_length)
  {
    width = row_length - start_pos;
  }

  for (row = 0; row < m_nof_row; row++)
  {
    offset = start_idx;
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
        m_pixel_sbh->setPixelColor(idx, color);
      }
      
      offset += space + width;      
      // check if offset is further than the rowlength
      if (offset >= row_length) { break; }
    }
  }
  
  m_pixel->show();   // Send the updated pixel colors to the hardware.
  m_pixel_sbh->show();   // Send the updated pixel colors to the hardware.
  */
}
